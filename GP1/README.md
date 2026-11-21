# GP1 starter: Patrol and Inspect

This folder is the starter for GP1. The assignment page (Read the Docs,
Assignments, GP1) says what to build. This file says what is already here
and where your team starts.

## What is provided

```text
GP1/
├── README.md                     this file
├── gp1_interfaces/               messages only (CMake)
│   ├── CMakeLists.txt            complete, with TODO marks
│   ├── package.xml               complete, with TODO marks
│   └── msg/.gitkeep              your three message files go in msg/
└── gp1_patrol/                   C++ nodes (rclcpp)
    ├── CMakeLists.txt            complete, with TODO marks
    ├── package.xml               complete, with TODO marks
    ├── launch/.gitkeep           your two launch files go in launch/
    └── include/gp1_patrol/
        ├── checkpoints.hpp       the checkpoint table
        └── geometry.hpp          small math helpers
```

There are no node sources, no `main` files, no messages and no launch
files. Your team writes them.

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

Each function has a Doxygen comment. Use them: do not write this math again.

## Build

From your workspace root (the folder that holds `src/`):

```bash
source /opt/ros/jazzy/setup.bash
colcon build --packages-up-to gp1_patrol
source install/setup.bash
```

As delivered, both packages build and nothing runs: there are no messages
and no executables yet. `gp1_patrol` already depends on `gp1_interfaces` in
`package.xml`, so colcon builds the interfaces first.

## TODOs, in order

Search for `TODO` in the two `CMakeLists.txt` and the two `package.xml`.

1. **Maintainers (both `package.xml`).** Replace the three placeholder
   `<maintainer>` lines with each team member's name and email. Do it on
   day one, so the build never ships the placeholders.
2. **R1: messages (`gp1_interfaces`).** Write `Arrival.msg`,
   `InspectionReport.msg` and `StopCommand.msg` in `msg/`. In
   `gp1_interfaces/CMakeLists.txt`, uncomment the line of each message in
   `GP1_MESSAGE_FILES` as you add its file. Build, then check each one with
   `ros2 interface show gp1_interfaces/msg/Arrival`.
3. **Use the interfaces (`gp1_patrol/CMakeLists.txt`).** Once R1 builds,
   uncomment `find_package(gp1_interfaces REQUIRED)`.
4. **R2: stand-in robot.** Write `stand_in_robot_node` and
   `stand_in_robot_main.cpp`. Uncomment `stand_in_robot_exe`.
5. **R3 and R4: log and navigation.** Write `patrol_log_node` and
   `navigation_node`, each as `.hpp` and `.cpp`.
6. **R5: two executables.** Write `patrol_single_main.cpp` and
   `patrol_main.cpp`. Uncomment `patrol_single_exe` and `patrol_exe`.
7. **R6: mission and operator.** Write `mission_node`, `operator_node` and
   their `main` files. Uncomment `mission_exe` and `operator_exe`.
8. **Install list.** Uncomment the `install(TARGETS ...)` block in
   `gp1_patrol/CMakeLists.txt` and delete the names of executables you have
   not written yet. Without it, `ros2 run` cannot find your executables.
9. **R7: launch files.** Write `robots_launch.py` and `patrol_launch.py` in
   `launch/`. The `install(DIRECTORY launch ...)` line is already active.
10. **R8: documentation and demo.** Write `run_demo.sh` at the workspace
    root, your own `README.md` and `CONTRIBUTIONS.md`, and keep the R5
    evidence logs in `evidence/`.

Build after each step. Each step should build with no warnings.


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