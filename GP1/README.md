# GP1: Patrol and Inspect

### checkpoints.hpp

It declares, in the namespace `gp1_patrol`:

* `Checkpoint`: a struct with `x`, `y` (meters), `kind` (an `Arrival::KIND_`
  constant) and `scan_range_m` (the range the stand-in scan returns there).
* `kCheckpoints`: the four checkpoints, in the order the robot visits them.
* `kNearDistanceM`: 0.3 m, how close the robot must be for the stand-in
  scan to return the checkpoint's range.
* `kFarRangeM`: 4.0 m, the range everywhere else.

The header includes `gp1_interfaces/msg/arrival.hpp` for `KIND_ROOM` and
`KIND_DOOR`. It compiles only after your `Arrival.msg` (R1) builds, with
those two constants as `uint8`.

### geometry.hpp

Header-only helpers in the namespace `gp1_patrol`. Angles are in radians.

* `yaw_from_quaternion(orientation)`: the heading of a
  `geometry_msgs::msg::Quaternion`.
* `quaternion_from_yaw(yaw)`: a quaternion with only `z` and `w` set.
* `distance_between(from_x, from_y, to_x, to_y)`: the distance between two points.
* `bearing_between(from_x, from_y, to_x, to_y)`: the direction from the first
  point to the second, counterclockwise from +x.
* `wrap_angle(angle)`: the same direction as an angle in (-pi, pi].
* `angle_difference(target, current)`: how far to turn from `current` to
  `target`, the short way, in (-pi, pi]. Positive means counterclockwise.


## Build

From your workspace root (the folder that holds `src/`):

```bash
source /opt/ros/jazzy/setup.bash
colcon build --packages-up-to gp1_patrol
source install/setup.bash
```

## The Interface Package, Stand In Robot Node, and Patrol Log Node

The interface package 'gp1_interfaces' provides Arrival, InspectionReport, and StopCommand messages.
It should be included by each node that uses it. The Arrival message indicates when a robot arrives to
a checkpoint. It is stamped with the current time and includes a checkpoint type and id. The InspectionReport
message indicates the result of an inspection of a checkpoint. It is stamped and includes a result, checkpoint id,
and range in meters, which represents the range of the laser scan used to classify the result. The StopCommand
indicated an emergency stop or that is is safe to resume after a stop. It is stamped with the current time and
includes a command and reason for the command.

The Stand In Robot Node represents an imaginary robot. The robot moves around based on velocity commands while
publishing its odometry and laser scans determined from a table based on its location. A StandInRobot Node can
be independently run with the stand_in_robot_exe executable.

The Patrol Log node keeps track of the results of each report the robot publishes while on patrol. The counds include
the number of clear and blocked rooms and the number of open and closed doors. The node logs these values. There is
no mechanism to independently run a patrol log.

## The Navigation Node and Executables

## The Mission Node, Operator Node, and Launch Files


## R5 Navigation and Timing Evidence - Member B: Andrew Schug

Three trials were conducted using patrol_single_exe and patrol_exe running the same patrol and navigation nodes.

The following table contains the STOP command reception delays for both the single and multi-threaded executors.
The delays were calculated by the navigation node using the difference between the timestamp of the STOP command
published by the operator node and the time the navigation node processed the command.

| Run | Single-Threaded STOP (ms) | Multi-Threaded STOP (ms) |
|---|---:|---:|
| 1 | 639.8 | 0.6 |
| 2 | 1181.1 | 0.3 |
| 3 | 0.8 | 0.4 |
| Average | 607.2 | 0.43 |

Questions for R5:

1. Why does the STOP wait in patrol_single_exe, and which callback does it wait for?

The single threaded executor can only execute one callback at a time. When the PatrolLogNode::log()
executes its 1.5 second sleep operation, the navigation node can't process the STOP command until
the 1.5 second sleep finishes. This explains why runs 1 and 2 have a 639.8 and 1181.1 ms wait time
until STOP was received. Run 3 receives a STOP command in only 0.8 ms because the STOP command
likely arrived when the logging callback wasn't executing.

2. Why does it not wait in patrol_exe, when you made no callback group? 
(Lecture 11: which group does each node’s callback join?)

The patrol_exe is a multi-threaded executor which is configured with two threads.
ROS 2 automatically assigns each node's callbacks to its own default mutually exclusive callback group. 
Since the navigation node and patrol log node belong to separate callback groups, the multi-threaded
executor can execute callbacks from both nodes simultaneously. The small STOP delays of 0.6, 0.3, and 0.4 
on the multi-threaded demonstrate the multi-threaded executor processing STOP commands without waiting 
for the patrol logging callback to finish.

3. Why do the navigation node’s callbacks stay in one mutually exclusive group? Say what could go wrong in a reentrant group.

The navigation node's callbacks remain in the default mutually exclusive callback group assigned by ROS 2. 
This ensures that only one navigation callback can execute at a time, preventing multiple callbacks from simultaneously 
accessing or modifying shared member variables. For example if the navigation node's callbacks were assigned to the reentrant group,
one thread could see that emergency_stopped_ is false and then start publishing a movement command and a moment later, 
another thread could see that emergency_stopped_ is true and publish the stop commands while the other thread is commanding
the robot to move. That design could create a race condition where multiple threads are rewriting the robot's shared variables at
the same time.

4. Why does the robot fail to arrive in patrol_single_exe?

The patrol logging callback runs every 2 seconds and when it runs, the 1.5 seconds sleep blocks in the single-thread executor.
The navigation callback is supposed to run every 100 ms, but while the logging callback is blocking the executor, the
navigation callback is not executed. This prevents the navigation node from continuously updating the robot's position with
velocity commands from odometry updates. As a result, the robot may not receive movement commands frequently enough to reach the 
first checkpoint before the emergency stop command is sent by the operator node. 

The six experiment logs for the single and multi-threaded executor are provided in the evidence/ directory.