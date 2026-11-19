#include <memory>

#include "rclcpp/rclcpp.hpp"

#include "operator/operator_node.hpp"

using namespace gp1_patrol; 

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);

    // Initializing the node and keeping it running.
    auto node_operator{std::make_shared<OperatorNode>()};
    rclcpp::spin(node_operator);
    if (rclcpp::ok()) {
        rclcpp::shutdown();
    }
}