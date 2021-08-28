
/*
     ______                 __                 _______                              
    |   __ \.-----.-----.--|  |.-----.----.   |     __|.----.----.-----.-----.-----.
    |      <|  -__|     |  _  ||  -__|   _|   |__     ||  __|   _|  -__|  -__|     |
    |___|__||_____|__|__|_____||_____|__|     |_______||____|__| |_____|_____|__|__|

*/

void spritesToLine(std::uint32_t y, std::uint8_t* line){

    if(spritesOnLine[y]==0) return;

    auto *scanLine = &line[0];
    
    if(spriteCount>=0){
        uint8_t offset = 0;
        for(uint8_t spriteIndex = 1; spriteIndex<spriteCount; spriteIndex++){
            auto & sprite = sprites[spriteIndex];
            if(y >= sprite.y && y < sprite.y + sprite.imageData[1]){
                if(sprite.x>-sprite.imageData[0] && sprite.x<PROJ_LCDWIDTH){
                    switch(sprite.bit){
                        case 16:{
                            // untested, I don't have any 16bit sprites
                            sprite.offset = 2+(sprite.imageData[0] * (y-sprite.y));
                            if(sprite.hFlip){
                                for(offset=0; offset < sprite.imageData[0]; offset++){
                                    int pixPos = (sprite.imageData[0] + sprite.x - offset);
                                    if(sprite.imageData[sprite.offset]){
                                        // we |1 to the colour value to make sure it isn't transparent
                                        scanLine[PRESCAN + pixPos] = sprite.imageData[sprite.offset] | 1;
                                    }
                                    sprite.offset++;
                                }
                            }else{
                                for(offset=0; offset < sprite.imageData[0]; offset++){
                                    int pixPos = sprite.x + offset;
                                    if(sprite.imageData[sprite.offset]){
                                        scanLine[PRESCAN + pixPos] = sprite.imageData[sprite.offset] | 1;
                                    }
                                    sprite.offset++;
                                }
                            }
                            break;
                        } // case 16
                        case 8:{
                            sprite.offset = 2+(sprite.imageData[0] * (y-sprite.y));
                            if(sprite.hFlip){
                                for(offset=0; offset < sprite.imageData[0]; offset++){
                                    int pixPos = sprite.imageData[0] + sprite.x - offset;
                                    if(sprite.imageData[sprite.offset]){
                                        // we |1 to the colour value to make sure it isn't transparent
                                        scanLine[PRESCAN + pixPos] = sprite.imageData[sprite.offset] | 1;
                                    }
                                    sprite.offset++;
                                }
                            }else{
                                for(offset=0; offset < sprite.imageData[0]; offset++){
                                    int pixPos = sprite.x + offset;
                                    if(sprite.imageData[sprite.offset]){
                                        scanLine[PRESCAN + pixPos] = sprite.imageData[sprite.offset] | 1;
                                    }
                                    sprite.offset++;
                                }
                            }
                            break;
                        } // case 8
                        case 4:{
                            sprite.offset = 2+((sprite.imageData[0]/2) * (y-sprite.y));
                            if(sprite.hFlip){
                                for(offset=0; offset < sprite.imageData[0];){
                                    int pixPos = sprite.imageData[0] + sprite.x - offset;
                                    if((sprite.imageData[sprite.offset]>>4)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[sprite.imageData[sprite.offset]>>4] | 1;
                                    }
                                    offset++;
                                    pixPos--;
                                    if((sprite.imageData[sprite.offset]&15)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[sprite.imageData[sprite.offset]&15] | 1;
                                    }
                                    sprite.offset++;
                                    offset++;
                                }
                            }else{
                                for(offset=0; offset < sprite.imageData[0];){
                                    int pixPos = sprite.x + offset;
                                    if((sprite.imageData[sprite.offset]>>4)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[sprite.imageData[sprite.offset]>>4] | 1;
                                    }
                                    offset++;
                                    pixPos++;
                                    if((sprite.imageData[sprite.offset]&15)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[sprite.imageData[sprite.offset]&15] | 1;
                                    }
                                    sprite.offset++;
                                    offset++;
                                }
                            }
                            break;
                        } // case 4
                        case 2:{
                            sprite.offset = 2+((sprite.imageData[0]/4) * (y-sprite.y));
                            if(sprite.hFlip){
                                for(offset=0; offset < sprite.imageData[0];){
                                    int pixPos = sprite.imageData[0] + sprite.x - offset;
                                    if((sprite.imageData[sprite.offset]>>6)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[sprite.imageData[sprite.offset]>>6] | 1;
                                    }
                                    offset++;
                                    pixPos--;
                                    if(((sprite.imageData[sprite.offset]>>4)&3)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>4)&3] | 1;
                                    }
                                    pixPos--;
                                    offset++;
                                    if(((sprite.imageData[sprite.offset]>>2)&3)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>2)&3] | 1;
                                    }
                                    offset++;
                                    pixPos--;
                                    if((sprite.imageData[sprite.offset]&3)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[sprite.imageData[sprite.offset]&3] | 1;
                                    }
                                    sprite.offset++;
                                    offset++;
                                }
                            }else{
                                for(offset=0; offset < sprite.imageData[0];){
                                    int pixPos = sprite.x + offset;
                                    if((sprite.imageData[sprite.offset]>>6)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[sprite.imageData[sprite.offset]>>6] | 1;
                                    }
                                    offset++;
                                    pixPos++;
                                    if(((sprite.imageData[sprite.offset]>>4)&3)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>4)&3] | 1;
                                    }
                                    offset++;
                                    pixPos++;
                                    if(((sprite.imageData[sprite.offset]>>2)&3)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>2)&3] | 1;
                                    }
                                    offset++;
                                    pixPos++;
                                    if((sprite.imageData[sprite.offset]&3)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[sprite.imageData[sprite.offset]&3] | 1;
                                    }
                                    sprite.offset++;
                                    offset++;
                                }
                            }
                            break;
                        } // case 2
                        case 1:{
                            sprite.offset = 2+((sprite.imageData[0]/8) * (y-sprite.y));
                            if(sprite.hFlip){
                                for(offset=0; offset < sprite.imageData[0];){
                                    int pixPos = sprite.imageData[0] + sprite.x - offset;
                                    if((sprite.imageData[sprite.offset]>>7)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[sprite.imageData[sprite.offset]>>7] | 1;
                                    }
                                    offset++;
                                    pixPos--;
                                    if(((sprite.imageData[sprite.offset]>>6)&1)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>6)&1] | 1;
                                    }
                                    offset++;
                                    pixPos--;
                                    if(((sprite.imageData[sprite.offset]>>5)&1)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>5)&1] | 1;
                                    }
                                    offset++;
                                    pixPos--;
                                    if(((sprite.imageData[sprite.offset]>>4)&1)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>4)&1] | 1;
                                    }
                                    offset++;
                                    pixPos--;
                                    if(((sprite.imageData[sprite.offset]>>3)&1)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>3)&1] | 1;
                                    }
                                    offset++;
                                    pixPos--;
                                    if(((sprite.imageData[sprite.offset]>>2)&1)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>2)&1] | 1;
                                    }
                                    offset++;
                                    pixPos--;
                                    if(((sprite.imageData[sprite.offset]>>1)&1)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>1)&1] | 1;
                                    }
                                    offset++;
                                    pixPos--;
                                    if(((sprite.imageData[sprite.offset])&1)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset])&1] | 1;
                                    }
                                    sprite.offset++;
                                    offset++;
                                }
                            }else{
                                for(offset=0; offset < sprite.imageData[0];){
                                    int pixPos = sprite.x + offset;
                                    if((sprite.imageData[sprite.offset]>>7)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[sprite.imageData[sprite.offset]>>7] | 1;
                                    }
                                    offset++;
                                    pixPos++;
                                    if(((sprite.imageData[sprite.offset]>>6)&1)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>6)&1] | 1;
                                    }
                                    offset++;
                                    pixPos++;
                                    if(((sprite.imageData[sprite.offset]>>5)&1)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>5)&1] | 1;
                                    }
                                    offset++;
                                    pixPos++;
                                    if(((sprite.imageData[sprite.offset]>>4)&1)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>4)&1] | 1;
                                    }
                                    offset++;
                                    pixPos++;
                                    if(((sprite.imageData[sprite.offset]>>3)&1)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>3)&1] | 1;
                                    }
                                    offset++;
                                    pixPos++;
                                    if(((sprite.imageData[sprite.offset]>>2)&1)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>2)&1] | 1;
                                    }
                                    offset++;
                                    pixPos++;
                                    if(((sprite.imageData[sprite.offset]>>1)&1)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset]>>1)&1] | 1;
                                    }
                                    offset++;
                                    pixPos++;
                                    if(((sprite.imageData[sprite.offset])&1)){
                                        scanLine[PRESCAN + pixPos] = sprite.paletteData[(sprite.imageData[sprite.offset])&1] | 1;
                                    }
                                    sprite.offset++;
                                    offset++;
                                }
                            }
                            break;
                        } // case 1
                    }
                } // if X
            } // if Y
        } // for spriteCount
    } // sprite count >1
}

void spriteFill(std::uint8_t* line, std::uint32_t y, bool skip){
    //spritesToLine(y); // even without rendering ANY sprites this is very slow.
    return;
}

inline void myBGFiller16bit(std::uint8_t* line, std::uint32_t y, bool skip){

    // my Background filer, doesn't take into account scrolling or anything at all
    using PD=Pokitto::Display;

    if(skip) return;
    if(y==0){
        for(int x=0; x<220; x++)
            line[x]=x;
    }
/*
    // set bgcolor different for every line
//    Pokitto::Display::palette[0] = hline_pal[hline[(y+(mapY/4))]];

    int stX = -(mapX&7);
    uint32_t x = stX;
    uint32_t tileIndex = (mapX>>3) + ((y+mapY)>>3) * map[0];
    uint16_t jStart = ((y+mapY)&7) << 3; // current line in current tile

    uint32_t tileStart;
    uint32_t isFlipped;

    auto Pal = &Pokitto::Display::palette[0];
    auto Map = &map[2];
    auto MapPal = &map_pal[0];

    #define bgTileLine()\
        isFlipped = (Map[tileIndex]>>15);\
        tileStart = jStart + (Map[tileIndex++]&32767)*tbt;\
        if(isFlipped){\
            tileStart += 7;\
            for(int b=-8; b; b++){\
                *Pal++ = MapPal[tiles[tileStart--]];\
            }\
        }else{\
            for(int b=-8; b; b++){\
                *Pal++ = MapPal[tiles[tileStart++]];\
            }\
        }        

    #define bgHalfTile()\
        isFlipped = Map[tileIndex]>>15;\
        tileStart = jStart + (Map[tileIndex++]&32767)*tbt;\
        if(isFlipped){\
            tileStart += 7;\
            for(int b=-4; b; b++){\
                *Pal++ = MapPal[tiles[tileStart--]];\
            }\
        }else{\
            for(int b=-4; b; b++){\
                *Pal++ = MapPal[tiles[tileStart++]];\
            }\
        }

    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgHalfTile();

    spritesToLine(y); // even without rendering ANY sprites this is very slow.
*/
}

inline void myBGFiller8bit(std::uint8_t* line, std::uint32_t y, bool skip){

    // my Background filer, doesn't take into account scrolling or anything at all
    using PD=Pokitto::Display;

    if(skip) return;
    if(y==0){
        for(int x=0; x<220; x++)
            line[x]=x;
    }

    // set bgcolor different for every line
//    Pokitto::Display::palette[0] = hline_pal[hline[(y+(mapY/4))]];

    int stX = 0;
    uint32_t x = stX;
    uint32_t tileIndex = (y/bgTileSizeH) * background_map[0];
    uint16_t jStart = (y %bgTileSizeH) * bgTileSizeW; // current line in current tile

    uint32_t tileStart;
    uint32_t isFlipped;

    auto Map = &globalMap[0];
    auto Tile = &globalTile[0];
    auto Line = &line[0];


    #define bgTileLine()\
        isFlipped = Map[tileIndex]>>15;\
        tileStart = jStart + (Map[tileIndex++]&32767)*tbt;\
        if(isFlipped){\
            tileStart += 7;\
            for(int b=-8; b; b++){\
                *Line++ = Tile[tileStart--];\
            }\
        }else{\
            for(int b=-8; b; b++){\
                *Line++ = Tile[tileStart++];\
            }\
        }        

    #define bgHalfTile()\
        isFlipped = Map[tileIndex]>>15;\
        tileStart = jStart + (Map[tileIndex++]&32767)*tbt;\
        if(isFlipped){\
            tileStart += 7;\
            for(int b=-4; b; b++){\
                *Line++ = Tile[tileStart--];\
            }\
        }else{\
            for(int b=-4; b; b++){\
                *Line++ = Tile[tileStart++];\
            }\
        }

    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    bgTileLine(); bgTileLine(); bgTileLine(); bgTileLine();
    //bgHalfTile();

//    spritesToLine(y, line); // even without rendering ANY sprites this is very slow.

}
