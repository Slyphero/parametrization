#include "tutte.h"

#include <iostream>

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
  std::vector<Eigen::Triplet<float>> coefficients ;
  // std::set<edge> allEdgeSet = cutEdgeSet+notCutEdgeSet;
  // for (const edge& e : allEdgeSet) {
  //   coefficients.emplace_back(e.first);
  // }
  coefficients.emplace_back(0,2,0.25) ;
  coefficients.emplace_back(1,3,0.25) ;
}