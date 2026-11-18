#pragma once
#include <gp1_interfaces/msg/inspection_report.hpp>
#include <functional>
#include <rclcpp/rclcpp.hpp>

namespace gp1_patrol {

class PatrolLogNode : public rclcpp::Node {
    public:
        PatrolLogNode() 
        : rclcpp::Node("patrol_log_node"), 
          timer_{create_wall_timer(std::chrono::milliseconds(2000), std::bind(&PatrolLogNode::log, this))},
          report_subscriber_{create_subscription<gp1_interfaces::msg::InspectionReport>("inspection_report", 10, std::bind(&PatrolLogNode::update_counts, this, std::placeholders::_1))}
        {}

    private:
        void log();
        void update_counts(const gp1_interfaces::msg::InspectionReport &msg);
        rclcpp::TimerBase::SharedPtr timer_;
        rclcpp::Subscription<gp1_interfaces::msg::InspectionReport>::SharedPtr report_subscriber_;
        int clear_count_{0};
        int blocked_count_{0};
        int door_open_count_{0};
        int door_closed_count_{0};
};

} // namespace gp1_patrol