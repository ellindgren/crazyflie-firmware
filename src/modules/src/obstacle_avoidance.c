/**
 * This task is called by the stabilizer module and if there is a obstacle
 * in the direction of the setpoint or the velocity comand the setpoint is
 * corrected before being sent to the controller by the stabilizer module.
 */

#include "config.h"
#include "debug.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "static_mem.h"
#include "task.h"

#include "range.h"
#include "estimator_kalman.h"
#include "log.h"
#include "param.h"
#include "obstacle_avoidance.h"
#include "commander.h"
#include "supervisor.h"

STATIC_MEM_TASK_ALLOC(obstacleAvoidanceTask, OBSTACLE_AVOIDANCE_STACKSIZE);


static bool isInit = false;

//static float front_distance, back_distance, left_distance, right_distance;
static rangeDirection_t front = rangeFront;
static rangeDirection_t back = rangeBack;
static rangeDirection_t left = rangeLeft;
static rangeDirection_t right = rangeRight;

static bool frontObstacle = false;
static bool backObstacle = false;
static bool leftObstacle = false;
static bool rightObstacle = false;

static point_t position;
static setpoint_t setpoint_avoid;

static float distance_threshhold = 500;
static float vel = 0.2;
static float height = 0.8;
static bool keepFlying = false;


void setHoverSetpoint(setpoint_t* setpoint, float vx, float vy, float z, float yawrate)
{
  setpoint->mode.z = modeAbs;
  setpoint->position.z = z;
  setpoint->mode.yaw = modeVelocity;
  setpoint->attitudeRate.yaw = yawrate;
  setpoint->mode.x = modeVelocity;
  setpoint->mode.y = modeVelocity;
  setpoint->velocity.x = vx;
  setpoint->velocity.y = vy;
  setpoint->velocity_body = true;
  commanderSetSetpoint(setpoint, 3);
}


static void obstacleAvoidanceTask(){
	paramVarId_t IDkeepFlying = paramGetVarId("ctrl_app", "fly");

	while(1){
		estimatorKalmanGetEstimatedPos(&position);
		keepFlying = paramGetUint(IDkeepFlying);
		if(position.z > (float) 0.15 && supervisorIsFlying() && keepFlying){
			//Check if obstacle in front of drone
			if (rangeGet(front) < distance_threshhold){
				frontObstacle = true;
				setHoverSetpoint(&setpoint_avoid, -vel, 0, height, 0);
			}else{
				frontObstacle = false;
			}
			//Check if obstacle behind of drone
			if (rangeGet(back) < distance_threshhold){
					backObstacle = true;
					setHoverSetpoint(&setpoint_avoid, vel, 0, height, 0);
				}else{
					backObstacle = false;
				}
			//Check if obstacle left of drone
			if (rangeGet(left) < distance_threshhold){
					leftObstacle = true;
					setHoverSetpoint(&setpoint_avoid, 0, -vel, height, 0);
				}else{
					leftObstacle= false;
				}
			//Check if obstacle right of drone
			if (rangeGet(right) < distance_threshhold){
					rightObstacle = true;
					setHoverSetpoint(&setpoint_avoid, 0, vel, height, 0);
				}else{
					rightObstacle = false;
				}
		}
	}
}
/**
void updateSetpointObstacleAvoidance(setpoint_t* setpoint ){

	if (frontObstacle){//check if obstacle in front
		if (setpoint->mode.x == modeVelocity) {//Check if velocity mode
			if( setpoint->velocity.x > 0){	//if velocity is in direction of obstacle
				setpoint->velocity.x = -0.2; // set velocity in obstacle direction to zero
			}
		}else if( (setpoint->position.x - position.x) > 0 ){
			setpoint->position.x = position.x; //set x setpoint position to current position
		}
	}
	if (backObstacle){									//check if obstacle in back
		if (setpoint->mode.x == modeVelocity) {			//Check if velocity mode
			if( setpoint->velocity.x < 0){ 				//if velocity is in direction of obstacle
				setpoint->velocity.x = 0.2; 				// set velocity in obstacle direction to zero
			}
		}else if( (setpoint->position.x - position.x) < 0 ){ //if desired position is behind drone
			setpoint->position.x = position.x; //set x setpoint position to current position
		}
	}


	if (leftObstacle){//check if obstacle to the right
		if (setpoint->mode.y == modeVelocity) {//Check if velocity mode
			if( setpoint->velocity.y > 0){	//if velocity is in direction of obstacle
				setpoint->velocity.y = -0.2; // set velocity in obstacle direction to zero
			}
		}else if( (setpoint->position.y - position.y) > 0 ){
			setpoint->position.y = position.y; //set x setpoint position to current position
		}
	}
	if (rightObstacle){									//check if obstacle to the left
		if (setpoint->mode.y == modeVelocity) {			//Check if velocity mode
			if( setpoint->velocity.y < 0){ 				//if velocity is in direction of obsticle
				setpoint->velocity.y = 0.2; 				// set velocity in obstacle direction to zero
			}
		}else if( (setpoint->position.y - position.y) < 0 ){ //if desired position is behind drone
			setpoint->position.y = position.y; //set x setpoint position to current position
		}
	}
}
*/
void obstacleAvoidanceTaskInit() {
  //inputQueue = STATIC_MEM_QUEUE_CREATE(inputQueue); //no input queue needed?
  // TODO
  STATIC_MEM_TASK_CREATE(obstacleAvoidanceTask, obstacleAvoidanceTask, OBSTACLE_AVIDANCE_TASK_NAME, NULL, OBSTACLE_AVOIDANCE_PRI);
  isInit = true;
}

bool obstacleavoidanceTaskTest() {
  return isInit;
}

//LOG_GROUP_START(OBST_AVOID)
//LOG_ADD(LOG_INT8 , frontObstacle, 	&frontObstacle)
//LOG_ADD(LOG_INT8 , backObstacle, 	&backObstacle)
//LOG_ADD(LOG_INT8 , leftObstacle, 	&leftObstacle)
//LOG_ADD(LOG_INT8 , rightObstacle, 	&rightObstacle)
//LOG_GROUP_STOP(OBST_AVOID)
