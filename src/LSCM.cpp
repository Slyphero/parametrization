#include "LSCM.h"

#include <iterator>

void LSCM::selectFixPoints(const MeshIOData &data)
{
    double max_y, min_y;

    for (int i = 0; i < data.positions.size(); ++i)
    {
        if (data.positions[i].y < min_y)
        {
            min_y = data.positions[i].y;
            lowy = data.indices[i];
        }
        else if (data.positions[i].y > max_y)
        {
            max_y = data.positions[i].y;
            highy = data.indices[i];
        }
    }
}

int LSCM::variable_index(int i)
{
    if (i == lowy)
    {
        return -1 ;
    }
    if (i == highy)
    {
        return -2 ;
    }

    int res = i ;
    if (i > lowy)
    {
        res -= 1;
    }
    if (i < highy)
    {
        res -= -1;
    }
    return res;
}

void LSCM::solveLSCM(const MeshIOData &data)
{
    int facesCount = data.indices.size() / 3;
    int verticesCount = data.positions.size();

    Eigen::SparseMatrix<float> system( 2 * facesCount, 2 * (verticesCount - 2) );
    Eigen::SparseMatrix<float> rhsGenerator( 2 * facesCount, 4 );

    // Parcourir toutes les faces (triangles)
    for (unsigned int i = 0; i < data.indices.size() - 2; i += 3)
    {
        // Obtenir les sommets (indices) du triangle étudié
        unsigned int triangleIndexes[3] = {
            data.indices[i],
            data.indices[i + 1],
            data.indices[i + 2]
        };

        // Récupérer les positions associées à chaque indice
        Point trianglePositions[3] = {
            data.positions[triangleIndexes[0]],
            data.positions[triangleIndexes[1]],
            data.positions[triangleIndexes[2]]
        };

        // Construire la base locale
        Vector e0 = trianglePositions[1] - trianglePositions[0];
        e0 = normalize(e0);

        Vector e2 = cross(e0, trianglePositions[2] - trianglePositions[0]);

        Vector e1 = cross(e2, e0);
        e1 = normalize(e1);

        // 2 * aire triangle
        float dt = std::sqrt(length(cross(
            trianglePositions[1] - trianglePositions[0],
            trianglePositions[2] - trianglePositions[0]
            )));

        // Coordonnées locales

        std::vector<float> px, py;

        for (Point pp : trianglePositions ) {
            px.push_back(dot((pp - trianglePositions[0]),e0));
            py.push_back(dot((pp - trianglePositions[0]),e1));
        }

        // Construire système
        for (int j = 0; j < 3; ++j)
        {

        }
    }

    Eigen::VectorXf pins(4);
    pins[0] = 0.5;
    pins[1] = 0.8;
    pins[2] = 0.5;
    pins[3] = 0.2;

    Eigen::SparseMatrix<float> lsqSystem( 2 * (verticesCount - 2), 2 * (verticesCount - 2) );
    Eigen::VectorXf lsqRhs(2 * (verticesCount - 2));

    Eigen::VectorXf solution(2 * (verticesCount - 2));
    Eigen::LeastSquaresConjugateGradient<Eigen::SparseMatrix<float>> solver;
    solver.compute(lsqSystem);
    solution = solver.solve(lsqRhs);
}
