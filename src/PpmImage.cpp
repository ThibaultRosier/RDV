#include "PpmImage.h"

PpmImage::PpmImage(int hauteur,int largeur)
{
    this->hauteur = hauteur;
    this->largeur = largeur;

    int taille = hauteur * largeur;

    for(int i = 0 ; i < taille ; i++)
    {
        lesPixels.push_back (new Pixel());
    }
}

PpmImage::setRGB(int x,int y,unsigned int r, unsigned int g , unsigned int b)
{
    lesPixels[x+y*(largeur-1)]->setRGB(r,g,b);
}

PpmImage::~PpmImage()
{
    int taille = hauteur * largeur;

    for(int i = 0 ; i < taille ; i++)
    {
        delete lesPixels[i];
    }
}

PpmImage::creeImage()
{
    FILE* ppm = NULL;
    ppm = fopen("diablo.ppm", "w+");

    fprintf(ppm, "P3\n");
    fprintf(ppm, "%d %d",largeur,hauteur);
    fprintf(ppm, "\n255\n");

    for(int i = hauteur-1 ; i > 0 ; i--)
    {
        for(int j = 0 ; j < largeur ; j++)
        {
            fprintf(ppm, "%d %d %d  ",lesPixels[j+i*(largeur-1)]->getR(),lesPixels[j+i*(largeur-1)]->getG(),lesPixels[j+i*(largeur-1)]->getB());
        }
        fprintf(ppm, "\n");
    }

    fclose(ppm);
}
