#!/usr/bin/env python3

from collections import defaultdict
import datetime
import json
import scipy.stats
import sys

import matplotlib.pyplot as plt


if __name__ == '__main__':
    data = defaultdict(list)
    # Merge data from all files.
    for ifile in sys.argv[1:]:
        with open(ifile, 'r') as f:
            cur_data = json.loads(f.read())
            for k, v in cur_data.items():
                data[int(k)].extend(v)

    for d in data.values():
        d.sort()

    print(data)

    # Plot graph.
    x = []
    y = []
    for key in sorted(data.keys()):

        # if key in [5, 6]:
        #     continue

        trim = int(len(data[key]) * 0.2)
        d = data[key][trim:-trim]
        avg = sum(d) / len(d)
        print(str(key) + ' ' + str(avg))
        x.append(key)
        y.append(avg)


    slope, intercept, r_value, p_value, std_err = scipy.stats.linregress(x, y)
    plt.plot(x, y, 'rx')
    plt.xlabel('Počet hodnot')
    plt.ylabel('Čas [μs]')
    # plt.title('Výsledky experimentů')
    plt.savefig(f'experiment_{datetime.datetime.now():%Y-%m-%d_%H-%M}.png')
    plt.savefig(f'experiment_{datetime.datetime.now():%Y-%m-%d_%H-%M}.svg')
    plt.savefig(f'experiment_{datetime.datetime.now():%Y-%m-%d_%H-%M}.pdf')
    # plt.show()
