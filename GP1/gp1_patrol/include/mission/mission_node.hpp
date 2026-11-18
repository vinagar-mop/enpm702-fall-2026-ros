/**
 * @file mission_node.h
 * @author Vinay Agarwal (vinagar1@umd.edu)
 * @brief   The mission node reads the scans at they arrive in order. When a message arrives that messages
 * gets published as a log. This log will have reports will have lables such as clear, blocked, door open
 * or door closed.
 * @version 0.1
 * @date 2026-11-18
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#pragma once

#include <rclcpp/rclcpp.hpp>

#include <mission/mission_node.hpp>


/**
 * @brief Mission class reports any and every message that arrives. This class also keeps the last scan.
 * 
 */
class MissionNode : public rclcpp::Node {
    public:
    /**
     * @brief constructer for the mission set all and every node to there default value
     * 
     */
        MissionNode()
            : rclcpp::node{"mission"},
            subscription_{create_subscription<scan>}
            

    private:

}

