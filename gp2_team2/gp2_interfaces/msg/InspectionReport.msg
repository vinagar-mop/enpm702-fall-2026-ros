# A message reporting the results of an inspection of a checkpoint

# Constants representing the inspection result
uint8 RESULT_CLEAR = 0
uint8 RESULT_BLOCKED = 1
uint8 RESULT_DOOR_OPEN = 2
uint8 RESULT_DOOR_CLOSED = 3
uint8 RESULT_NOT_REACHED = 4

std_msgs/Header header   # When the report was made

uint32 checkpoint_id     # The id of the checkpoint the robot inspects
uint8 result             # The result of the robot's inspection
float32 range_m          # The range in meters that the decision used
DetectedObject[] objects # The objects seen by the camera, may be empty