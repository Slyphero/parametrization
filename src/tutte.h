#pragma once

#include <map>
#include <set>
#include <vector>

#include "Eigen/Sparse"
#include "Eigen/Dense"
#include "mesh_io.h"
#include "typedefs.h"

class Tutte
{
public:
    std::set<edge> cutEdgeSet;
    std::set<edge> notCutEdgeSet;
    std::map<unsigned int, unsigned int> cutEdgeMap;
    std::vector<Point> edgePointPos;

    std::map<unsigned int, unsigned int> systemsLinesIndicesMap;

    Tutte() = default;
    Tutte(const Tutte &) = delete;
    Tutte(const Tutte &&) = delete;
    Tutte &operator=(const Tutte &) = delete;
    Tutte &operator=(const Tutte &&) = delete;
    ~Tutte() = default;

    void buildEdgeSet(const MeshIOData &data);
    void printEdgeSet();

    void buildEdgeMap();
    void printEdgeMap();

    void buildEdgePos(const MeshIOData &data);
    void printEdgePos();

    void calculNeighbors(const MeshIOData &data);
    void printNeighbors();

    void buildInsidePos(const MeshIOData &data);
};