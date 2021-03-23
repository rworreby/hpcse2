#!/usr/bin/env python3

"""
HPCSE II, 2021
Exercise 2 - task 2

In this exercise we optimize the Rosenbrock function,
using CMA-ES and Korali.
"""

import sys

sys.path.append('./_model')

from model import model
import korali


def main():
    k = korali.Engine()
    e = korali.Experiment()

    e["Problem"]["Type"] = "Optimization"
    e["Problem"]["Objective Function"] = model

    # Defining the problem's variables.
    e["Variables"][0]["Name"] = "x0"
    e["Variables"][0]["Lower Bound"] = -3.0
    e["Variables"][0]["Upper Bound"] = +3.0

    e["Variables"][1]["Name"] = "y0"
    e["Variables"][1]["Lower Bound"] = -3.0
    e["Variables"][1]["Upper Bound"] = +3.0

    # Choose the solver: CMAES
    e["Solver"]["Type"] = "Optimizer/CMAES"

    # Configuring CMA-ES parameters
    e["Solver"]["Population Size"] = 32
    e["Solver"]["Termination Criteria"]["Min Value Difference Threshold"] = 1e-14
    e["Solver"]["Termination Criteria"]["Max Generations"] = 100

    k.run(e)


if __name__ == '__main__':
    main()
