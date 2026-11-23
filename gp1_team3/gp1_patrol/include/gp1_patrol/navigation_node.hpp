/**
 * @file navigation_node.hpp
 * @author Andrew Schug (aschug@umd.edu)
 * @brief ROS 2 navigation node responsible for navigating the stand-in robot through four checkpoints
 * using odometry feedback. Publishes velocity commands and arrival notifications, waits for inspection
 * reports, and handles emergency stop/resume commands.
 * 
 * @version 0.1
 * @date 2026-11-20
 * 
 * @copyright Copyright (c) 2026
 * 
 */


#pragma once

#include <cstddef>
#include <memory>
#include <chrono>
#include <functional>

#include <rclcpp/rclcpp.hpp>

#include <geometry_msgs/msg/twist.hpp>
#include <nav_msgs/msg/odometry.hpp>

#include <gp1_interfaces/msg/arrival.hpp>
#include <gp1_interfaces/msg/inspection_report.hpp>
#include <gp1_interfaces/msg/stop_command.hpp>

namespace gp1_patrol {

class NavigationNode : public rclcpp::Node {
public:

    /**
     * @brief Constructs the navigation node and initializes all the publishers, subscribers, and timer used for navigation
     */
    NavigationNode()
    : rclcpp::Node{"navigation_node"},
    

        velocity_publisher_{create_publisher<geometry_msgs::msg::Twist>( "cmd_vel", 10)}, 
        arrival_publisher_{create_publisher<gp1_interfaces::msg::Arrival>( "arrival", 10)},

        odometry_subscriber_{create_subscription<nav_msgs::msg::Odometry>(
                "odometry/filtered", 10, 
                std::bind(&NavigationNode::odometry_callback, this, std::placeholders::_1))},

        inspection_report_subscriber_{create_subscription<gp1_interfaces::msg::InspectionReport>(
              "inspection_report", 10,
              std::bind(&NavigationNode::inspection_report_callback, this, std::placeholders::_1))},

        stop_command_subscriber_{create_subscription<gp1_interfaces::msg::StopCommand>(
              "/emergency_stop", 10,
              std::bind(&NavigationNode::stop_command_callback, this, std::placeholders::_1))},

        navigation_timer_{create_wall_timer(
              std::chrono::milliseconds{100},
              std::bind(&NavigationNode::navigation_callback, this))} {}

private:
    
    /**
     * @brief Updates the robot's current pose from odometry.
     * 
     * @param msg Shared pointer to the received odometry message.
     */
    void odometry_callback(const nav_msgs::msg::Odometry::SharedPtr msg);


    /**
     * @brief Processes inspection reports and goes to the next checkpoint.
     * 
     * @param msg Received inspection report from the mission node.
     */
    void inspection_report_callback(const gp1_interfaces::msg::InspectionReport::SharedPtr msg);

    /**
     * @brief Handles emergency stop and resume commands
     * 
     * @param msg Received operator command containing the stop state
     */
    void stop_command_callback(const gp1_interfaces::msg::StopCommand::SharedPtr msg);

    /**
     * @brief Calculates and publishes navigation commands at 10Hz. 
     * Rotates towrds the current checkpoint before going forward.
     * 
     */
    void navigation_callback();

    /**
     * @brief Publishes a zero velocity command.
     * 
     */
    void publish_stop();

    /// Publishers
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr velocity_publisher_;
    rclcpp::Publisher<gp1_interfaces::msg::Arrival>::SharedPtr arrival_publisher_;

    /// Subscribers
    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odometry_subscriber_;
    rclcpp::Subscription<gp1_interfaces::msg::InspectionReport>::SharedPtr inspection_report_subscriber_;
    rclcpp::Subscription<gp1_interfaces::msg::StopCommand>::SharedPtr stop_command_subscriber_;

    /// Timer at 10Hz
    rclcpp::TimerBase::SharedPtr navigation_timer_;

    /// Robot pose
    double current_x_{0.0};
    double current_y_{0.0};
    double current_yaw_{0.0};
    
    /// Checks whether valid odometry has been received
    bool has_odometry_{false};

    // Checkpoint progress index (0-3)
    std::size_t checkpoint_index_{0};

    /// Check for waiting for the mission node to complete inspection
    bool waiting_for_report_{false};

    /// Emergency stop state check
    bool emergency_stopped_{false};
};

}  // namespace gp1_patrol