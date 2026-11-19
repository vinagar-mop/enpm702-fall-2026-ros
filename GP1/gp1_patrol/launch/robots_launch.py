from launch import LaunchDescription
from launch.actions import GroupAction, DeclareLaunchArgument, IfCondition, LaunchConfiguration
from launch_ros.actions import Node, PushROSNamespace

def one_robot (namespace, option):
    return GroupAction (
        [
            PushROSNamespace(namespace),
            Node(package="gp1_patrol", executable="patrol_exe", condition=option),
            Node(package="gp1_patrol", executable="stand_in_robot_exe",  condition=option),
            Node(package="gp1_patrol", executable="mission_exe",  condition=option),
        ]
    )

def generate_launch_description():
    ld = LaunchDescription();
    two_robots = DeclareLaunchArgument(
        "two_robots",
        default_value="false",
        description="Launching two robots",
        condition=IfCondition(LaunchConfiguration("with_dispatcher"))
    ) 
    
    ld.add_action(two_robots)
    ld.add_action(one_robot("robot_1"))
    ld.add_action(one_robot("robot_2"),two_robots=True)
    return ld
