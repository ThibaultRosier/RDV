#ifndef PPMIMAGE_H
#define PPMIMAGE_H

#include <limits>
#include "Pixel.h"

using namespace std;

class PpmImage
{

private:


    int hauteur;
    int largeur;

public:

    vector<int> zBuffer;
    vector<Pixel*> lesPixels;

    PpmImage(int hauteur, int largeur);

    void setRGB(int x, int y, unsigned int r, unsigned int g, unsigned int b);

    void creeImage();

    virtual ~PpmImage();

};

#endif // PPMIMAGE_H
