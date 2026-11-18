# A message indicating the robot should stop immediately or resume after a stop

# Constants representing an emergency stop or safe resume command
uint8 STOP = 0
uint8 RESUME = 1

# When the status was taken, and the frame the position is in.
std_msgs/Header header

uint8 command   # the command for the robot to stop or resume
string reason   # why the operator send the command
