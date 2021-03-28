#!/usr/bin/env python


def model_3p(s, X):
    D0 = s["Parameters"][0]
    k1 = s["Parameters"][1]
    k2 = s["Parameters"][2]
    k3 = s["Parameters"][3]
    sig = s["Parameters"][4]

    s["Reference Evaluations"] = []
    s["Standard Deviation"] = []
    for x in X:
        s["Reference Evaluations"] += [D0 + k1*x + k2*x*x + k3*x*x*x]
        s["Standard Deviation"] += [sig]


def getReferenceData():
    # Extension of RBC in micro-meters
    y = []
    y.append(7.7985)
    y.append(9.164)
    y.append(9.677)
    y.append(10.019)
    y.append(10.909)
    y.append(11.353)
    y.append(12.343)
    y.append(13.027)
    y.append(13.643)
    y.append(14.123)
    y.append(14.602)
    y.append(14.876)
    y.append(15.286)
    return y


def getReferencePoints():
    # Stretching force in pico-Newton
    x = []
    x.append(0.0)
    x.append(16.502)
    x.append(19.501)
    x.append(31.0285)
    x.append(38.432)
    x.append(47.61)
    x.append(67.858)
    x.append(88.195)
    x.append(108.907)
    x.append(130.47)
    x.append(150.714)
    x.append(172.651)
    x.append(193.091)
    return x
