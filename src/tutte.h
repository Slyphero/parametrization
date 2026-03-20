#pragma once

#include <map>
#include <set>

#include "mesh_io.h"
#include "typedefs.h"

class Tutte {
 public:
  std::set<edge> cutEdgeSet;
  std::map<unsigned int, unsigned int> cutEdgeMap;

  Tutte() = default;
  void buildEdgeSet(const MeshIOData& data);
  void printEdgeSet();

  void buildEdgeMap();
  void printEdgeMap();
};