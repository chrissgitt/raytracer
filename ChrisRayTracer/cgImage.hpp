#ifndef CGIMAGE_H
#define CGIMAGE_H
#include <string>
#include <vector>
#include <SDL2/SDL.h>

class cgImage{

    public:
        cgImage();

        ~cgImage();

        // func to initialise
        void Initialize(const int xSize, const int ySize, SDL_Renderer *pRenderer);

        //func to set pixel colour
        void SetPixel(const int x, const int y, const double red, const double green, const double blue);

        void Display();

        //Funtions
        int GetXSize();
        int GetYSize();
    
    private:
        Uint32 ConverColor(const double red, const double green, const double blue);
        void InitTexture();
        void computeMaxValues();

    private:
        std::vector<std::vector<double>> m_rChannel;
        std::vector<std::vector<double>> m_gChannel;
        std::vector<std::vector<double>> m_bChannel;

        int m_xSize, m_ySize;
        // store maximum values
        double m_maxRed, m_maxGreen, m_maxBlue, m_overallMax;

        SDL_Renderer *m_pRenderer;
        SDL_Texture *m_pTexture;
};
#endif