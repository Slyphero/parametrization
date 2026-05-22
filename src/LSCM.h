#pragma once

#include <set>
#include <map>

#include "mesh_io.h"
#include "typedefs.h"

class LSCM {
public:
    std::set<edge> cutEdgeSet;
    std::set<edge> notCutEdgeSet;

    std::vector<Point> PointPos;
    std::map<unsigned int, unsigned int> systemsLinesIndicesMap;

    LSCM() = default;
    void buildEdgeSet(const MeshIOData &data);
    void selectFixPoints(const MeshIOData &data);
    void solveLSCM(const MeshIOData &data);
    int variable_index(int i);

    int lowy;
    int highy;
};

