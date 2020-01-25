/**
 * @file    tree_mesh_builder.h
 *
 * @author  Vladimir Dusek <xdusek27@stud.fit.vutbr.cz>
 *
 * @brief   Parallel Marching Cubes implementation using OpenMP tasks + octree early elimination
 *
 * @date    5 December 2019
 **/

#ifndef TREE_MESH_BUILDER_H
#define TREE_MESH_BUILDER_H

#include "base_mesh_builder.h"


class TreeMeshBuilder : public BaseMeshBuilder
{
public:
    TreeMeshBuilder(unsigned gridEdgeSize);

protected:
    unsigned marchCubes(const ParametricScalarField &field);
    float evaluateFieldAt(const Vec3_t<float> &pos, const ParametricScalarField &field);
    void emitTriangle(const Triangle_t &triangle);
    const Triangle_t *getTrianglesArray() const;

    std::vector<Triangle_t> mTriangles; ///< Temporary array of triangles

private:
    unsigned decomposeCube(Vec3_t<float> &cubeOrigin, int edge, const ParametricScalarField &field);
};

#endif // TREE_MESH_BUILDER_H
