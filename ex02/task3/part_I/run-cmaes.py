#!/usr/bin/env python3

"""
HPCSE II, 2021
Exercise 2 - task 3

Bayesian Optimization for a model of
RBC extension under stretching,
using CMA-ES and Korali.
"""

import sys
sys.path.append('./_model')
from model import *



# TODO: Implement korali's configuration
# by adapting examples/example_2/run-cmaes.py
# ...



k = korali.Engine()
k.run(e)
