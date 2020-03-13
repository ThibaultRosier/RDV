#ifndef PPMIMAGE_H
#define PPMIMAGE_H


#include "Pixel.h"

using namespace std;

class PpmImage
{

    private:

        vector<Pixel*> lesPixels;
        int hauteur;
        int largeur;

    public:

        PpmImage(int hauteur,int largeur);

        setRGB(int x,int y,unsigned int r, unsigned int g , unsigned int b);

        creeImage();

        virtual ~PpmImage();

};

#endif // PPMIMAGE_H
