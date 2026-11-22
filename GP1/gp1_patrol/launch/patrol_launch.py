from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, LogInfo, DeclareLaunchArgument
from launch.substitutions import PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration

def  generate_launch_description():
    ld = LaunchDescription()
    demo_share = FindPackageShare("gp1_patrol")
    robot = PathJoinSubstitution(
        [demo_share, "launch", "robots_launch.py"]
    )
    with_operator = DeclareLaunchArgument(
        "with_operator",
        default_value="true",
        description="Start the operator node"
    )

    operator = Node(
        package="gp1_patrol",
        executable="operator_exe",
        condition=IfCondition(LaunchConfiguration("with_operator"))
    ) 

    talker_listener = IncludeLaunchDescription(robot)

    log = LogInfo(msg=["two_robots = ", LaunchConfiguration("two_robots"), 
                        ", with_operator = ", LaunchConfiguration("with_operator")])

    ld.add_action(with_operator)
    ld.add_action(talker_listener)
    ld.add_action(log)
    ld.add_action(operator)
    
    return ld