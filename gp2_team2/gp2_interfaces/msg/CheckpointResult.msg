# A checkpoint representing the robot's outcome in regards to a goal checkpoint

# Constants representing the outcome
uint8 GOAL_REACHED = 0
uint8 OUTCOME_OBSTACLE = 1
uint8 OUTCOME_CANCELLED = 2
uint8 OUTCOME_TIMED_OUT = 3
uint8 OUTCOME_REJECTED = 4

std_msgs/Header header   # When the result was sent

uint32 checkpoint_id     # The id of the checkpoint from the goal
uint8 outcome            # The outcome of the robot's actions in regards to goal checkpoint
float32 final_distance_m # The distance from the robot to the goal checkpoint