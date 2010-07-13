#!/usr/bin/python

# the optimized version for sa.py, using the heapq priority queue
# ./saop.py < input

import random
import math
import heapq

rand = random.Random()

def dist((a,b), (c,d)):
    return abs(a-c) + abs(b-d)

def cost(bacons, mindists):
    ret = 0
    good = set(bacons)
    for x in mindists:
        while True:
            a, b = x[0]
            if b not in good:
                heapq.heappop(x)
            else:
                ret += a
                break
    return ret

def choose(bacons, candidates):
    k = rand.randint(0, len(bacons)-1)
    m = rand.randint(0, len(candidates)-1)
    return (k, m)

def run(houses, candidates, b):
    # inits
    temperate = 10**3
    rand.shuffle(candidates)
    bacons = candidates[:b]
    candidates = candidates[b:]
    mindists = []
    for h in houses:
        s = [(dist(h, x), x) for x in bacons]
        heapq.heapify(s)
        mindists.append(s)
    c = cost(bacons, mindists)
    print c
    bestc, bestb = c, bacons[:]

    # SA iteration
    while temperate > 1e-300:
        k, m = choose(bacons, candidates)
        bacons[k], candidates[m] = candidates[m], bacons[k]
        for i, x in enumerate(mindists):
            heapq.heappush(x, (dist(houses[i], bacons[k]), bacons[k]))
        nc = cost(bacons, mindists)
        if nc < c or rand.uniform(0,1) < math.exp((c - nc) / temperate):
            if nc < bestc:
                print nc
                bestc = nc
                bestb = bacons[:]
            if nc > c:
                print math.exp((c-nc)/temperate)
            c = nc
        else: # swap back
            bacons[k], candidates[m] = candidates[m], bacons[k]
            for i, x in enumerate(mindists):
                heapq.heappush(x, (dist(houses[i], bacons[k]), bacons[k]))
        temperate *= 0.9

    return (bestc, bestb)

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
