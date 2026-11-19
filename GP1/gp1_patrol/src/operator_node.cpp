#include <operator/operator_node.hpp>

using namespace gp1_patrol; 

void OperatorNode::stop_timer_callback_() {
    gp1_interfaces::msg::StopCommand stop_msg{};
    stop_msg.command = gp1_interfaces::msg::StopCommand::STOP;
    stop_msg.header.stamp = this->now();
    stop_msg.reason  = "operator stop";
    publish_operation_->publish(stop_msg);

    // Loging that stop timer is published
    RCLCPP_INFO_STREAM(get_logger(),"sent STOP (operator stop)");

    // Calling resume timer callback 
    this->resume_timer_ = create_wall_timer(std::chrono::seconds{5},
                         std::bind(&OperatorNode::resume_timer_callback_, this));
    this->stop_timer_->cancel();
}

void OperatorNode::resume_timer_callback_() {
    gp1_interfaces::msg::StopCommand resume_msg{};
    resume_msg.command = gp1_interfaces::msg::StopCommand::RESUME;
    resume_msg.header.stamp = this->now();
    resume_msg.reason  = "operator resume";
    publish_operation_->publish(resume_msg);

    // Loging that stop timer is published
    RCLCPP_INFO_STREAM(get_logger(),"sent RESUME (operator resume)");
    
    this->resume_timer_->cancel();
}


