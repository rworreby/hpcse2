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
from model import model_3p, getReferenceData, getReferencePoints

import korali


def main():
    e = korali.Experiment()

    # Setting up the reference likelihood for the Bayesian Problem
    e["Problem"]["Type"] = "Bayesian/Reference"
    e["Problem"]["Likelihood Model"] = "Normal"
    e["Problem"]["Reference Data"] = getReferenceData()
    e["Problem"]["Computational Model"] = lambda sampleData: model(sampleData, getReferencePoints())

    # Configuring TMCMC parameters
    e["Solver"]["Type"] = "Sampler/TMCMC"
    e["Solver"]["Population Size"] = 5000
    e["Solver"]["Target Coefficient Of Variation"] = 0.5
    e["Solver"]["Covariance Scaling"] = 0.04

    # Configuring the problem's random distributions
    e["Distributions"][0]["Name"] = "Uniform 4 12"
    e["Distributions"][0]["Type"] = "Univariate/Uniform"
    e["Distributions"][0]["Minimum"] = +4.0
    e["Distributions"][0]["Maximum"] = +12.0

    e["Distributions"][1]["Name"] = "Uniform 05 05"
    e["Distributions"][1]["Type"] = "Univariate/Uniform"
    e["Distributions"][1]["Minimum"] = -0.5
    e["Distributions"][1]["Maximum"] = +0.5

    e["Distributions"][2]["Name"] = "Uniform 01 01"
    e["Distributions"][2]["Type"] = "Univariate/Uniform"
    e["Distributions"][2]["Minimum"] = -0.1
    e["Distributions"][2]["Maximum"] = +0.1

    e["Distributions"][3]["Name"] = "Uniform 0 5"
    e["Distributions"][3]["Type"] = "Univariate/Uniform"
    e["Distributions"][3]["Minimum"] = +0.0
    e["Distributions"][3]["Maximum"] = +5.0

    # Configuring the problem's variables
    e["Variables"][0]["Name"] = "D0"
    e["Variables"][0]["Prior Distribution"] = "Uniform 4 12"

    e["Variables"][1]["Name"] = "k1"
    e["Variables"][1]["Prior Distribution"] = "Uniform 05 05"

    e["Variables"][2]["Name"] = "k2"
    e["Variables"][2]["Prior Distribution"] = "Uniform 01 01"

    e["Variables"][3]["Name"] = "sig"
    e["Variables"][3]["Prior Distribution"] = "Uniform 0 5"

    e["Store Sample Information"] = True

    k = korali.Engine()
    k.run(e)


if __name__ == '__main__':
    main()
