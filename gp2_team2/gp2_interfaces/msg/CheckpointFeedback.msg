# A message representing the robot's progress towards a checkpoint goal

std_msgs/Header header    # When the feedback was sent

uint32 checkpoint_id      # The checkpoint the robot is going to
float32 distance_left_m   # The distance from the robot to the checkpoint, in meters
float32 heading_error_rad # The difference in the robot's heading and heading facing checkpoint, in radians