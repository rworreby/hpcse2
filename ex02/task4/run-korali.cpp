#include "_model/heat2d.h"
#include "korali.hpp"

/*
HPCSE II, 2021
Exercise 2 - task 4
*/

// TODO Create a new Korali experiment (see below for the variable name used for the experiment)

// Load the measurement data (reference points) which can then be used by Korali.
// auto p = heat2DInit(&argc, &argv);

// TODO Define the problem (Hint: Type, Likelihood Model, Reference Data, Computational Model)

// TODO Choose the solver

// TODO Define the variables which determine how our model at hand
// Also, don't forget to define their prior distributions and support

// TODO: Define the Korali engine and run the experiment

int main(int argc, char* argv[])
{

    bool tmcmc = false;  // for model selection
    bool cmaes = true; // for optimization (finding x,y)

    if (argc<2) {
        printf("\nusage: %s Np \n", argv[0]);
        printf("        Np = 1,2 or 3 \n");
        exit(1);
    }
    else if (tmcmc == cmaes){
        printf("\n Choose exactly one solver!\n");
        exit(1);
    }

    size_t n_candles = atoi(argv[1]);
    printf("Running with %d candle(s)\n", n_candles);

    auto k = korali::Engine();
    auto e = korali::Experiment();
    auto p = heat2DInit(&argc, &argv);

    e["Problem"]["Type"] = "Bayesian/Reference";
    e["Problem"]["Likelihood Model"] = "Normal";
    e["Problem"]["Reference Data"] = p.refTemp;
    e["Problem"]["Computational Model"] = &heat2DSolver;

    if (tmcmc) {
        e["Solver"]["Type"] = "Sampler/TMCMC";
        e["Solver"]["Population Size"] = 500;
    }

    if (cmaes) {
        e["Solver"]["Type"] = "Optimizer/CMAES";
        e["Solver"]["Termination Criteria"]["Max Generations"] = 30;
        e["Solver"]["Termination Criteria"]["Min Value Difference Threshold"] = 1e-9;

        // For CMAES optimization
        int lambda = 4 + floor(3 * log(2 * n_candles + 1));
        int mu = lambda / 2;

        printf("\nrunning cmaes with lambda %d and mu %d\n\n", lambda, mu);
        e["Solver"]["Population Size"] = lambda;
        e["Solver"]["Mu Value"] = mu;
    }

    switch (n_candles) {
        case 1:
            e["Distributions"][0]["Name"] = "Uniform 0";
            e["Distributions"][0]["Type"] = "Univariate/Uniform";
            e["Distributions"][0]["Minimum"] = 0.0;
            e["Distributions"][0]["Maximum"] = 1.0;

            e["Distributions"][1]["Name"] = "Uniform 1";
            e["Distributions"][1]["Type"] = "Univariate/Uniform";
            e["Distributions"][1]["Minimum"] = 0.0;
            e["Distributions"][1]["Maximum"] = 1.0;

            e["Distributions"][2]["Name"] = "Uniform 2";
            e["Distributions"][2]["Type"] = "Univariate/Uniform";
            e["Distributions"][2]["Minimum"] = 0.0;
            e["Distributions"][2]["Maximum"] = 20.0;

            e["Variables"][0]["Name"] = "PosX";
            e["Variables"][0]["Prior Distribution"] = "Uniform 0";
            e["Variables"][0]["Lower Bound"] = 0.0;
            e["Variables"][0]["Upper Bound"] = 1.0;

            e["Variables"][1]["Name"] = "PosY";
            e["Variables"][1]["Prior Distribution"] = "Uniform 1";
            e["Variables"][1]["Lower Bound"] = 0.0;
            e["Variables"][1]["Upper Bound"] = 1.0;

            e["Variables"][2]["Name"] = "[Sigma]";
            e["Variables"][2]["Prior Distribution"] = "Uniform 2";
            e["Variables"][2]["Lower Bound"] = 0.0;
            e["Variables"][2]["Upper Bound"] = 20.0;
            break;

        case 2:
            e["Distributions"][0]["Name"] = "Uniform 0";
            e["Distributions"][0]["Type"] = "Univariate/Uniform";
            e["Distributions"][0]["Minimum"] = 0.0;
            e["Distributions"][0]["Maximum"] = 0.5;

            e["Distributions"][1]["Name"] = "Uniform 1";
            e["Distributions"][1]["Type"] = "Univariate/Uniform";
            e["Distributions"][1]["Minimum"] = 0.0;
            e["Distributions"][1]["Maximum"] = 1.0;

            e["Distributions"][2]["Name"] = "Uniform 2";
            e["Distributions"][2]["Type"] = "Univariate/Uniform";
            e["Distributions"][2]["Minimum"] = 0.5;
            e["Distributions"][2]["Maximum"] = 1.0;

            e["Distributions"][3]["Name"] = "Uniform 3";
            e["Distributions"][3]["Type"] = "Univariate/Uniform";
            e["Distributions"][3]["Minimum"] = 0.0;
            e["Distributions"][3]["Maximum"] = 1.0;

            e["Distributions"][4]["Name"] = "Uniform 4";
            e["Distributions"][4]["Type"] = "Univariate/Uniform";
            e["Distributions"][4]["Minimum"] = 0.0;
            e["Distributions"][4]["Maximum"] = 20.0;

            e["Variables"][0]["Name"] = "PosX0";
            e["Variables"][0]["Prior Distribution"] = "Uniform 0";
            e["Variables"][0]["Lower Bound"] = 0.0;
            e["Variables"][0]["Upper Bound"] = 0.5;

            e["Variables"][1]["Name"] = "PosY0";
            e["Variables"][1]["Prior Distribution"] = "Uniform 1";
            e["Variables"][1]["Lower Bound"] = 0.0;
            e["Variables"][1]["Upper Bound"] = 1.0;

            e["Variables"][2]["Name"] = "PosX1";
            e["Variables"][2]["Prior Distribution"] = "Uniform 2";
            e["Variables"][2]["Lower Bound"] = 0.5;
            e["Variables"][2]["Upper Bound"] = 1.0;

            e["Variables"][3]["Name"] = "PosY1";
            e["Variables"][3]["Prior Distribution"] = "Uniform 3";
            e["Variables"][3]["Lower Bound"] = 0.0;
            e["Variables"][3]["Upper Bound"] = 1.0;

            e["Variables"][4]["Name"] = "[Sigma]";
            e["Variables"][4]["Prior Distribution"] = "Uniform 4";
            e["Variables"][4]["Lower Bound"] = 0.0;
            e["Variables"][4]["Upper Bound"] = 20.0;
            break;

        case 3:
            e["Distributions"][0]["Name"] = "Uniform 0";
            e["Distributions"][0]["Type"] = "Univariate/Uniform";
            e["Distributions"][0]["Minimum"] = 0.0;
            e["Distributions"][0]["Maximum"] = 0.5;

            e["Distributions"][1]["Name"] = "Uniform 1";
            e["Distributions"][1]["Type"] = "Univariate/Uniform";
            e["Distributions"][1]["Minimum"] = 0.0;
            e["Distributions"][1]["Maximum"] = 1.0;

            e["Distributions"][2]["Name"] = "Uniform 2";
            e["Distributions"][2]["Type"] = "Univariate/Uniform";
            e["Distributions"][2]["Minimum"] = 0.5;
            e["Distributions"][2]["Maximum"] = 1.0;

            e["Distributions"][3]["Name"] = "Uniform 3";
            e["Distributions"][3]["Type"] = "Univariate/Uniform";
            e["Distributions"][3]["Minimum"] = 0.0;
            e["Distributions"][3]["Maximum"] = 1.0;

            e["Distributions"][4]["Name"] = "Uniform 4";
            e["Distributions"][4]["Type"] = "Univariate/Uniform";
            e["Distributions"][4]["Minimum"] = 0.5;
            e["Distributions"][4]["Maximum"] = 1.0;

            e["Distributions"][5]["Name"] = "Uniform 5";
            e["Distributions"][5]["Type"] = "Univariate/Uniform";
            e["Distributions"][5]["Minimum"] = 0.0;
            e["Distributions"][5]["Maximum"] = 1.0;

            e["Distributions"][6]["Name"] = "Uniform 6";
            e["Distributions"][6]["Type"] = "Univariate/Uniform";
            e["Distributions"][6]["Minimum"] = 0.0;
            e["Distributions"][6]["Maximum"] = 20.0;

            e["Variables"][0]["Name"] = "PosX0";
            e["Variables"][0]["Prior Distribution"] = "Uniform 0";
            e["Variables"][0]["Lower Bound"] = 0.0;
            e["Variables"][0]["Upper Bound"] = 0.5;

            e["Variables"][1]["Name"] = "PosY0";
            e["Variables"][1]["Prior Distribution"] = "Uniform 1";
            e["Variables"][1]["Lower Bound"] = 0.0;
            e["Variables"][1]["Upper Bound"] = 1.0;

            e["Variables"][2]["Name"] = "PosX1";
            e["Variables"][2]["Prior Distribution"] = "Uniform 2";
            e["Variables"][2]["Lower Bound"] = 0.5;
            e["Variables"][2]["Upper Bound"] = 1.0;

            e["Variables"][3]["Name"] = "PosY1";
            e["Variables"][3]["Prior Distribution"] = "Uniform 3";
            e["Variables"][3]["Lower Bound"] = 0.0;
            e["Variables"][3]["Upper Bound"] = 1.0;

            e["Variables"][4]["Name"] = "PosX2";
            e["Variables"][4]["Prior Distribution"] = "Uniform 4";
            e["Variables"][4]["Lower Bound"] = 0.5;
            e["Variables"][4]["Upper Bound"] = 1.0;

            e["Variables"][5]["Name"] = "PosY2";
            e["Variables"][5]["Prior Distribution"] = "Uniform 5";
            e["Variables"][5]["Lower Bound"] = 0.0;
            e["Variables"][5]["Upper Bound"] = 1.0;

            e["Variables"][6]["Name"] = "[Sigma]";
            e["Variables"][6]["Prior Distribution"] = "Uniform 6";
            e["Variables"][6]["Lower Bound"] = 0.0;
            e["Variables"][6]["Upper Bound"] = 20.0;
            break;

        default:
            printf("\nusage: %s {1,2,3} \n\n", argv[0]);
            exit(1);
    }

    // auto k = korali::Engine();
    k.run(e);

    return 0;
}
