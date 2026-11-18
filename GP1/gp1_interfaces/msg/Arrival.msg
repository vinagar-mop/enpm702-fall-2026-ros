# A message indicating when a robot arrives to a checkpoint

# Constants representing the type of checkpoint
uint8 KIND_ROOM = 0
uint8 KIND_DOOR = 1

# When the status was taken, and the frame the position is in.
std_msgs/Header header

uint32 checkpoint_id # the id of the checkpoint the robot arrives at
uint8 kind           # the kind of the checkpoint the robot arrives at