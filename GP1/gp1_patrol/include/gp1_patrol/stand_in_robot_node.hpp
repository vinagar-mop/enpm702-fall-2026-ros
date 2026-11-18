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
        StandInRobotNode() 
        : rclcpp::Node("stand_in_robot_node"), 
          odometry_publisher_{create_publisher<nav_msgs::msg::Odometry>("odometry/filtered", 10)},
          odometry_timer_{create_wall_timer(std::chrono::milliseconds(50), std::bind(&StandInRobotNode::publish_odometry, this))},
          scan_publisher_{create_publisher<sensor_msgs::msg::LaserScan>("scan", 10)},
          scan_timer_{create_wall_timer(std::chrono::milliseconds(200), std::bind(&StandInRobotNode::publish_scan, this))},
          cmd_vel_subscriber_{create_subscription<geometry_msgs::msg::Twist>("cmd_vel", 10, std::bind(&StandInRobotNode::update_position, this, std::placeholders::_1))}
        {}

    private:
        void publish_odometry();
        void publish_scan();
        void update_position(const geometry_msgs::msg::Twist &msg);
        rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odometry_publisher_;
        rclcpp::TimerBase::SharedPtr odometry_timer_;
        rclcpp::Publisher<sensor_msgs::msg::LaserScan>::SharedPtr scan_publisher_;
        rclcpp::TimerBase::SharedPtr scan_timer_;
        rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_subscriber_;
        double x_{0.0};                     // robot x-position in meters
        double y_{0.0};                     // robot y-position in meters
        double heading_{0.0};               // robot yaw in radians
        bool near_checkpoint_{false};       // true if robot is within distance of checkpoint, false otherwise
        rclcpp::Time last_vel_time_{now()}; // time of last velocity callback, in seconds
};

} // namespace gp1_patrol