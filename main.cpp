#include <iostream>
#include <FreeImagePlus.h>
#include <stdint.h>

using namespace std;

const bool isTile = true;
const uint32_t tileW = 16;
const uint32_t tileH = 8;

void swizzleARGB(BYTE *dst, const BYTE *src, int width, int height) {

    if(isTile) { //16x8 Tile
        uint32_t widthInTiles = (width + tileW-1) / tileW;
        const uint8_t *pSrcCur = src;
        uint8_t * pDstCur;
        for(int y = 0; y < height; ++y) {
            for(int x = 0; x < width; ++x) {
                uint32_t tileX = x / tileW;
                uint32_t tileY = y / tileH;
                uint32_t inTileX = x % tileW;
                uint32_t inTileY = y % tileH;

                pDstCur = dst +((tileY * widthInTiles + tileX) * (tileW * tileH)
                                      + inTileY * tileW
                                      + inTileX) * 4;
                pDstCur[0] = pSrcCur[0];
                pDstCur[1] = pSrcCur[1];
                pDstCur[2] = pSrcCur[2];
                pDstCur[3] = pSrcCur[3];

                pSrcCur += 4;
            }
        }
    }//16x8 Tile
    else {  //Complicated Swizzle

    }

}

void unswizzleARGB(BYTE *dst, const BYTE *src, int width, int height) {

    if(isTile) { //16x8 Tile
        uint32_t widthInTiles = (width + tileW-1) / tileW;

        const uint8_t * pSrcCur;
        uint8_t *pDstCur = dst;

        for(int y = 0; y < height; ++y) {
            for(int x = 0; x < width; ++x) {
                uint32_t tileX = x / tileW;
                uint32_t tileY = y / tileH;
                uint32_t inTileX = x % tileW;
                uint32_t inTileY = y % tileH;

                pSrcCur = src + ((tileY * widthInTiles + tileX) * (tileW * tileH)
                                      + inTileY * tileW
                                      + inTileX) * 4;

                pDstCur[0] = pSrcCur[0];
                pDstCur[1] = pSrcCur[1];
                pDstCur[2] = pSrcCur[2];
                pDstCur[3] = pSrcCur[3];

                pDstCur += 4;
            }
        }
    }//16x8 Tile

    else {

    }


}

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

        fip.crop(0, 0, fip.getWidth()/tileW*tileW, fip.getHeight()/tileH*tileH);

        int width = fip.getWidth();
        int height = fip.getHeight();

        fipImage fop(FIT_BITMAP, width, height, 32);
        swizzleARGB(fop.accessPixels(), fip.accessPixels(), width, height);

        fipImage frec(FIT_BITMAP, width, height, 32);
        unswizzleARGB(frec.accessPixels(), fop.accessPixels(), width, height);

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

