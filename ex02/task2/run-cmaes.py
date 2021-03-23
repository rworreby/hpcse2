#!/usr/bin/env python3

"""
HPCSE II, 2021
Exercise 2 - task 2

In this exercise we optimize the Rosenbrock function,
using CMA-ES and Korali.
"""


# TODO: import Korali's engine
# ...


# Import the computational model
# import sys
from model import model

import korali


k = korali.Engine()
e = korali.Experiment()

e["Problem"]["Objective Function"] = model

# TODO: Define the problem:
# - Type of problem: Optimization
# - Objective function: the model function
# ...


# Initialize random seed
e["Random Seed"] = 0xC0FEE


# TODO: Define all variables and their respective lower and upper bounds
# ...


# Choose the solver: CMAES
e["Solver"]["Type"] = "Optimizer/CMAES"


# Configuring CMA-ES parameters
e["Solver"]["Population Size"] = 32
e["Solver"]["Termination Criteria"]["Min Value Difference Threshold"] = 1e-14
e["Solver"]["Termination Criteria"]["Max Generations"] = 100


# TODO: Run Korali
# ...
