#include "../include/ising.h"

int main(int argc, char **argv) {

  // default values
  init();

  // get user input
  getParams(argc, argv);
  matrixAllocation();

  // launch job
  lauchSimulation();

}
