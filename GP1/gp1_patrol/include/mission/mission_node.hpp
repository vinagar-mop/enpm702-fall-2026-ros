/**
 * @file mission_node.hpp
 * @author Vinay Agarwal (vinagar1@umd.edu)
 * @brief   The mission node reads the scans at they arrive in order. When a message arrives that messages
 * gets published as a log. This log will have reports will have lables such as clear, blocked, door open
 * or door closed.
 * @version 0.1
 * @date 2026-11-18
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#pragma once

#include <rclcpp/rclcpp.hpp>

#include <sensor_msgs/msg/laser_scan.hpp>
#include <gp1_interfaces/msg/arrival.hpp>
#include <gp1_interfaces/msg/inspection_report.hpp>

/**
 * @brief Mission class reports any and every message that arrives. This class also keeps the last scan.
 * 
 */
class MissionNode : public rclcpp::Node {
    public:
    /**
     * @brief constructer for the mission set all and every node to there default value
     * 
     */
        MissionNode()
            : rclcpp::Node{"mission_node"},
            subscriber_scan_{this->create_subscription<sensor_msgs::msg::LaserScan>("scan", 10, std::bind(&MissionNode::subscriber_scan_callback, this, std::placeholders::_1))},
            subscriber_arrival_{this->create_subscription<gp1_interfaces::msg::Arrival>("arrival", 10, std::bind(&MissionNode::subscriber_arrival_callback, this, std::placeholders::_1))},
            publisher_inspect_{this->create_publisher<gp1_interfaces::msg::InspectionReport>("inspection_report", 10)}
            {}

    private:
        rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr subscriber_scan_;
        rclcpp::Subscription<gp1_interfaces::msg::Arrival>::SharedPtr subscriber_arrival_;
        rclcpp::Publisher<gp1_interfaces::msg::InspectionReport>::SharedPtr publisher_inspect_;

        void subscriber_scan_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg);
        void subscriber_arrival_callback(const gp1_interfaces::msg::Arrival::SharedPtr msg);

        sensor_msgs::msg::LaserScan::SharedPtr last_scan_; // points to the last scan befor new fram

};