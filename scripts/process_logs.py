#!/usr/bin/python3

import sys
import collections

sums = collections.defaultdict(lambda: 0)
lens = collections.defaultdict(lambda: 0)

def mva(n_a, s_lb, s_a, avg_len, clients, z):
    n_centers = n_a + 2
    V = [2] + [1 / n_a] * n_a + [1]
    S = [s_lb] + [s_a] * n_a + [z]
    is_delay = [False] * (n_centers - 1) + [True]
    ns = [0] * n_centers

    for i in range(1, clients + 1):
        r = []

        for j in range(n_centers):
            if is_delay[j]:
                r.append(S[j])
            else:
                r.append(V[j] * S[j] * (1 + ns[j]))

        total_r = sum(r)
        throughput = i / total_r

        throughputs = []
        utilizations = []

        for j in range(n_centers):
            throughputs.append(V[j] * throughput)
            utilizations.append(V[j] * throughput * S[j])
            ns[j] = throughput * r[j]

    print("LB Utilization:", utilizations[0])
    print("AC Utilization:", utilizations[1])
    print("R:", total_r - z)

for f in sys.argv[1:]:
    for l in open(f):
        try:
            a, b = l.split()
            b = int(b)
            sums[a] += b
            lens[a] += 1
        except:
            print("Error:", l)

for k, v in sums.items():
    print("{} {}".format(k, v / lens[k]))
