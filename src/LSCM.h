#pragma once

#include <set>
#include <map>
#include <cmath>

#include "mesh_io.h"
#include "vec.h"
#include "Eigen/Sparse"
#include "Eigen/Dense"
#include "typedefs.h"

class LSCM {
public:
    std::vector<Point> PointPos;

    LSCM() = default;
    LSCM(const LSCM &) = delete;
    LSCM(const LSCM &&) = delete;
    LSCM &operator=(const LSCM &) = delete;
    LSCM &operator=(const LSCM &&) = delete;
    ~LSCM() = default;

    void selectFixPoints(const MeshIOData &data);
    void solveLSCM(const MeshIOData &data);
    int variable_index(int i);

    int lowy;
    int highy;
};

