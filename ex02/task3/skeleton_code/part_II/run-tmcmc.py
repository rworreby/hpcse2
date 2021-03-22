#!/usr/bin/env python3

"""
HPCSE II, 2021
Exercise 2 - task 3

Bayesian inference of the posterior distribution
for a model of RBC extension under stretching,
using TMCMC and Korali.
"""

import sys
sys.path.append('./_model')
from model import *



# TODO: Implement korali's configuration
# by adapting examples/example_3/run-tmcmc.py
# ...



k = korali.Engine()
k.run(e)
