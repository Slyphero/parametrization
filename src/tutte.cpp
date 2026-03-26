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
  /*
  auto it = cutEdgeSet.begin();
  const edge firstEdge = *it;
  std::cout << "(" << firstEdge.first << ", " << firstEdge.second << ")" << std::endl;

  cutEdgeMap[firstEdge.first] = firstEdge.second;

  while (it->second != firstEdge.first) {
    it = cutEdgeSet.lower_bound(std::make_pair(it->first, 0U));
    cutEdgeMap[it->first] = it->second;
  }
  */
  for (const edge& e : cutEdgeSet) {
    cutEdgeMap[e.first] = e.second;
  }
}

void Tutte::printEdgeMap() {
  for (const auto& [key, value] : cutEdgeMap) {
    std::cout << key << ": " << value << std::endl;
  }
}

void Tutte::buildEdgePos(const MeshIOData& data) {
  int k = cutEdgeMap.size();
  int currentKey = 0;
  edgePointPos.clear();
  edgePointPos.resize(data.indices.size(), Point(0., 0., 0.));
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