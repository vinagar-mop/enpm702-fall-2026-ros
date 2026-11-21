/**
 * @file patrol_log_node.cpp
 * @author Jacob Collier-Tenison (jacobct@umd.edu)
 * @brief Definitions of functions from patrol_log_node.hpp
 * 
 */
#include "gp1_patrol/patrol_log_node.hpp"

#include <cmath>
#include <chrono>
#include <thread>

namespace gp1_patrol {


void PatrolLogNode::update_counts(const gp1_interfaces::msg::InspectionReport &msg) {
    if (msg.result == gp1_interfaces::msg::InspectionReport::RESULT_CLEAR) {
        clear_count_++;
    } else if (msg.result == gp1_interfaces::msg::InspectionReport::RESULT_BLOCKED) {
        blocked_count_++;
    } else if (msg.result == gp1_interfaces::msg::InspectionReport::RESULT_DOOR_OPEN) {
        door_open_count_++;
    } else if (msg.result == gp1_interfaces::msg::InspectionReport::RESULT_DOOR_CLOSED) {
        door_closed_count_++;
    }
}
void PatrolLogNode::log() {

    // Simulate the slow logging for R5 (1.5 seconds)
    std::this_thread::sleep_for(std::chrono::milliseconds{1500});

    RCLCPP_INFO_STREAM(get_logger(), "patrol log: clear  " << clear_count_ << ", blocked " << blocked_count_ << ", door open " << door_open_count_ << ", door closed " << door_closed_count_);
}

} // namespace gp1_patrol
