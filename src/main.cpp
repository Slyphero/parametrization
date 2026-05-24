// idem tp1, mais lecture du mesh avec MeshIOData

#include <string>

#include "buffers.h"
#include "default_program.h"
#include "draw.h"
#include "glcore.h"
#include "LSCM.h"
#include "mat.h"
#include "mesh_io.h"
#include "program.h"
#include "texture.h"
#include "tutte.h"
#include "uniforms.h"
#include "vec.h"
#include "window.h"

GLuint vao = 0;
unsigned count = 0;
std::string assetsPath = "projets/parameterization-etu/assets/";

float zoomFactor = -2.;

GLuint texture;

Tutte tutte;
LSCM lscm;

bool init() {
    MeshIOData data;

    texture = read_texture(0, (assetsPath + "blender_checker.png").c_str());

    if (!read_meshio_data((assetsPath + "suzanne_uvsplit.obj").c_str(), data)) {
        return false; // erreur de lecture
    }
    default_texture(0, texture);

    tutte.buildEdgeSet(data);
    tutte.buildEdgeMap();
    tutte.buildEdgePos(data);
    tutte.buildInsidePos(data);

    lscm.selectFixPoints(data);
    lscm.solveLSCM(data);
    
    // vao = create_buffers(data.positions, data.indices, data.positions, data.normals);
    // vao = create_buffers(tutte.edgePointPos, data.indices);'
    vao = create_buffers(lscm.PointPos, data.indices);
    /* ou
        vao= create_buffers(data.positions, data.indices, data.texcoords,
       data.normals); mais s'il y a des coordonn�es de textures dans l'objet, il
       faut aussi une texture pour le dessiner. on verra plus tard. donc pour
       l'instant on ne cree pas de coordonn�es de texture...
     */
    count = data.indices.size();

    return true;
}

void quit() { 
    release_buffers(vao); 
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Transform model;                                // placer le modele
    Transform view = Translation(0, 0, zoomFactor); // camera
    Transform projection = Perspective(45, 1024.0 / 576.0, 0.1, 100);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    draw(vao, GL_TRIANGLES, count, model, view, projection);
}

int main(int argc, char **argv) {
    Window window = create_window(1024, 576);
    Context context = create_context(window); // a virer

    // etat openGL de base / par defaut
    glViewport(0, 0, 1024, 576);
    glClearColor(0.2, 0.2, 0.2, 1);
    glClearDepth(1);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

    if (!init()) {
        return 1;
    }

    bool close = false;
    while (!close) {
        SDL_Event event;

        // recuperer un evenement a la fois, poll event renvoie faux lorsqu'ils
        // ont tous ete traite
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                close = true; // sortir si click sur le bouton 'fermer' de la fenetre
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                close = true; // sortir si la touche esc / echapp est enfoncee
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP) {
                zoomFactor += 0.1;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN) {
                zoomFactor -= 0.1;
            }
        }

        // dessiner
        draw();

        // presenter / montrer le resultat
        SDL_GL_SwapWindow(window);
    }

    quit();

    release_context(context);
    release_window(window);
    return 0;
}
