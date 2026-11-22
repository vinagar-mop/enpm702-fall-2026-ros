/**
 * @file patrol_log_node.hpp
 * @author Jacob Collier-Tenison (jacobct@umd.edu)
 * @brief Counts the reports. Every 2 s it writes the patrol log, which takes 1.5 s of work.
 * 
 */
#pragma once
#include <gp1_interfaces/msg/inspection_report.hpp>
#include <functional>
#include <rclcpp/rclcpp.hpp>

namespace gp1_patrol {

class PatrolLogNode : public rclcpp::Node {
    /**
     * @brief Construct a Patrol Log Node.
     * Has a 2s timer to publish patrol logs and a subscription to insepction report to maintain counts.
     */
    public:
        PatrolLogNode() 
        : rclcpp::Node("patrol_log_node"), 
          timer_{create_wall_timer(std::chrono::milliseconds(2000), std::bind(&PatrolLogNode::log, this))},
          report_subscriber_{create_subscription<gp1_interfaces::msg::InspectionReport>("inspection_report", 10, std::bind(&PatrolLogNode::update_counts, this, std::placeholders::_1))}
        {}

    private:
        /**
         * @brief Publishes a patrol log
         * 
         */
        void log();
        /**
         * @brief Uses an insepction report to update event counts
         * 
         * @param msg The inspection report
         */
        void update_counts(const gp1_interfaces::msg::InspectionReport &msg);
        rclcpp::TimerBase::SharedPtr timer_; // A timer uses to call the log() function every 2 seconds
        rclcpp::Subscription<gp1_interfaces::msg::InspectionReport>::SharedPtr report_subscriber_; // A subsriber to inspection_report topic
        int clear_count_{0}; // Count of clear reports. Starts at 0, always increases.
        int blocked_count_{0}; // Count of blocked reports. Starts at 0, always increases.
        int door_open_count_{0}; //Count of door open reports. Starts at 0, always increases.
        int door_closed_count_{0}; // Count of door closed reports. Starts at 0, always decreases.
};

} // namespace gp1_patrol