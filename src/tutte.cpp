#include "tutte.h"

#include <iostream>
#include <algorithm>
#include <Eigen/SparseCholesky>

const double PI = 3.14159265358979323846;

void Tutte::buildEdgeSet(const MeshIOData& data) {
  unsigned int indicesCount = data.indices.size();

  for (unsigned int i = 0; i < indicesCount - 2; i += 3) {
    edge edgeArray[3] = {std::make_pair(data.indices[i], data.indices[i + 1]),
                         std::make_pair(data.indices[i + 1], data.indices[i + 2]),
                         std::make_pair(data.indices[i + 2], data.indices[i])};

    for (int j = 0; j < 3; ++j) {
      edge invertedEdge = std::make_pair(edgeArray[j].second, edgeArray[j].first);

      if (cutEdgeSet.contains(invertedEdge)) {
        cutEdgeSet.erase(cutEdgeSet.find(invertedEdge));
        notCutEdgeSet.insert(edgeArray[j]);
      } else {
        cutEdgeSet.insert(edgeArray[j]);
      }
    }
  }
}

void Tutte::printEdgeSet() {
  for (const edge& e : cutEdgeSet) {
    std::cout << "(" << e.first << ", " << e.second << ")" << std::endl;
  }
  std::cout << "Nombre d'arêtes: " << cutEdgeSet.size() << std::endl;
}

void Tutte::buildEdgeMap() {
  for (const edge& e : cutEdgeSet) {
    cutEdgeMap[e.first] = e.second;
  }
}

void Tutte::printEdgeMap() {
  int first = cutEdgeMap.begin()->first;
  int current = first;
  while (cutEdgeMap[current] != first) {
    std::cout << current << " -> ";
    current = cutEdgeMap[current];
  }
  std::cout << first << std::endl;
}

void Tutte::buildEdgePos(const MeshIOData& data) {
  int k = cutEdgeMap.size();
  int currentKey = cutEdgeMap.begin()->first;
  edgePointPos.clear();
  edgePointPos.resize(data.positions.size(), Point(0., 0., 0.));
  for (int i = 0; i < k; ++i) {
    edgePointPos[currentKey] = Point(cos((2. * PI * i) / k), sin((2. * PI * i) / k), 0.);
    currentKey = cutEdgeMap[currentKey];
  }
}

void Tutte::printEdgePos() {
  for (Point p : edgePointPos) {
    std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
  }
}

void Tutte::calculNeighbors(const MeshIOData &data) {
  numberNeighbor.resize(data.positions.size(), 0);

  for (const edge& e : notCutEdgeSet) {
    numberNeighbor[e.first] += 1;
    numberNeighbor[e.second] += 1;
  }
}

void Tutte::printNeighbors() {
  for (int i = 0; i < numberNeighbor.size(); ++i) {
    std::cout << "(" << numberNeighbor[i] << ", " << i << ")" << std::endl;
  }
}

void Tutte::buildInsidePos(const MeshIOData &data) {
  int index = 0;
  systemsLinesIndicesMap.clear();

  // 1. Build boundary vertex set
  std::set<unsigned int> boundaryVertices;
  for (const auto& kv : cutEdgeMap) {
    boundaryVertices.insert(kv.first);
  }

  // Renumber interior vertices
  for (int i = 0; i < data.positions.size(); ++i) {
    if (boundaryVertices.find(i) == boundaryVertices.end()) {
      systemsLinesIndicesMap[i] = index;
      index++;
    }
  }

  int n = systemsLinesIndicesMap.size();

  // 2. Prepare system
  std::vector<Eigen::Triplet<double>> coefficients;
  Eigen::VectorXd rhs(2 * n);
  rhs.setZero();

  // Build adjacency list
  std::vector<std::vector<int>> neighbors(data.positions.size());
  std::set<edge> allEdgeSet;
  allEdgeSet.insert(cutEdgeSet.begin(), cutEdgeSet.end());
  allEdgeSet.insert(notCutEdgeSet.begin(), notCutEdgeSet.end());

  for (const edge& e : allEdgeSet) {
    if (std::find(neighbors[e.first].begin(), neighbors[e.first].end(), e.second) == neighbors[e.first].end())
      neighbors[e.first].push_back(e.second);

    if (std::find(neighbors[e.second].begin(), neighbors[e.second].end(), e.first) == neighbors[e.second].end())
      neighbors[e.second].push_back(e.first);
  }

  // 3. Fill matrix and rhs
  for (auto& [vertex, idx] : systemsLinesIndicesMap) {
    int rowX = 2 * idx;
    int rowY = 2 * idx + 1;

    int deg = neighbors[vertex].size();
    if (deg == 0) {
      coefficients.emplace_back(rowX, rowX, 1.0);
      coefficients.emplace_back(rowY, rowY, 1.0);
      continue;
    }

    // Use unnormalized Laplacian: deg * x_i - sum x_j = sum (boundary neighbors)
    // diagonal
    coefficients.emplace_back(rowX, rowX, static_cast<double>(deg));
    coefficients.emplace_back(rowY, rowY, static_cast<double>(deg));

    for (int voisin : neighbors[vertex]) {
      if (systemsLinesIndicesMap.find(voisin) != systemsLinesIndicesMap.end()) {
        int j = systemsLinesIndicesMap[voisin];
        coefficients.emplace_back(rowX, 2 * j, -1.0);
        coefficients.emplace_back(rowY, 2 * j + 1, -1.0);
      } else {
        rhs[rowX] += edgePointPos[voisin].x;
        rhs[rowY] += edgePointPos[voisin].y;
      }
    }
  }

  // 4. Build sparse matrix
  Eigen::SparseMatrix<double> M(2 * n, 2 * n);
  M.setFromTriplets(coefficients.begin(), coefficients.end());

  // 5. Solve
  Eigen::SimplicialLLT<Eigen::SparseMatrix<double>> solver;
  solver.compute(M);
  Eigen::VectorXd solution = solver.solve(rhs);

  if (solver.info() != Eigen::Success) {
    std::cout << "Solver failed!" << std::endl;
  }

  // 6. Write back solution
  for (auto& [vertex, idx] : systemsLinesIndicesMap) {
    edgePointPos[vertex].x = solution[2 * idx];
    edgePointPos[vertex].y = solution[2 * idx + 1];
  }
}