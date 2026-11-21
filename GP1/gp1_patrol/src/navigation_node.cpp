
#include "gp1_patrol/navigation_node.hpp"
#include "gp1_patrol/checkpoints.hpp"
#include "gp1_patrol/geometry.hpp"

#include <cmath>
#include <cstdint>




namespace gp1_patrol {

void NavigationNode::odometry_callback(const nav_msgs::msg::Odometry::SharedPtr msg){

    current_x_ = msg->pose.pose.position.x;
    current_y_ = msg->pose.pose.position.y;
    current_yaw_ = yaw_from_quaternion(msg->pose.pose.orientation);
    has_odometry_ =  true;
}

void NavigationNode::inspection_report_callback(const gp1_interfaces::msg::InspectionReport::SharedPtr msg){

    // Ignore report if robot is not waiting for inspection
    if (!waiting_for_report_){
        return;
    }

    // Ignore report if checkpoint is not for the current checkpoint
    if (msg->checkpoint_id != checkpoint_index_ + 1){
        return;
    }

    RCLCPP_INFO(get_logger(), "Inspection complete for checkpoint %u. Advancing.", msg->checkpoint_id);

    ++checkpoint_index_;
    waiting_for_report_ = false;
    if (checkpoint_index_ >= kCheckpoints.size()) {
        RCLCPP_INFO(get_logger(), "patrol complete");
    }
}

void NavigationNode::stop_command_callback(const gp1_interfaces::msg::StopCommand::SharedPtr msg){

    const rclcpp::Time sent_time{msg->header.stamp};
    const rclcpp::Time received_time{now()};
    const double delay_ms{(received_time - sent_time).seconds() * 1000.0};

    if (msg->command == gp1_interfaces::msg::StopCommand::STOP) {
        emergency_stopped_ = true;
        publish_stop();

        RCLCPP_INFO(get_logger(), "STOP (%s) received %.1f ms after it was sent", msg->reason.c_str(), delay_ms);

    } else if (msg->command == gp1_interfaces::msg::StopCommand::RESUME){
        emergency_stopped_ = false;

        RCLCPP_INFO(get_logger(), "RESUME (%s) received %.1f ms after it was sent", msg->reason.c_str(), delay_ms);
    }

}

void NavigationNode::navigation_callback(){

    if (!has_odometry_ || emergency_stopped_ || waiting_for_report_) {
        publish_stop();
        return;
    }

    if (checkpoint_index_ >= kCheckpoints.size()) {
        publish_stop();
        return;
    }

    const auto& checkpoint{kCheckpoints[checkpoint_index_]};

    const double distance{distance_between(current_x_, current_y_, checkpoint.x, checkpoint.y)};

    if (distance < 0.15) {

        publish_stop();

        gp1_interfaces::msg::Arrival arrival{};

        arrival.header.stamp = now();
        arrival.header.frame_id = "odom";

        arrival.checkpoint_id = static_cast<std::uint32_t>(checkpoint_index_ + 1);

        arrival.kind = checkpoint.kind;

        waiting_for_report_ = true;

        arrival_publisher_->publish(arrival);

        RCLCPP_INFO(get_logger(), "arrived at checkpoint %u", arrival.checkpoint_id);

        return;
    }

    const double desired_heading{bearing_between(current_x_, current_y_, checkpoint.x, checkpoint.y)};
    const double heading_error{angle_difference(desired_heading, current_yaw_)};

    geometry_msgs::msg::Twist command{};

    if (std::abs(heading_error) > 0.1) {
        command.angular.z = (heading_error > 0.0) ? 0.5 : -0.5;
    } else {
        command.linear.x = 0.3;
    }

    velocity_publisher_->publish(command);


}

void NavigationNode::publish_stop(){
    geometry_msgs::msg::Twist command{};
    velocity_publisher_->publish(command);
}

}  // namespace gp1_patrol