#include "PpmImage.h"
#define _CRT_SECURE_NO_WARNINGS

PpmImage::PpmImage(int hauteur, int largeur)
{
    this->hauteur = hauteur;
    this->largeur = largeur;

    int taille = (hauteur + 1) * largeur;
    int moinInf = numeric_limits<int>::min();

    for (int i = 0; i < taille; i++)
    {
        lesPixels.push_back(new Pixel());
        zBuffer.push_back(moinInf);
    }
}

void PpmImage::setRGB(int x, int y, unsigned int r, unsigned int g, unsigned int b)
{
    lesPixels[x + y * (largeur)]->setRGB(r, g, b);
}

PpmImage::~PpmImage()
{
    int taille = hauteur * largeur;

    for (int i = 0; i < taille; i++)
    {
        delete lesPixels[i];
    }
}

void PpmImage::creeImage()
{
    FILE* ppm = NULL;
    ppm = fopen("diablo.ppm", "w+");

    fprintf(ppm, "P3\n");
    fprintf(ppm, "%d %d", largeur, hauteur);
    fprintf(ppm, "\n255\n");

    for (int i = hauteur - 1; i >= 0; i--)
    {
        for (int j = 0; j < largeur; j++)
        {
            fprintf(ppm, "%d %d %d  ", lesPixels[j + i * (largeur)]->getR(), lesPixels[j + i * (largeur)]->getG(), lesPixels[j + i * (largeur)]->getB());
        }
        fprintf(ppm, "\n");
    }

    fclose(ppm);
}
