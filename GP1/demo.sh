#~bashrc

timeout -s INT 75 ros2 launch gp1_patrol patrol_launch.py two_robots:=true 2>&1 | tee evidence/demo.log

