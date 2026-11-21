/**
 * @file stand_in_robot_node.hpp
 * @author Jacob Collier-Tenison (jacobct@umd.edu)
 * @brief Subscribes to cmd_vel, moves an imaginary robot, and publishes odometry/filtered and scan. 
 * The scan comes from a table that depends on where the robot stands.
 * 
 */
#pragma once
#include <chrono>
#include <functional>
#include <rclcpp/rclcpp.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>
#include <geometry_msgs/msg/twist.hpp>

namespace gp1_patrol {

class StandInRobotNode : public rclcpp::Node {
    public:
        /**
         * @brief Construct a new Stand In Robot Node. 
         * Has a published object to odometry to update the robot's location, a 50ms timer to
         * publish new odometry, a publisher to scan to publish simulates laser scans, a timer to
         * publish a new scan every 200ms, and a subscriber to cmd vel to move the imaginary robot
         * as requested.
         */
        StandInRobotNode() 
        : rclcpp::Node("stand_in_robot_node"), 
          odometry_publisher_{create_publisher<nav_msgs::msg::Odometry>("odometry/filtered", 10)},
          odometry_timer_{create_wall_timer(std::chrono::milliseconds(50), std::bind(&StandInRobotNode::publish_odometry, this))},
          scan_publisher_{create_publisher<sensor_msgs::msg::LaserScan>("scan", 10)},
          scan_timer_{create_wall_timer(std::chrono::milliseconds(200), std::bind(&StandInRobotNode::publish_scan, this))},
          cmd_vel_subscriber_{create_subscription<geometry_msgs::msg::Twist>("cmd_vel", 10, std::bind(&StandInRobotNode::update_position, this, std::placeholders::_1))}
        {}

    private:
        /**
         * @brief Publishes the robot's position (x, y) and orientation (rotation about z).
         * 
         */
        void publish_odometry();
        /**
         * @brief Publishes a laser scan with values determined by lookup table.
         * 
         */
        void publish_scan();
        /**
         * @brief Updates the robots position based on a velocity command.
         * 
         * @param msg The received velocity command.
         */
        void update_position(const geometry_msgs::msg::Twist &msg);
        rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odometry_publisher_; // Publisher to odometry/filtered topic
        rclcpp::TimerBase::SharedPtr odometry_timer_; // Timer used to publish new odometry every 50 ms
        rclcpp::Publisher<sensor_msgs::msg::LaserScan>::SharedPtr scan_publisher_; // Publisher to scan topic
        rclcpp::TimerBase::SharedPtr scan_timer_; // Timer used to publish a laser scan every 200 ms
        rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_subscriber_; // A subscriber to cmd_vel topic
        double x_{0.0};                     // robot x-position in meters
        double y_{0.0};                     // robot y-position in meters
        double heading_{0.0};               // robot yaw in radians
        bool near_checkpoint_{false};       // true if robot is within distance of checkpoint, false otherwise
        rclcpp::Time last_vel_time_{now()}; // time of last velocity callback, in seconds
};

} // namespace gp1_patrol