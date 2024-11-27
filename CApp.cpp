#include "CApp.h"
#include "./ChrisRayTracer/qbLinAlg/qbVector.h"

CApp::CApp(){
    isRunning = true;
    pWindow = NULL;
    pRenderer = NULL;
}

bool CApp::OnInit(){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        return false;
    }

    int xSize = 1280;
    int ySize = 720;

    pWindow = SDL_CreateWindow("ChrisRayTracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, xSize, ySize, SDL_WINDOW_SHOWN);

    if(pWindow != NULL){
        pRenderer = SDL_CreateRenderer(pWindow, -1, 0);
        m_image.Initialize(xSize,ySize,pRenderer);

        // test the camera class

        //set bg colour to white
        SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
        SDL_RenderClear(pRenderer);

        //render the scene
        m_scene.Render(m_image);

        //setup a texture
        // cgRT::Texture::Checker testTexture;
        // testTexture.setTransform(qbVector<double>{std::vector<double>{0.0,0.0}},
        // M_PI/4,
        // qbVector<double>{std::vector<double>{4.0, 4.0}});

        //render the texture
        // for(int y=0; y<ySize; ++y){
        //     for(int x=0; x<xSize; ++x){
        //         //comput (u,v)
        //         double u = (static_cast<double>(x) / (static_cast<double>(xSize) / 2.0)) - 1.0;
        //         double v = (static_cast<double>(y) / (static_cast<double>(ySize) / 2.0)) - 1.0;
        //         qbVector<double> uvCoords{std::vector<double> {u, v}};

        //         //get the color from the texture
        //         qbVector<double> pixelColor = testTexture.GetColor(uvCoords);

        //         //update this pixel of the image
        //         m_image.SetPixel(x, y, pixelColor.GetElement(0), pixelColor.GetElement(1), pixelColor.GetElement(2));
        //     }
        // }

        //Display the image
        m_image.Display();

        // show the result
        SDL_RenderPresent(pRenderer);
    } 
    else{
        return false;
    }
return true;
}

int CApp::OnExecute(){
    SDL_Event event;

    if(OnInit() == false) return -1;

    while(isRunning){
        while(SDL_PollEvent(&event)!=0){
            OnEvent(&event);
        }

        OnLoop();
        OnRender();
    }
}

void CApp::OnEvent(SDL_Event *event){
    if(event->type == SDL_QUIT) isRunning = false;
}

void CApp::OnLoop(){

}

void CApp::OnRender(){
    // SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
    // SDL_RenderClear(pRenderer);

    // //Render scene
    // m_scene.Render(m_image);
    // //display image
    // m_image.Display();

    // SDL_RenderPresent(pRenderer);
}

void CApp::OnExit(){
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    pWindow = NULL;
    SDL_Quit();
}

//PRIVATE FUNCS

void CApp::PrintVector(const qbVector<double> &inputVector){
    int nRows = inputVector.GetNumDims();

    for(int row = 0;row < nRows;++row){
        std::cout << std::fixed << std::setprecision(3) << inputVector.GetElement(row) << std::endl;
    }
}