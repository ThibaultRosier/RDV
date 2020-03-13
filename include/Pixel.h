#ifndef PIXEL_H
#define PIXEL_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

class Pixel
{
    public:
        Pixel();

        setRGB(unsigned int r,unsigned int g, unsigned int b);
        unsigned int getR();
        unsigned int getG();
        unsigned int getB();

        virtual ~Pixel();

    protected:

    private:
        unsigned int r;
        unsigned int g;
        unsigned int b;
};

#endif // PIXEL_H
