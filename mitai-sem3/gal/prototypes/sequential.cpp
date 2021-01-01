// VUT FIT MITAI
// GAL 2020/2021
// Project: Topic 38 - Graph Radio Coloring Parallelization
// Authors: Vladimir Dusek, Patrik Goldschmidt
//
// Module description:
//     C++ prototype of sequential coloring

#include <iostream>
#include <limits>
#include <string>
#include <vector>

const int INF = 1000000;

void print(std::vector<int> v) {
    for (std::vector<int>::const_iterator i = v.begin(); i != v.end(); i++) {
        std::cout << std::to_string(*i) << ' ';
    }
    std::cout << std::endl;
}

void print(std::vector<std::vector<int>> v) {
    for (std::vector<std::vector<int>>::const_iterator i = v.begin(); i != v.end(); i++) {
        for (std::vector<int>::const_iterator j = (*i).begin(); j != (*i).end(); j++) {
            std::cout << std::to_string(*j) << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
}

/**
 * @brief Using Floyed-Warshall's algorithm to compute the distance matrix of graph.
 *
 * @param adj_mat: the adjacency matrix of graph
 * @return: the distance matrix of graph
 */
std::vector<std::vector<int>> get_dist_mat(std::vector<std::vector<int>> adj_mat) {
    // Get number of vertices
    int n = adj_mat.size();

    // Initiate distance matrix
    auto dist_mat = std::vector<std::vector<int>>{};

    for (int i = 0; i < n; i++) {
        auto inner = std::vector<int>{};
        for (int j = 0; j < n; j++) {
            if (adj_mat[i][j] == 0) {
                inner.push_back(INF);
            } else {
                inner.push_back(adj_mat[i][j]);
            }
        }
        dist_mat.push_back(inner);
        inner.clear();
    }

    // Compute distance matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                dist_mat[j][k] = std::min(dist_mat[j][k], dist_mat[j][i] + dist_mat[i][k]);
            }
        }
        dist_mat[i][i] = 0;
    }

    return dist_mat;
}

/**
 * @brief Algorithm for k-radio coloring of unoriented graph.
 *
 * @param adj_mat: the adjacency matrix of graph
 * @param k: positive integer k
 * @return: labels
 */
std::vector<int> radio_k_coloring(std::vector<std::vector<int>> adj_mat, int k) {
    // Get number of vertices
    int n = adj_mat.size();

    // Get distance matrix
    auto dist_mat = get_dist_mat(adj_mat);

    // Init part 1
    int r = 0;
    auto c = std::vector<std::vector<int>>{};

    for (int i = 0; i < n; i++) {
        auto inner = std::vector<int>{};
        for (int j = 0; j < n; j++) {
            inner.push_back(INF);
        }
        c.push_back(inner);
        inner.clear();
    }

    // Part 1
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (k + 1 >= dist_mat[i][j]) {
                c[i][j] = k + 1 - dist_mat[i][j];
            } else {
                c[i][j] = 0;
            }
            c[j][i] = c[i][j];
        }
        c[i][i] = INF;
    }

    // Init part 2
    int l = r;
    auto result = std::vector<int>{0};
    for (int i = 0; i < n - 1; i++) {
        result.push_back(INF);
    }

    // Part 2
    for (int i = 0; i < n - 1; i++) {
        int min_label = INF;
        int p = INF;

        for (int j = 0; j < n; j++) {
            // There's a bug in the paper on the following line
            if (min_label > c[l][j]) {
                min_label = c[l][j];
                p = j;
            }
        }

        for (int j = 0; j < n; j++) {
            c[p][j] = c[p][j] + min_label;
        }

        for (int j = 0; j < n; j++) {
            if (c[p][j] < c[l][j]) {
                c[p][j] = c[l][j];
            }
        }

        result[p] = min_label;
        l = p;
    }

    return result;
}

int main() {
    int k = 4;
    auto adj_mat = std::vector<std::vector<int>>{
        std::vector<int>{0, 1, 0, 0},
        std::vector<int>{1, 0, 1, 1},
        std::vector<int>{0, 1, 0, 0},
        std::vector<int>{0, 1, 0, 0}
    };

    print(radio_k_coloring(adj_mat, k));

    k = 4;
    adj_mat = std::vector<std::vector<int>>{
        std::vector<int>{0, 1, 0, 1, 0, 0},
        std::vector<int>{1, 0, 0, 1, 1, 0},
        std::vector<int>{0, 0, 0, 0, 1, 1},
        std::vector<int>{1, 1, 0, 0, 1, 0},
        std::vector<int>{0, 1, 1, 1, 0, 0},
        std::vector<int>{0, 0, 1, 0, 0, 0}
    };

    print(radio_k_coloring(adj_mat, k));

    // Graph from the paper's example
    k = 4;
    adj_mat = std::vector<std::vector<int>>{
        std::vector<int>{0, 1, 0, 1, 0, 1, 0, 0},
        std::vector<int>{1, 0, 1, 1, 0, 0, 0, 0},
        std::vector<int>{0, 1, 0, 0, 0, 0, 0, 0},
        std::vector<int>{1, 1, 0, 0, 1, 0, 0, 0},
        std::vector<int>{0, 0, 0, 1, 0, 0, 0, 0},
        std::vector<int>{1, 0, 0, 0, 0, 0, 1, 1},
        std::vector<int>{0, 0, 0, 0, 0, 1, 0, 0},
        std::vector<int>{0, 0, 0, 0, 0, 1, 0, 0}
    };

    print(radio_k_coloring(adj_mat, k));

    return 0;
}
