#include "tutte.h"

#include <iostream>

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
}