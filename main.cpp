#include <vector>
#include <cmath>
#include "PpmImage.h"
#include "model.h"
#include "geometry.h"

using namespace std;

#define PI 3.14159265

/** Code de Base **/

Model* model = NULL;
const int width = 800;
const int height = 800;
const int profondeur = 800;


/** Fin Code Base **/


/**********                  Code Perso                        *************/


void intervertir(int* v0, int* v1)
{
    *v0 -= *v1;
    *v1 += *v0;
    *v0 -= *v1;
    *v0 = -*v0;
}

int produitVectoriel(int xA, int yA, int xB, int yB, int xC, int yC)
{
    return (xA - xC) * (yB - yC) - (yA - yC) * (xB - xC);
}


void line(Vec3f* v0, Vec3f* v1, PpmImage* image)
{

    /** Ici on adapte notre maillage a notre image tout en la centrant **/
    int x0 = (v0->x + 1.) * width / 2.;
    int y0 = (v0->y + 1.) * height / 2.;
    int x1 = (v1->x + 1.) * width / 2.;
    int y1 = (v1->y + 1.) * height / 2.;

    bool horizontal = true;
    float e;//valeur de l'erreur
    float de;//corespond aux raport de la difference des ordo sur la diference des absi (valeur absolu)
    int x, y;
    /*** On veut savoir si la difference des abscice (valeur absolu) est superieur ou non a la difference des abscisse (valeur absolue) pour faire une boucle en consequence ***/

    if (abs(x0 - x1) < abs(y0 - y1))
    {
        horizontal = false;
        /** Si la pente n'est pas dit horizontal (si elle vari plus verticalement que horizontalement )**/
        /** alors on inverse le repere pour simplifier le code **/

        intervertir(&x0, &y0);
        intervertir(&x1, &y1);
    }

    if (x0 > x1)// si x0 < x1 alors pour aller dans le bon sens on les inverse
    {
        intervertir(&x0, &x1);
        intervertir(&y0, &y1);
    }
    de = abs((float)(y1 - y0) / (float)(x1 - x0));
    e = 0;
    y = y0;

    for (x = x0; x <= x1; x++)
    {
        if (horizontal)
        {
            image->setRGB(x, y, 255, 255, 255);
        }
        else
        {
            image->setRGB(y, x, 255, 255, 255);
        }
        e += de;
        if (e > 0.5)
        {
            if (y0 > y1)//si y0 > y1  il faut logiquement dessendre
            {
                y--;
            }
            else
            {
                y++;
            }
            e -= 1.;
        }

    }
}

bool estDansTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int xM, int yM)
{
    if (produitVectoriel(x0, y0, x1, y1, xM, yM) < 0)
    {
        return false;
    }
    if (produitVectoriel(x1, y1, x2, y2, xM, yM) < 0)
    {
        return false;
    }
    if (produitVectoriel(x2, y2, x0, y0, xM, yM) < 0)
    {
        return false;
    }
    return true;
}

void triangle(Vec3f* v0, Vec3f* v1, Vec3f* v2, PpmImage* image)
{
    /** Ici on adapte notre maillage a notre image tout en la centrant **/
    int x0 = (v0->x + 1.) * width / 2.;
    int y0 = (v0->y + 1.) * height / 2.;
    int x1 = (v1->x + 1.) * width / 2.;
    int y1 = (v1->y + 1.) * height / 2.;
    int x2 = (v2->x + 1.) * width / 2.;
    int y2 = (v2->y + 1.) * height / 2.;

    int xMax = x0, xMin = x0, yMin = y0, yMax = y0;



    /** on va vouloir trouver les coordonnées x et y minimal et maximal des 3 sommet **/

    if (x0 > x1)
    {
        xMin = x1;
    }
    else
    {
        xMax = x1;
    }

    if (xMin > x2)
    {
        xMin = x2;
    }
    if (xMax < x2)
    {
        xMax = x2;
    }


    if (y0 > y1)
    {
        yMin = y1;
    }
    else
    {
        yMax = y1;
    }

    if (yMin > y2)
    {
        yMin = y2;
    }
    if (yMax < y2)
    {
        yMax = y2;
    }

    /*** On parcour donc la zone delimitee ***/
    bool dejColor;
    for (int y = yMin; y <= yMax; y++)
    {
        dejColor = false;
        for (int x = xMin; x <= xMax; x++)
        {
            if (estDansTriangle(x0, y0, x1, y1, x2, y2, x, y))
            {
                image->setRGB(x, y, 255, 255, 255);
                dejColor = true;
            }
            else
            {
                if (dejColor)//On a colorier et on arrive a un pixel non colorier donc on peut sauter tout les autre etape de la boucle actu
                {
                    break;
                }
            }
        }
    }
}

void intervertir(float* v0, float* v1)
{
    *v0 -= *v1;
    *v1 += *v0;
    *v0 -= *v1;
    *v0 = -*v0;
}

void intervetirVecteur(Vec3f* A, Vec3f* B)
{
    intervertir(&(A->x), &(A->x));
    intervertir(&(A->y), &(A->y));
    intervertir(&(A->z), &(A->z));
}

int rechercheDeZ(Vec3f* A, Vec3f* B, Vec3f* C, int x, int y)//renvoi la coordoner z du point
{
    double t = 0, s;
    float z = 0,zAlpha,zBeta,zDelta = 0;
    bool AB = true;
    bool AC = true;
    bool BC = false;
    int delta;

    /*** Pour AB ***/
    delta = B->x - A->x;
    if (delta != 0)
    {
        zAlpha = (B->z - A->z) / (delta);
        zAlpha = A->z + zAlpha * (x - A->x);
    }
    else {
        delta = B->y - A->y;
        if (delta != 0)
        {
            zAlpha = (B->z - A->z) / (delta);
            zAlpha = A->z + zAlpha * (y - A->y);
        }
        else {
            AB = false;
        }
    }

    /*** Pour AC ***/
    delta = C->x - A->x;
    if (delta != 0)
    {
        zBeta = (C->z - A->z) / (delta);
        zBeta = A->z + zBeta * (x - A->x);
    }
    else {
        delta = B->y - A->y;
        if (delta != 0)
        {
            zBeta = (C->z - A->z) / (delta);
            zBeta = A->z + zBeta * (y - A->y);
        }
        else {
            AC = false;
        }
    }

    if (!AC) {
        BC = true;

        delta = C->x - B->x;
        if (delta != 0)
        {
            zBeta = (C->z - B->z) / (delta);
            zBeta = B->z + zBeta * (x - B->x);
        }
        else {
            delta = C->y - B->y;
            if (delta != 0)
            {
                zBeta = (C->z - B->z) / (delta);
                zBeta = B->z + zBeta * (y - B->y);
            }
        }
    }

    if (!AB) {
        BC = true;

        delta = C->x - B->x;
        if (delta != 0)
        {
            zAlpha = (C->z - B->z) / (delta);
            zAlpha = B->z + zAlpha * (x - B->x);
        }
        else {
            delta = C->y - B->y;
            if (delta != 0)
            {
                zAlpha = (C->z - B->z) / (delta);
                zAlpha = B->z + zAlpha * (y - B->y);
            }
        }
    }


    zDelta = (zBeta + zAlpha)/(2);


    return (int)zDelta;
}

void mettreCouleur(Vec3f *lumiere,Vec3f *iso,Vec3f *p,PpmImage *image)
{

    float angleOpti = 90;
    float angle = 90;
    float cos;

    Vec3f vecteur;
    vecteur.x = p->x - iso->x;
    vecteur.y = p->y - iso->y;
    vecteur.z = p->z - iso->z;

    cos = (lumiere->x*vecteur.x + lumiere->y*vecteur.y + lumiere->z * vecteur.z) / ( sqrt(lumiere->x* lumiere->x + lumiere->y * lumiere->y + lumiere->z * lumiere->z ) * sqrt(vecteur.x * vecteur.x + vecteur.y * vecteur.y + vecteur.z * vecteur.z) );
    angle = acos(cos)/PI *180;

    float ratioLumineu = 1-(abs( angleOpti - abs(angle) )/angleOpti);

    image->setRGB(p->x, p->y, ratioLumineu*255, ratioLumineu*255, ratioLumineu*255);
}

void triangleZBuffer(Vec3f* v0, Vec3f* v1, Vec3f* v2, PpmImage* image,Vec3f *lumiere)
{
    /** Ici on adapte notre maillage a notre image tout en la centrant **/
    int x0 = (v0->x + 1.) * width / 2.;
    int y0 = (v0->y + 1.) * height / 2.;
    int z0 = (v0->z + 1.) * profondeur / 2.;

    int x1 = (v1->x + 1.) * width / 2.;
    int y1 = (v1->y + 1.) * height / 2.;
    int z1 = (v1->z + 1.) * profondeur / 2.;

    int x2 = (v2->x + 1.) * width / 2.;
    int y2 = (v2->y + 1.) * height / 2.;
    int z2 = (v2->z + 1.) * profondeur / 2.;

    /*** Calcul isoBarycentre ***/
    Vec3f iso;
    iso.x = (x0 + x1 + x2) / 3;
    iso.y = (y1 + y2 + y0) / 3;
    iso.z = (z0 + z1 + z2) / 3;

    /*** On definie les point optenue ***/

    Vec3f vecteurA;
    vecteurA.x = x0;
    vecteurA.y = y0;
    vecteurA.z = z0;

    Vec3f vecteurB;
    vecteurB.x = x1;
    vecteurB.y = y1;
    vecteurB.z = z1;

    Vec3f vecteurC;
    vecteurC.x = x2;
    vecteurC.y = y2;
    vecteurC.z = z2;

    Vec3f p;

    int xMax = x0, xMin = x0, yMin = y0, yMax = y0;

    /** on va vouloir trouver les coordonnées x et y minimal et maximal des 3 sommet **/

    if (x0 > x1)
    {
        xMin = x1;
    }
    else
    {
        xMax = x1;
    }

    if (xMin > x2)
    {
        xMin = x2;
    }
    if (xMax < x2)
    {
        xMax = x2;
    }


    if (y0 > y1)
    {
        yMin = y1;
    }
    else
    {
        yMax = y1;
    }

    if (yMin > y2)
    {
        yMin = y2;
    }
    if (yMax < y2)
    {
        yMax = y2;
    }

    /*** On parcour donc la zone delimitee ***/
    bool dejColor;
    int z, indexBuffer;

    for (int y = yMin; y <= yMax; y++)
    {
        dejColor = false;
        for (int x = xMin; x <= xMax; x++)
        {
            if (estDansTriangle(x0, y0, x1, y1, x2, y2, x, y))
            {

                z = rechercheDeZ(&vecteurA, &vecteurB, &vecteurC, x, y);
                //printf("%d\n",z);
                indexBuffer = x + y * width;
                if (image->zBuffer[indexBuffer] < z)
                {
                    p.x = x;
                    p.y = y;
                    p.z = z;

                    mettreCouleur(lumiere, &iso,&p,image);
                    image->zBuffer[indexBuffer] = z;
                    dejColor = true;
                }
            }
            else
            {
                if (dejColor)//On a colorier et on arrive a un pixel non colorier donc on peut sauter tout les autre etape de la boucle actu
                {
                    break;
                }
            }
        }
    }
}

int main(int argc, char** argv)
{
    Vec3f lumiere;
    lumiere.x = 0;
    lumiere.y = 0;
    lumiere.z = 1;

    if (2 == argc) {
        model = new Model(argv[1]);
    }
    else {
        model = new Model("obj/diablo3_pose.obj");
    }

    PpmImage* image = new PpmImage(height, width);
    for (int i = 0; i < model->nfaces(); i++)
    {
        vector<int> face = model->face(i);
        /**** Partie 1 ****/
        /*for (int j=0; j<3; j++)
        {
            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j+1)%3]);

            line(&v0, &v1, image);
        }*/

        /***** Partie 2 ****/
        Vec3f v0 = model->vert(face[0]);
        Vec3f v1 = model->vert(face[1]);
        Vec3f v2 = model->vert(face[2]);

        triangleZBuffer(&v0, &v1, &v2, image, &lumiere);
    }

    //image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image->creeImage();

    delete image;
    delete model;
    return 0;
}
/**********                  Fin Code Perso                        *************/

/** Code de Test du donner dans le sujet **/


