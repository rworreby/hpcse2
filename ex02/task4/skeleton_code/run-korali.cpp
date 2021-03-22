#include "_model/heat2d.h"
#include "korali.hpp"

/*
HPCSE II, 2021
Exercise 2 - task 4
*/


int main(int argc, char *argv[])
{
  // TODO Create a new Korali experiment (see below for the variable name used for the experiment)
  // ...


  // Load the measurement data (reference points) which can then be used by Korali.
  auto p = heat2DInit(&argc, &argv);


  // TODO Define the problem (Hint: Type, Likelihood Model, Reference Data, Computational Model)
  // ...


  // TODO Choose the solver
  // ...


  // TODO Define the variables which determine how our model at hand
  // Also, don't forget to define their prior distributions and support
  // ...


  // TODO: Define the Korali engine and run the experiment
  // ...
  //

  return 0;
}
