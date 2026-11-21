#include "gp1_patrol/navigation_node.hpp"
#include "gp1_patrol/patrol_log_node.hpp"

#include <memory>
#include <rclcpp/rclcpp.hpp>

int main(int argc, char **argv) {

    rclcpp::init(argc, argv);

    // Create navigation and patrol log nodes
    auto navigation{std::make_shared<gp1_patrol::NavigationNode>()};
    auto patrol_log{std::make_shared<gp1_patrol::PatrolLogNode>()};

    // Multi-threaded executor using two threads
    rclcpp::executors::MultiThreadedExecutor executor{
        rclcpp::ExecutorOptions{}, 2};

    // Add both nodes to the executor
    executor.add_node(navigation);
    executor.add_node(patrol_log);

    // Begin processing callbacks
    executor.spin();

    if (rclcpp::ok()) {
        rclcpp::shutdown();
    }

    return 0;
}