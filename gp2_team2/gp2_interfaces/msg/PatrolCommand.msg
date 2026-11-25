# A message from the operator to the robot to pause or resume a patrol

# Constants representing the command
uint8 PAUSE = 0
uint8 RESUME = 1

std_msgs/Header header   # When the command was sent

uint8 command            # The command sent by the operator
string reason            # Why the operator sent the command