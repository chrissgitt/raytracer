#ifndef CAPP_H
#define CAPP_H

#include <SDL2/SDL.h>
#include "./ChrisRayTracer/cgImage.hpp"
#include "./ChrisRayTracer/scene.hpp"
#include "./ChrisRayTracer/camera.hpp"
#include "./ChrisRayTracer/cgTextures/flat.hpp"
#include "./ChrisRayTracer/cgTextures/checker.hpp"

class CApp{

    public:
        CApp();

        int OnExecute();
        bool OnInit();
        void OnEvent(SDL_Event *event);
        void OnLoop();
        void OnRender();
        void OnExit();

    private:
        void PrintVector(const qbVector<double> &inputvector);

    private:
        //instance of cgImage class to store image
        cgImage m_image;

        //instance of scene class
        cgRT::Scene m_scene;
        // SDL2 stuff
        bool isRunning;
        SDL_Window *pWindow;
        SDL_Renderer *pRenderer;
};

#endif