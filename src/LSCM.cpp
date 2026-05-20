#include "LSCM.h"

void LSCM::buildEdgeSet(const MeshIOData &data)
{
    unsigned int indicesCount = data.indices.size();

    for (unsigned int i = 0; i < indicesCount - 2; i += 3)
    {
        edge edgeArray[3] = {std::make_pair(data.indices[i], data.indices[i + 1]),
                             std::make_pair(data.indices[i + 1], data.indices[i + 2]),
                             std::make_pair(data.indices[i + 2], data.indices[i])};

        for (int j = 0; j < 3; ++j)
        {
            edge invertedEdge = std::make_pair(edgeArray[j].second, edgeArray[j].first);

            if (cutEdgeSet.contains(invertedEdge))
            {
                cutEdgeSet.erase(cutEdgeSet.find(invertedEdge));
                notCutEdgeSet.insert(edgeArray[j]);
            }
            else
            {
                cutEdgeSet.insert(edgeArray[j]);
            }
        }
    }
}

void LSCM::selectFixPoints(const MeshIOData &data) {
    double max_y, min_y;
    unsigned indice_max_y, indice_min_y;

    for (int i = 0; i < data.positions.size(); ++i) {
        if (data.positions[i].y < min_y) {
            min_y = data.positions[i].y;
            indice_min_y = data.indices[i];
        }
        else if (data.positions[i].y > max_y) {
            max_y = data.positions[i].y;
            indice_max_y = data.indices[i];
        }
    }

    PointPos.push_back(Point(0.5, 0.8, 0.));
    PointPos.push_back(Point(0.5, 0.2, 0.));

    systemsLinesIndicesMap[indice_max_y] = 0;
    systemsLinesIndicesMap[indice_min_y] = 1;
}
