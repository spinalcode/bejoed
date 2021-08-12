
#include <Pokitto.h>
#include <Tilemap.hpp>

#include <file>
#include "globals.h"

#include "font.h"
#include "font_2bit.h"
#include "background.h"
#include "buttonhandling.h"
#include "image.h"
#include "screen.h"

bool is8Bit = true;

void drawSprite(int x, int y, const uint8_t *imageData,const uint16_t *paletteData, bool hFlip, uint8_t bit){

    // if out of screen bounds, don't bother
    if(x<-22 || x+imageData[0]>241) return;
    if(y<-imageData[1] || y>175) return;

    if(++spriteCount>NUMSPRITES-1)spriteCount=NUMSPRITES-1; // don't overflow the sprite max

    sprites[spriteCount].x = x;
    sprites[spriteCount].y = y;
    sprites[spriteCount].imageData = imageData;
    sprites[spriteCount].paletteData = paletteData;
    sprites[spriteCount].hFlip = hFlip;
    sprites[spriteCount].bit = bit;
    
    for(int t=y; t<y+imageData[1]; t++){
        if(t>=0 && t<=176)
            spritesOnLine[t]++;
    }
    
}

// print text
void myPrint(char x, char y, const char* text) {
  uint8_t numChars = strlen(text);
  uint8_t x1 = 0;//2+x+28*y;
  for (uint8_t t = 0; t < numChars; t++) {
    uint8_t character = text[t] - 32;
    if(is8Bit){
        Pokitto::Display::drawSprite(x+((x1++)*8), y, font88[character]);
    }else{
        drawSprite(x+((x1++)*8), y,font_2bit[character], font_2bit_pal,0,2);
    }
  }
}

void setFPS(int fps){
  myDelay = 1000 / fps;
}

int main(){
    using PC=Pokitto::Core;
    using PD=Pokitto::Display;
    using PB=Pokitto::Buttons;

    PC::begin();
    PD::load565Palette(map_pal);
    PD::invisiblecolor = 0;
    PD::persistence = true;
    PD::adjustCharStep = 0;
    PD::adjustLineStep = 0;
    
    //setFPS(30);

    frameCount=0;

    if(is8Bit){
        PD::lineFillers[0] = myBGFiller8bit; // map layer
    }else{
        PD::lineFillers[0] = myBGFiller16bit; // map layer
    }
    //PD::lineFillers[3] = spriteFill; // sprite layer

    while( PC::isRunning() ){
        
        if( !PC::update() ) 
            continue;

        for(int t=0; t<176; t++){
            spritesOnLine[t]=0;
        }
            
        spriteCount = 0;
        updateButtons();

        if(_A[NEW]){
            is8Bit = 1-is8Bit;
            if(is8Bit){
                PD::lineFillers[0] = myBGFiller8bit; // map layer
                PD::load565Palette(map_pal);
            }else{
                PD::lineFillers[0] = myBGFiller16bit; // map layer
            }
        }

        char tempText[20];
        sprintf(tempText,"FPS:%d    ",fpsCount);
        myPrint(0,0,tempText);

        frameCount++;
        fpsCounter++;
        if(PC::getTime() >= lastMillis+1000){
            lastMillis = PC::getTime();
            fpsCount = fpsCounter;
            fpsCounter = 0;
        }

    }
    
    return 0;
}
