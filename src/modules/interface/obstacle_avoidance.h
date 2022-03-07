/*
 * obsticle_avoidance.h
 *
 *  Created on: Sep 17, 2021
 *      Author: matvik
 */

#ifndef SRC_MODULES_INTERFACE_OBSTACLE_AVOIDANCE_H_
#define SRC_MODULES_INTERFACE_OBSTACLE_AVOIDANCE_H_

#include <stdbool.h>


void obstacleAvoidanceTaskInit();
bool obstacleavoidanceTaskTest();
void updateSetpointObstacleAvoidance(setpoint_t* setpoint);
void setHoverSetpoint(setpoint_t *setpoint, float vx, float vy, float z, float yawrate);




#endif /* SRC_MODULES_INTERFACE_OBSTACLE_AVOIDANCE_H_ */
