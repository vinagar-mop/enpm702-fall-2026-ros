/**
 * @file operator_node.hpp
 * @author Vinay Agarwal (vinagar2@umd.edu)
 * @brief  This keeps track of all the opperations that are happening within the system.
 * @version 0.1
 * @date 2026-11-19
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#pragma once

#include <rclcpp/rclcpp.hpp>
#include <chrono>
#include <functional>
#include <std_msgs/msg/float64.hpp>


#include "gp1_interfaces/msg/stop_command.hpp"

namespace gp1_patrol {

/**
 * @brief This node will run the commanded at each checkpoints based off of the timed commands
 * 
 */
class OperatorNode : public rclcpp::Node {
    public:
        OperatorNode()
            : rclcpp::Node{"operator_node"},
            publish_operation_(create_publisher<gp1_interfaces::msg::StopCommand>("/emergency_stop", 10)),
            stop_timer_{create_wall_timer(std::chrono::seconds{13},
                                            std::bind(&OperatorNode::stop_timer_callback_, this))},
            resume_timer_{nullptr}
            {}
    private:
        rclcpp::Publisher<gp1_interfaces::msg::StopCommand>::SharedPtr publish_operation_;
        rclcpp::TimerBase::SharedPtr stop_timer_;
        rclcpp::TimerBase::SharedPtr resume_timer_;

        /**
         * @brief After a set time, the callback should be finished allow to pubish stop_operation.
         * This shall give enough time to discontinue the opperation
         * 
         * @param msg hold a timer referance used to log a message.
         */
        void stop_timer_callback_();

        /**
         * @brief Once the stop_timer_callback is called another timer will start after from this funtion
         * This is a 4 second timer that would give the system enough time to finish its shutdown and start again.
         * 
         * @param msg hold a timer referance used to log a message.
         */
        void resume_timer_callback_();
    };

} // within the gp1_patrol