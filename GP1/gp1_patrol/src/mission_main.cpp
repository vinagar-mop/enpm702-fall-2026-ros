#include <memory>

#include "rclcpp/rclcpp.hpp"

#include "mission/mission_node.hpp"


int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);

    // Initializing the node and keeping it running.
    auto node_mission{std::make_shared<MissionNode>()};
    rclcpp::spin(node_mission);
    if (rclcpp::ok()) {
        rclcpp::shutdown();
    }
}