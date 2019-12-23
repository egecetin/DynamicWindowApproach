#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/* https://stackoverflow.com/a/3437484 */
 #define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

/* https://stackoverflow.com/a/3437484 */
 #define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

typedef struct {
  float xtop;
  float yleft;
  float xbottom;
  float yright;
} Rect;

typedef struct {
  float maxSpeed;
  float minSpeed;
  float maxYawrate;
  float maxAccel;
  float maxdYawrate;
  float velocityResolution;
  float yawrateResolution;
  float dt;
  float predictTime;
  float heading;
  float clearance;
  float velocity;
  Rect base;
} Robot;

typedef struct {
  float linearVelocity;
  float angularVelocity;
} Velocity;

typedef struct {
  float x;
  float y;
} Point;

typedef struct {
  Point point;
  float yaw;
} Pose;

typedef struct {
  int nPossibleV;
  float *possibleV;
  int nPossibleW;
  float *possibleW;
} DynamicWindow;

void
createDynamicWindow(Velocity velocity, Robot robot, DynamicWindow **dynamicWindow) {
  float minV = max(robot.minSpeed, velocity.linearVelocity - robot.maxAccel * robot.dt);
  float maxV = min(robot.maxSpeed, velocity.linearVelocity + robot.maxAccel * robot.dt);
  float minW =
    max(-robot.maxYawrate, velocity.angularVelocity - robot.maxdYawrate * robot.dt);
  float maxW =
    max(robot.maxYawrate, velocity.angularVelocity + robot.maxdYawrate * robot.dt);

  int nPossibleV = (maxV - minV) / robot.velocityResolution;
  int nPossibleW = (maxW - minW) / robot.yawrateResolution;
  *dynamicWindow = malloc(sizeof(DynamicWindow) +
			  nPossibleV * sizeof(float) +
			  nPossibleW * sizeof(float) +
			  2 * sizeof(int));

  (*dynamicWindow)->possibleV = malloc(nPossibleV * sizeof(float));
  (*dynamicWindow)->possibleW = malloc(nPossibleW * sizeof(float));
  (*dynamicWindow)->nPossibleV = nPossibleV;
  (*dynamicWindow)->nPossibleW = nPossibleW;

  for(int i=0; i < (maxV - minV) / robot.velocityResolution; i++) {
    (*dynamicWindow)->possibleV[i] = minV + (float)i * robot.velocityResolution;
  }

  for(int i=0; i < (maxW - minW) / robot.yawrateResolution; i++) {
    (*dynamicWindow)->possibleW[i] = minW + (float)i * robot.yawrateResolution;
  }
}

void main() {
}
