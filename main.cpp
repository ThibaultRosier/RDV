#include <vector>
#include <cmath>
#include "PpmImage.h"
#include "model.h"
#include "geometry.h"

using namespace std;


/** Code de Base **/
//const TGAColor white = TGAColor(255, 255, 255, 255);
//const TGAColor red   = TGAColor(255, 0,   0,   255);
Model *model = NULL;
const int width  = 800;
const int height = 800;
const int profondeur = 800;
/** Fin Code Base **/


/**********                  Code Perso                        *************/


void intervertir(int *v0,int *v1)
{
    *v0 -= *v1;
    *v1 += *v0;
    *v0 -= *v1;
    *v0 = -*v0;
}

int produitVectoriel(int xA, int yA, int xB, int yB,int xC,int yC)
{
    return (xA-xC)*(yB-yC)-(yA-yC)*(xB-xC);
}


void line(Vec3f *v0, Vec3f *v1,PpmImage *image)
{

    /** Ici on adapte notre maillage a notre image tout en la centrant **/
    int x0 = (v0->x+1.)*width/2.;
    int y0 = (v0->y+1.)*height/2.;
    int x1 = (v1->x+1.)*width/2.;
    int y1 = (v1->y+1.)*height/2.;

    bool horizontal = true;
    float e;//valeur de l'erreur
    float de;//corespond aux raport de la difference des ordo sur la diference des absi (valeur absolu)
    int x,y;
    /*** On veut savoir si la difference des abscice (valeur absolu) est superieur ou non a la difference des abscisse (valeur absolue) pour faire une boucle en consequence ***/

    if(abs(x0-x1) < abs(y0-y1))
    {
        horizontal = false;
        /** Si la pente n'est pas dit horizontal (si elle vari plus verticalement que horizontalement )**/
        /** alors on inverse le repere pour simplifier le code **/

        intervertir(&x0,&y0);
        intervertir(&x1,&y1);
    }

    if(x0 > x1)// si x0 < x1 alors pour aller dans le bon sens on les inverse
    {
        intervertir(&x0,&x1);
        intervertir(&y0,&y1);
    }
    de = abs((float)(y1-y0)/(float)(x1-x0));
    e = 0;
    y = y0;

    for(x = x0; x <= x1 ; x++)
    {
        if(horizontal)
        {
            image->setRGB(x,y,255,255,255);
        }
        else
        {
            image->setRGB(y,x,255,255,255);
        }
        e += de;
        if(e > 0.5)
        {
            if(y0 > y1)//si y0 > y1  il faut logiquement dessendre
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

bool estDansTriangle(int x0,int y0,int x1,int y1,int x2,int y2,int xM,int yM)
{
    if(produitVectoriel(x0,y0,x1,y1,xM,yM) < 0)
    {
        return false;
    }
    if(produitVectoriel(x1,y1,x2,y2,xM,yM) < 0)
    {
        return false;
    }
    if(produitVectoriel(x2,y2,x0,y0,xM,yM) < 0)
    {
        return false;
    }
    return true;
}

void triangle(Vec3f *v0, Vec3f *v1,Vec3f *v2,PpmImage *image)
{
    /** Ici on adapte notre maillage a notre image tout en la centrant **/
    int x0 = (v0->x+1.)*width/2.;
    int y0 = (v0->y+1.)*height/2.;
    int x1 = (v1->x+1.)*width/2.;
    int y1 = (v1->y+1.)*height/2.;
    int x2 = (v2->x+1.)*width/2.;
    int y2 = (v2->y+1.)*height/2.;


    int xMax = x0,xMin=x0,yMin=y0,yMax=y0;



    /** on va vouloir trouver les coordonnées x et y minimal et maximal des 3 sommet **/

    if(x0 > x1 )
    {
        xMin = x1;
    }else
    {
        xMax = x1;
    }

    if(xMin > x2)
    {
        xMin = x2;
    }
    if(xMax < x2)
    {
        xMax = x2;
    }


    if(y0 > y1 )
    {
        yMin = y1;
    }else
    {
        yMax = y1;
    }

    if(yMin > y2)
    {
        yMin = y2;
    }
    if(yMax < y2)
    {
        yMax = y2;
    }

    /*** On parcour donc la zone delimitee ***/
    bool dejColor;
    for(int y = yMin; y <= yMax ; y++)
    {
        dejColor = false;
        for(int x = xMin; x <= xMax ; x++)
        {
            if( estDansTriangle(x0,y0,x1,y1,x2,y2,x,y) )
            {
                image->setRGB(x,y,255,255,255);
                dejColor = true;
            }
            else
            {
                if(dejColor)//On a colorier et on arrive a un pixel non colorier donc on peut sauter tout les autre etape de la boucle actu
                {
                    break;
                }
            }
        }
    }
}


int main(int argc, char** argv)
{

    if (2==argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("obj/diablo3_pose.obj");
    }

    PpmImage *image =  new PpmImage(width, height);
    for (int i=0; i<model->nfaces(); i++)
    {
        vector<int> face = model->face(i);
        /*for (int j=0; j<3; j++)
        {
            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j+1)%3]);

            line(&v0, &v1, image);
        }*/
        Vec3f v0 = model->vert(face[0]);
        Vec3f v1 = model->vert(face[1]);
        Vec3f v2 = model->vert(face[2]);

        triangle(&v0, &v1,&v2,image);
    }

    //image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image->creeImage();

    delete image;
    delete model;
    return 0;
}
/**********                  Fin Code Perso                        *************/

/** Code de Test du donner dans le sujet **/


