#!/usr/bin/env python

# This is a linear regression model with two params (slope and intercept)


def model(s, X):
    a = s["Parameters"][0]
    b = s["Parameters"][1]
    sig = s["Parameters"][2]

    s["Reference Evaluations"] = []
    s["Standard Deviation"] = []
    for x in X:
        s["Reference Evaluations"] += [a * x + b]
        s["Standard Deviation"] += [sig]


def getReferenceData():
    y = []
    y.append(3.21)
    y.append(4.14)
    y.append(4.94)
    y.append(6.06)
    y.append(6.84)
    return y


def getReferencePoints():
    x = []
    x.append(1.0)
    x.append(2.0)
    x.append(3.0)
    x.append(4.0)
    x.append(5.0)
    return x
