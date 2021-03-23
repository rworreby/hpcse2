#!/usr/bin/env python


def model(p):
    x = p["Parameters"][0]
    y = p["Parameters"][1]
    p["F(x)"] = (1.0 - x)**2 + 100 * (y - x * x)**2
