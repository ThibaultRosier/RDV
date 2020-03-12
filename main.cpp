#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

using namespace std;


/** Code de Base **/
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
Model *model = NULL;
const int width  = 800;
const int height = 800;
/** Fin Code Base **/


/**********                  Code Perso                        *************/


void intervertir(int *v0,int *v1)
{
    *v0 -= *v1;
    *v1 += *v0;
    *v0 -= *v1;
    *v0 = -*v0;
}

void line(Vec3f *v0, Vec3f *v1,TGAImage &image, TGAColor color)
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
            image.set(x,y,color);
        }
        else
        {
            image.set(y,x,color);
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


void triangle(Vec3f *v0, Vec3f *v1,Vec3f *v2,TGAImage &image, TGAColor color)
{
    /** Ici on adapte notre maillage a notre image tout en la centrant **/
    int x0 = (v0->x+1.)*width/2.;
    int y0 = (v0->y+1.)*height/2.;
    int x1 = (v1->x+1.)*width/2.;
    int y1 = (v1->y+1.)*height/2.;
    int x2 = (v2->x+1.)*width/2.;
    int y2 = (v2->y+1.)*height/2.;



    /** on va vouloir trouver les coordonnées x et y minimal et maximal des 3 sommet **/

    /*** Partie abscisse inf ***/
    if(x0 > x1 )
    {
        intervertir(&x0,&x1);
    }
    if(x0 > x2)
    {
        intervertir(&x0,&x2);
    }

    /** Partie abscisse supp **/
    if(x1 > x2)
    {
        intervertir(&x1,&x2);
    }

    /*** Partie ordo inf ***/
    if(y0 > y1 )
    {
        intervertir(&y0,&y1);
    }
    if(y0 > y2)
    {
        intervertir(&y0,&y2);
    }

    /** Partie ordo supp **/
    if(y1 > y2)
    {
        intervertir(&y1,&y2);
    }


    /*** On parcour donc la zone delimitee ***/
    for(int y = y0; y <= y2 ; y++)
    {
        for(int x = x0; x <= x2 ; x++)
        {
            image.set(x,y,color);
        }
    }



}
/**********                  Fin Code Perso                        *************/

/** Code de Test du donner dans le sujet **/

int main(int argc, char** argv) {
    if (2==argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("obj/diablo3_pose.obj");
    }

    TGAImage image(width, height, TGAImage::RGB);
    for (int i=0; i<model->nfaces(); i++)
    {
        vector<int> face = model->face(i);
        /*for (int j=0; j<3; j++)
        {
            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j+1)%3]);

            line(&v0, &v1, image, white);
        }*/
        Vec3f v0 = model->vert(face[0]);
        Vec3f v1 = model->vert(face[1]);
        Vec3f v2 = model->vert(face[2]);

        triangle(&v0, &v1,&v2,image,white );
    }

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    delete model;
    return 0;
}
