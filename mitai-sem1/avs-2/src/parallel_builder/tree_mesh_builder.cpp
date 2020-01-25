/**
 * @file    loop_mesh_builder.cpp
 *
 * @author  Vladimir Dusek <xdusek27@stud.fit.vutbr.cz>
 *
 * @brief   Parallel Marching Cubes implementation using OpenMP loops
 *
 * @date    5 December 2019
 **/

#include <iostream>
#include <math.h>
#include <limits>
#include <vector>

#include "tree_mesh_builder.h"


TreeMeshBuilder::TreeMeshBuilder(unsigned gridEdgeSize)
        : BaseMeshBuilder(gridEdgeSize, "Octree")
{

}

unsigned TreeMeshBuilder::decomposeCube(Vec3_t<float> &cubeOrigin, int edge, const ParametricScalarField &field)
{
    unsigned totalTriangles = 0;

    if (edge <= 1) {
        totalTriangles = buildCube(cubeOrigin, field);
    }
    else {
        int newEdge = edge / 2;
        float isoLevel = field.getIsoLevel();
        Vec3_t<float> center(mGridResolution * (cubeOrigin.x + newEdge),
                             mGridResolution * (cubeOrigin.y + newEdge),
                             mGridResolution * (cubeOrigin.z + newEdge));

        double right_side = isoLevel + (sqrt(3) / 2) * edge * mGridResolution;
        double left_side = evaluateFieldAt(center, field);

        if (left_side <= right_side) {

            const int NUM_OF_CHILDREN = 8;

            Vec3_t<float> cubes[NUM_OF_CHILDREN] = {
                    Vec3_t<float>(cubeOrigin.x, cubeOrigin.y, cubeOrigin.z),
                    Vec3_t<float>(cubeOrigin.x + newEdge, cubeOrigin.y, cubeOrigin.z),
                    Vec3_t<float>(cubeOrigin.x, cubeOrigin.y + newEdge, cubeOrigin.z),
                    Vec3_t<float>(cubeOrigin.x, cubeOrigin.y, cubeOrigin.z + newEdge),
                    Vec3_t<float>(cubeOrigin.x + newEdge, cubeOrigin.y + newEdge, cubeOrigin.z),
                    Vec3_t<float>(cubeOrigin.x + newEdge, cubeOrigin.y, cubeOrigin.z + newEdge),
                    Vec3_t<float>(cubeOrigin.x, cubeOrigin.y + newEdge, cubeOrigin.z + newEdge),
                    Vec3_t<float>(cubeOrigin.x + newEdge, cubeOrigin.y + newEdge, cubeOrigin.z + newEdge)
            };

            if (newEdge > 1) {
                unsigned tmp[NUM_OF_CHILDREN] = {0};

                for (int i = 0; i < NUM_OF_CHILDREN; i++) {
                    #pragma omp task shared(tmp)
                    tmp[i] = decomposeCube(cubes[i], newEdge, field);
                }

                #pragma omp taskwait
                for (int i = 0; i < NUM_OF_CHILDREN; i++) {
                    totalTriangles += tmp[i];
                }
            }
            else {
                for (int i = 0; i < NUM_OF_CHILDREN; i++) {
                    totalTriangles += buildCube(cubes[i], field);
                }
            }
        }
        else {
            totalTriangles = 0;
        }
    }

    return totalTriangles;
}

unsigned TreeMeshBuilder::marchCubes(const ParametricScalarField &field)
{
    unsigned totalTriangles;
    Vec3_t<float> cubeOrigin(0, 0, 0);

    #pragma omp parallel
    {
        #pragma omp single
        {
            totalTriangles = decomposeCube(cubeOrigin, mGridSize, field);
        }
    }

    return totalTriangles;
}

float TreeMeshBuilder::evaluateFieldAt(const Vec3_t<float> &pos, const ParametricScalarField &field)
{
    // NOTE: This method is called from "buildCube(...)"!

    float value = std::numeric_limits<float>::max();

    // 2. Find minimum square distance from points "pos" to any point in the field.
    for (const auto point : field.getPoints()) {

        float distanceSquared  = (pos.x - point.x) * (pos.x - point.x);
        distanceSquared += (pos.y - point.y) * (pos.y - point.y);
        distanceSquared += (pos.z - point.z) * (pos.z - point.z);

        // Comparing squares instead of real distance to avoid unnecessary "sqrt"s in the loop.
        value = std::min(value, distanceSquared);
    }

    // 3. Finally take square root of the minimal square distance to get the real distance
    return sqrt(value);
}

void TreeMeshBuilder::emitTriangle(const BaseMeshBuilder::Triangle_t &triangle)
{
    // NOTE: This method is called from "buildCube(...)"!

    // Store generated triangle into vector (array) of generated triangles.
    // The pointer to data in this array is return by "getTrianglesArray(...)" call
    // after "marchCubes(...)" call ends.

    #pragma omp critical
    {
        mTriangles.push_back(triangle);
    }
}

const TreeMeshBuilder::Triangle_t *TreeMeshBuilder::getTrianglesArray() const
{
    return mTriangles.data();
}
