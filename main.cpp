#include <iostream>
#include "swizzleUtility.h"

using namespace std;

int main(int argc, char* argv[])
{
    fipImage fip;
    if(argc == 1) {
        printf("ImageSwizzle [*.png]\n");
        return -1;
    }
    if(fip.load(argv[1], PNG_DEFAULT)) {
        if(fip.getBitsPerPixel() != 32)
            fip.convertTo32Bits();

        fip.rescale(fip.getWidth()/tileW*tileW, fip.getHeight()/tileH*tileH, FILTER_BILINEAR);

        int width = fip.getWidth();
        int height = fip.getHeight();

        fipImage fop(FIT_BITMAP, width, height, 32);
        swizzleARGB(fop.accessPixels(), fip.accessPixels(), width, height, SWIZZLE16x8);

        fipImage frec(FIT_BITMAP, width, height, 32);
        unswizzleARGB(frec.accessPixels(), fop.accessPixels(), width, height, SWIZZLE16x8);

        char swName[100] = {0};
        char recName[100] = {0};

        sprintf(swName, "%s.SW.png", argv[1]);
        fop.save(swName, PNG_DEFAULT);
        printf("Save %s successfull\n", swName);

        sprintf(recName, "%s.REC.png", argv[1]);
        frec.save(recName, PNG_DEFAULT);
        printf("Save %s successfull\n", recName);

    }
    else {
        printf("Load failed\n");
        return -2;
    }

    return 0;
}

