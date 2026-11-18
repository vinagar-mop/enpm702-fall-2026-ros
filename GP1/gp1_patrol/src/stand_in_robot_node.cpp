#include "gp1_patrol/stand_in_robot_node.hpp"
#include "gp1_patrol/geometry.hpp"
#include "gp1_patrol/checkpoints.hpp"
#include <cmath>

namespace gp1_patrol {


void StandInRobotNode::publish_odometry() {
    nav_msgs::msg::Odometry msg{};
    msg.header.stamp = now();
    msg.pose.pose.position.x = x_;
    msg.pose.pose.position.y = y_;
    msg.pose.pose.orientation = quaternion_from_yaw(heading_);
    odometry_publisher_->publish(msg);
}
void StandInRobotNode::publish_scan() {
    sensor_msgs::msg::LaserScan msg{};
    msg.header.stamp = now();
    msg.angle_min = -0.1;
    msg.angle_max = 0.1;
    msg.angle_increment = 0.1;
    msg.range_min = 0.1;
    msg.range_max = 10.0;
    msg.ranges = std::vector<float>(3, kFarRangeM);
    bool near_checkpoint{false};
    int i{0};
    for (const Checkpoint& checkpoint : kCheckpoints) {
        i++;
        if (distance_between(x_, y_, checkpoint.x, checkpoint.y) < kNearDistanceM) {
            msg.ranges = std::vector<float>(3, checkpoint.scan_range_m);
            if (!near_checkpoint_) {
                RCLCPP_INFO_STREAM(get_logger(), "stand-in robot near checkpoint " << i);
            }
            near_checkpoint = true;
        }
    }
    near_checkpoint_ = near_checkpoint;
    scan_publisher_->publish(msg);
}

void StandInRobotNode::update_position(const geometry_msgs::msg::Twist &msg) {
    double delta_t{(now()-last_vel_time_).seconds()};
    /// Messages expected at 20 Hz, some variance should be accounted for but robot should
    /// not teleport after long delay or when messages start.
    if (delta_t > 0.1) {
        delta_t = 0.05;
    }
    x_ += msg.linear.x * std::cos(heading_) * delta_t;
    y_ += msg.linear.x * std::sin(heading_) * delta_t;
    heading_ = wrap_angle(heading_+msg.angular.z*delta_t);
    last_vel_time_ = now();
}

} // namespace gp1_patrol
