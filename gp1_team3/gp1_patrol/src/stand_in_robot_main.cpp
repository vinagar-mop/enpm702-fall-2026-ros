/**
 * @file stand_in_robot_main.cpp
 * @author Jacob Collier-Tenison (jacobct@umd.edu)
 * @brief Creates a StandInRobotNode and uses rclcpp to spin
 * 
 */
#include <memory>
#include <rclcpp/rclcpp.hpp>
#include "gp1_patrol/stand_in_robot_node.hpp"

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    auto node{std::make_shared<gp1_patrol::StandInRobotNode>()};
    rclcpp::spin(node);
    if (rclcpp::ok()) {
        rclcpp::shutdown();
    }
}
