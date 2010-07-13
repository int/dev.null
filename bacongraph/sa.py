#!/usr/bin/python 

# prototyping code for a simple (yet just-works) simulate annealing
# at earch round, just randomly swap a chosen bacon and a candidate
# ./sa.py < input

import random
import math

rand = random.Random()

def dist((a,b), (c,d)):
    return abs(a-c) + abs(b-d)

def nearest((a,b), bacons):
    return min(map(lambda x: dist((a,b), x), bacons))

def cost(houses, bacons):
    return sum(map(lambda x: nearest(x, bacons), houses))

def next(houses, bacons, candidates):
    ret = bacons[:]
    k = rand.randint(0, len(ret)-1)
    ret[k] = rand.choice(candidates)
    return ret

def run(houses, candidates, b):
    temperate = 10**10
    bacons = rand.sample(candidates, b)
    c = cost(houses, bacons)
    print c
    while temperate > 1e-300:
        nb = next(houses, bacons, candidates)
        nc = cost(houses, nb)
        if nc < c or rand.uniform(0,1) < math.exp((c - nc) / temperate):
            if nc < c: print nc
            bacons = nb
            c = nc
        temperate *= 0.9
    return (c, bacons)

wh, b = raw_input().split()
width, height = map(int, wh.split('x'))
b = int(b)
houses, candidates = [], []
for x in xrange(height):
    s = raw_input()
    for y in xrange(width):
        if s[y] == 'P':
            houses.append((x,y))
        else:
            candidates.append((x,y))

print run(houses, candidates, b)
