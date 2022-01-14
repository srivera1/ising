#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int height;
  int width;
  int T;
  int Nsteps;
  int time;
  float JCteMax;
  float JCteMin;
  float muMax;
  float muMin;
  float temperature;
  float cooling;
  float warming;
  float spinNearestNeighbors;
  float spinMagnetic;
  float spinTime;
} params_s;

static params_s params;

void init(void);
int lauchSimulation(void);
int getParams(int argc, char **argv);
void matrixAllocation(void);
