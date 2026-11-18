#include "gp1_patrol/patrol_log_node.hpp"
#include <cmath>

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
    RCLCPP_INFO_STREAM(get_logger(), "patrol log: clear  " << clear_count_ << ", blocked " << blocked_count_ << ", door open " << door_open_count_ << ", door closed " << door_closed_count_);
}

} // namespace gp1_patrol
