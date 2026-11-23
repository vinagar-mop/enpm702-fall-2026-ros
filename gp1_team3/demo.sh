#~bashrc

source install/setup.bash
mkdir -p src/gp1_team3/evidence
timeout -s INT 75 ros2 launch gp1_patrol patrol_launch.py two_robots:=true 2>&1 | tee src/gp1_team3/evidence/demo.log