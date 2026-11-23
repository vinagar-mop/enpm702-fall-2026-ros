# A message reporting the results of an inspection of a checkpoint

# Constants representing the inspection result
uint8 RESULT_CLEAR = 0
uint8 RESULT_BLOCKED = 1
uint8 RESULT_DOOR_OPEN = 2
uint8 RESULT_DOOR_CLOSED = 3

# When the status was taken, and the frame the position is in.
std_msgs/Header header

uint32 checkpoint_id # the id of the checkpoint the robot inspects
uint8 result         # the result of the robot's inspection
float32 range_m      # the range in meters that the decision used