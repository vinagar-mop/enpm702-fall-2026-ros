#include "mission/mission_node.hpp"

#include <string>


void MissionNode::subscriber_scan_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg) {
    this->last_scan_ = msg;
}


void MissionNode::subscriber_arrival_callback(const gp1_interfaces::msg::Arrival::SharedPtr msg) {
    
    if (this->last_scan_ == nullptr) {
        RCLCPP_WARN_STREAM(get_logger(), "Last scan did not capture anything from laser.");
        return;
    }
    
    float middle_beam{this->last_scan_->ranges[this->last_scan_->ranges.size() / 2]};
    gp1_interfaces::msg::InspectionReport report{};
    std::string logged_kind{};
    std::string logged_result{};
    if (msg->kind == gp1_interfaces::msg::Arrival::KIND_ROOM) {
        if (middle_beam < 1.0) {
            report.result = gp1_interfaces::msg::InspectionReport::RESULT_BLOCKED;
            logged_result = "blocked";
        }
        else {
            report.result = gp1_interfaces::msg::InspectionReport::RESULT_CLEAR;
            logged_result = "clear";
        }
        logged_kind = "room";
    }
    else if (msg->kind == gp1_interfaces::msg::Arrival::KIND_DOOR) {
        if (middle_beam < 1.0) {
            report.result = gp1_interfaces::msg::InspectionReport::RESULT_DOOR_CLOSED;
            logged_result = "door closed";
        }
        else {
            report.result = gp1_interfaces::msg::InspectionReport::RESULT_DOOR_OPEN;
            logged_result = "door open";
        }
        logged_kind = "door";
    }
    else {
        RCLCPP_WARN_STREAM(get_logger(), "arrival kind was incorrect and dropped message.");
        return;
    }

    // Setting up report header for the time stamp
    report.header.stamp = now();
    report.checkpoint_id = msg->checkpoint_id;
    report.range_m = middle_beam;

    publisher_inspect_->publish(report);

    RCLCPP_INFO_STREAM(get_logger(), "checkpoint " << report.checkpoint_id << " (" << logged_kind << "): " << logged_result << ", range " << report.range_m << " m");
}

