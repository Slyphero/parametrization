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

GLuint defaultVao = 0;
GLuint tutteVao = 0;
GLuint lscmVao = 0;

unsigned count = 0;
std::string assetsPath = "projets/parametrization/assets/";

float zoomFactor = -3.0f;
float offset_x = 0.0f;
float offset_y = 0.0f;

GLuint texture;

Tutte tutte;
LSCM lscm;

bool init() {
    MeshIOData data;

    texture = read_texture(0, (assetsPath + "blender_checker.png").c_str());

    if (!read_meshio_data((assetsPath + "suzanne_uvsplit.obj").c_str(), data)) { return false; }

    default_texture(0, texture);

    tutte.buildEdgeSet(data);
    tutte.buildEdgeMap();
    tutte.buildEdgePos(data);
    tutte.buildInsidePos(data);

    lscm.selectFixPoints(data);
    lscm.solveLSCM(data);

    defaultVao = create_buffers(data.positions, data.indices, data.positions, data.normals);
    tutteVao = create_buffers(tutte.edgePointPos, data.indices);
    lscmVao = create_buffers(lscm.PointPos, data.indices);
    
    /* ou
        vao= create_buffers(data.positions, data.indices, data.texcoords,
       data.normals); mais s'il y a des coordonn�es de textures dans l'objet, il
       faut aussi une texture pour le dessiner. on verra plus tard. donc pour
       l'instant on ne cree pas de coordonn�es de texture...
     */
    count = data.indices.size();

    std::cout << std::endl 
            << "================================" << std::endl
            << "Deplacement : Fleches directionnelles" << std::endl
            << "Zoom : Espace / Backspace" << std::endl
            << "Parametrisation de Tutte : T" << std::endl
            << "Parametrisation LSCM : L" << std::endl
            << "Affichage Suzanne par defaut : R" << std::endl
            << "================================" << std::endl << std::endl;

    return true;
}

void quit() { 
    release_buffers(defaultVao); 
    release_buffers(tutteVao); 
    release_buffers(lscmVao); 
}

void draw(bool isParam, const GLuint& myVao) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Transform model;                                // placer le modele
    Transform view = Translation(offset_x, offset_y, zoomFactor); // camera
    Transform projection = Perspective(45, 1024.0 / 576.0, 0.1, 100);

    if (isParam) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
    else { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }

    draw(myVao, GL_TRIANGLES, count, model, view, projection);
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

    if (!init()) { return 1; }

    bool close = false;
    bool isTutte = false;
    bool isLscm = false;

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
                offset_y -= 0.1f;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN) {
                offset_y += 0.1f;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RIGHT) {
                offset_x -= 0.1f;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_LEFT) {
                offset_x += 0.1f;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                zoomFactor += 0.1f;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE) {
                zoomFactor -= 0.1f;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r) {
                isTutte = false;
                isLscm = false;
                zoomFactor = -3.0f;
                offset_x = 0.0f;
                offset_y = 0.0f;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_t) {
                isTutte = true;
                isLscm = false;
                zoomFactor = -3.0f;
                offset_x = 0.0f;
                offset_y = 0.0f;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_l) {
                isTutte = false;
                isLscm = true;
                zoomFactor = -3.0f;
                offset_x = -0.65f;
                offset_y = -0.65f;
            }
        }

        // dessiner
        if (isTutte) { draw(true, tutteVao); }
        else if (isLscm) { draw(true, lscmVao); }
        else { draw(false, defaultVao); }

        // presenter / montrer le resultat
        SDL_GL_SwapWindow(window);
    }

    quit();

    release_context(context);
    release_window(window);
    return 0;
}
