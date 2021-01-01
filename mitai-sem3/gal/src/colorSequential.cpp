/**
 * @file colorSequential.cpp
 * @brief Sequential radio coloring algorithm source.
 * @author Vladimir Dusek, <xdusek27@stud.fit.vutbr.cz>
 * @date 12-07-2020, last rev. 12-17-2020
 */

#include "colorSequential.hpp"
#include "graphUtils.hpp"
#include "matrix.hpp"
#include "radiocolor.hpp"
#include <iostream>

constexpr int INF = 2 << 28;

ColorSequential::~ColorSequential() {}

graphColors ColorSequential::process(const Matrix &graph, int k) {

    int n = static_cast<int>(graph.cols());

    // Step 1) Get distance matrix using Floyd-Warshall
    auto dist_mat = GraphUtils::computeDistanceMatrix(graph);

    // Step 2) Compute auxiliary matrix C
    auto c = Matrix(n, n, INF);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (k + 1 >= dist_mat.get(i, j)) {
                c.set(i, j, k + 1 - dist_mat.get(i, j));
            } else {
                c.set(i, j, 0);
            }
            c.set(i, j, c.get(i, j));
        }
        c.set(i, i, INF);
    }

    // Step 3) Compute labels
    int r = 0;
    graphColors labels(n, INF);
    labels[0] = 0;

    for (int i = 0; i < n - 1; i++) {
        int min_label = INF;
        int p = INF;

        for (int j = 0; j < n; j++) {
            // There's a bug in the paper on the following line
            if (min_label > c.get(r, j)) {
                min_label = c.get(r, j);
                p = j;
            }
        }

        for (int j = 0; j < n; j++) {
            c.set(p, j, c.get(p, j) + min_label);
        }

        for (int j = 0; j < n; j++) {
            if (c.get(p, j) < c.get(r, j)) {
                c.set(p, j, c.get(r, j));
            }
        }

        labels[p] = min_label;
        r = p;
    }

    return labels;
}
