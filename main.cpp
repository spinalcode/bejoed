
#include <Pokitto.h>
#include <Tilemap.hpp>

#include <file>
#include "globals.h"

#include "font.h"
#include "font_2bit.h"
#include "background.h"
#include "buttonhandling.h"
#include "image.h"

auto globalMap = &background_map[2];
auto globalTile = &background_Tiles[0];

#include "screen.h"
#include "sprites.h"

bool is8Bit = true;

void drawSprite(int x, int y, const uint8_t *imageData,const uint16_t *paletteData, bool hFlip, uint8_t bit){

    // if out of screen bounds, don't bother
    if(x<-22 || x+imageData[0]>241) return;
    if(y<-imageData[1] || y>175) return;

    if(++spriteCount>MAXSPRITES-1)spriteCount=MAXSPRITES-1; // don't overflow the sprite max

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

void drawGrid(){
    for(int y=0; y<GRIDSIZE; y++){
        for(int x=0; x<GRIDSIZE; x++){
            if(joeSprite[x][y].frame >=0){
                // skip the currently animating sprites
                if(!(animating && ((x == cursorX && y == cursorY) || (x == cursorX+cursorSelectX && y == cursorY+cursorSelectY)))){
                    Pokitto::Display::drawSprite(GRIDLEFT+joeSprite[x][y].x, GRIDTOP+joeSprite[x][y].y, ball[joeSprite[x][y].falling + joeSprite[x][y].frame*7]);
                }
            }
            //char tempText[20];
            //sprintf(tempText,"%d",levelGrid[x+8*y]);
            //myPrint(72+joeSprite[x][y].x, 24+joeSprite[x][y].y,tempText);
        }
    }
    
    int numUsed=0;
    int animSpeed = 3;
    for(int t=0; t<64; t++){
        if(bombSprite[t].used==true){
            numUsed++;
            Pokitto::Display::drawSprite(GRIDLEFT+bombSprite[t].x*16, GRIDTOP+bombSprite[t].y*16, explode[bombSprite[t].frame/animSpeed]);
            bombSprite[t].frame++;
            if((bombSprite[t].frame/animSpeed)==bombSprite[t].maxFrame){
                bombSprite[t].frame=0;
                bombSprite[t].used=false;
                numUsed--;
            }
        }
    }
    if(numUsed<=0)exploding=false;
    
    Pokitto::Display::drawSprite(GRIDLEFT, 0, topBar);

    
}

void startExplosion(int x, int y){
    // assume a Joe is vanishing, so increase the score
    scoreAdder++;
    lastBonus++;

    int useSprite=0;
    for(int t=64; t; t--){
        if(bombSprite[t].used==false){useSprite=t;}
    }

    bombSprite[useSprite].x = x;
    bombSprite[useSprite].y = y;
    bombSprite[useSprite].frame = 0;
    bombSprite[useSprite].used=true;
    exploding = true;

}


int isStreakH(int x, int y){
    int currentJoe = levelGrid[x+GRIDSIZE*y];
    int streak = 0;
    int tx = x;
    while(tx>0 && levelGrid[(tx-1)+GRIDSIZE*y]==currentJoe){
        tx--;
        streak++;
    }
    tx = x;
    while(tx<GRIDSIZE-1 && levelGrid[(tx+1)+GRIDSIZE*y]==currentJoe){
        tx++;
        streak++;
    }
    //printf("H Streak Count:%d\n",streak);
    return streak;
}
int isStreakV(int x, int y){
    int currentJoe = levelGrid[x+GRIDSIZE*y];
    int streak = 0;
    int ty = y;
    while(ty>0 && levelGrid[x+GRIDSIZE*(ty-1)]==currentJoe){
        ty--;
        streak++;
    }
    ty = y;
    while(ty<GRIDSIZE-1 && levelGrid[x+GRIDSIZE*(ty+1)]==currentJoe){
        ty++;
        streak++;
    }
    //printf("V Streak Count:%d\n",streak);

    return streak;
}

void removeStreakH(int x, int y){
    //printf("removing  H streak\n");
    
    int currentJoe = levelGrid[x+GRIDSIZE*y];
    int tx = x;
    while(tx>0 && levelGrid[(tx-1)+GRIDSIZE*y]==currentJoe){
        tx--;
        if(levelGrid[tx+GRIDSIZE*y]!=-1){
            levelGrid[tx+GRIDSIZE*y]=-1;
            startExplosion(tx,y);
        }
    }
    tx = x;
    while(tx<GRIDSIZE-1 && levelGrid[(tx+1)+GRIDSIZE*y]==currentJoe){
        tx++;
        if(levelGrid[tx+GRIDSIZE*y]!=-1){
            levelGrid[tx+GRIDSIZE*y]=-1;
            startExplosion(tx,y);
        }
    }
    if(levelGrid[x+GRIDSIZE*y]!=-1){
        levelGrid[x+GRIDSIZE*y]=-1;
        startExplosion(x,y);
    }
}
void removeStreakV(int x, int y){
    //printf("removing  V streak\n");

    int currentJoe = levelGrid[x+GRIDSIZE*y];
    int streak = 0;
    int ty = y;
    while(ty>0 && levelGrid[x+GRIDSIZE*(ty-1)]==currentJoe){
        ty--;
        if(levelGrid[x+GRIDSIZE*ty]!=-1){
            levelGrid[x+GRIDSIZE*ty]=-1;
            startExplosion(x,ty);
        }
    }
    ty = y;
    while(ty<GRIDSIZE-1 && levelGrid[x+GRIDSIZE*(ty+1)]==currentJoe){
        ty++;
        if(levelGrid[x+GRIDSIZE*ty]!=-1){
            levelGrid[x+GRIDSIZE*ty]=-1;
            startExplosion(x,ty);
        }
    }
    if(levelGrid[x+GRIDSIZE*y]!=-1){
        levelGrid[x+GRIDSIZE*y]=-1;
        startExplosion(x,y);
    }
}


void animateLR(int x1, int y1, int x2, int y2){
    animCol1 = levelGrid[x1+GRIDSIZE*y1];
    animCol2 = levelGrid[x2+GRIDSIZE*y2];
    anim_x1 = GRIDLEFT+x1*JOESIZE;
    anim_x2 = GRIDLEFT+x2*JOESIZE;
    anim_y1 = GRIDTOP+y1*JOESIZE;
    anim_y2 = GRIDTOP+y2*JOESIZE;
    if(x1>x2)anim_d = 0;
    if(x2>x1)anim_d = 1;
    if(y1>y2)anim_d = 2;
    if(y2>y1)anim_d = 3;
    anim_time=0;
    animating==1;
    action++;
}

void copyGrid(){
    for(int y=0; y<8; y++){
        for(int x=0; x<8; x++){
            joeSprite[x][y].x = x*JOESIZE;
            joeSprite[x][y].y = y*JOESIZE;
            joeSprite[x][y].frame = levelGrid[x+GRIDSIZE*y];
        }
    }
}

void refill(){

    copyGrid();

    // for each column, check for joes and replace them with sprites.

    for(int x=0; x<GRIDSIZE; x++){
        int useSprite=GRIDSIZE-1;
        for(int y=GRIDSIZE-1; y>=0; y--){
            int number = 0;
            if(levelGrid[x+GRIDSIZE*y] != -1){
                joeSprite[x][useSprite].frame = levelGrid[x+GRIDSIZE*y];
                joeSprite[x][useSprite].y = y*JOESIZE;
                useSprite--;
            }
        }

        // if any sprites are left, put them above the grid and assign a random colour
        int t=-1;
        while(useSprite>=0){
            joeSprite[x][useSprite].y = t*JOESIZE;
            if(joeSprite[x][useSprite].y <0){
                joeSprite[x][useSprite].frame = random(6000)/1000; //levelGrid[x+GRIDSIZE*useSprite];
            }
            useSprite--;
            t--;
        }
    }
}

void checkAll(){
    needsChecked=false;
    lastBonus=0;
    for(int y=0; y<8; y++){
        for(int x=0; x<8; x++){
            if(isStreakH(x,y) > 1){
                removeStreakH(x,y);
            }else
            if(isStreakV(x,y) > 1){
                removeStreakV(x,y);
            }
        }
    }

    if(scoreAdder>0){
        lastBonus=scoreAdder;
    }
    refill();
}

void gameLogic(){
    
        if(action != 5){
            oldAction = action;
        }
        action = 5;
        
        if(exploding==false){
            bool neededToMove = false;
            for(int x=0; x<8; x++){
                for(int y=0; y<8; y++){
                    if(joeSprite[x][y].y < y*JOESIZE){
                        joeSprite[x][y].y++;
                        joeSprite[x][y].falling = 5;
                        neededToMove=true;
                        needsChecked = true;
                    }else{
                        joeSprite[x][y].falling = 0;
                        levelGrid[x+GRIDSIZE*y] = joeSprite[x][y].frame;
                    }
                }
            }
            if(neededToMove==false){
                action = oldAction;
            }
        }

        if(action==0){
            int oldCX = cursorX;
            int oldCY = cursorY;
            if(needsChecked==true){
                checkAll();
                multiChain++;
                lastBonus *= (multiChain+1);
            }
            if(_Left[NEW]){
                if(selecting){
                    if(cursorX>0 && cursorSelectY==0 && cursorSelectX>-1)cursorSelectX--;
                }else{
                    if(cursorX>0)cursorX--;
                }
            }else
            if(_Right[NEW]){
                if(selecting){
                    if(cursorX<7 && cursorSelectY==0 && cursorSelectX<1)cursorSelectX++;
                }else{
                    if(cursorX<7)cursorX++;
                }
            }else
            if(_Up[NEW]){
                if(selecting){
                    if(cursorY>0 && cursorSelectX==0 && cursorSelectY>-1)cursorSelectY--;
                }else{
                    if(cursorY>0)cursorY--;
                }
            }else
            if(_Down[NEW]){
                if(selecting){
                    if(cursorY<7 && cursorSelectX==0 && cursorSelectY<1)cursorSelectY++;
                }else{
                    if(cursorY<7)cursorY++;
                }
            }else

            if(_A[NEW]){
                selecting = 1-selecting;
                if(selecting==false){
                    if(cursorSelectX !=0 || cursorSelectY !=0){
                        animateLR(cursorX, cursorY, cursorX+cursorSelectX, cursorY+cursorSelectY);
                        animating=true;
                    }
                }
            }
            if(oldCX != cursorX || oldCY != cursorY){
                multiChain = 0;
            }
        }

        if(action==2){
            int temp = levelGrid[cursorX+8*cursorY];
            levelGrid[cursorX+8*cursorY] = levelGrid[(cursorX+cursorSelectX)+8*(cursorY+cursorSelectY)];
            levelGrid[(cursorX+cursorSelectX)+8*(cursorY+cursorSelectY)] = temp;
            if(isStreakH(cursorX,cursorY) > 1){
                //printf("H Streak:%d,%d\n",cursorX,cursorY);
                removeStreakH(cursorX,cursorY);
                animating=0;
                cursorSelectX=0;
                cursorSelectY=0;
                action=0;
                selecting=false;
            }else
            if(isStreakH(cursorX+cursorSelectX,cursorY+cursorSelectY) > 1){
                //printf("H Streak:%d,%d\n",cursorX+cursorSelectX,cursorY+cursorSelectY);
                removeStreakH(cursorX+cursorSelectX,cursorY+cursorSelectY);
                animating=0;
                cursorSelectX=0;
                cursorSelectY=0;
                action=0;
                selecting=false;
            }else
            if(isStreakV(cursorX,cursorY) > 1){
                //printf("V Streak:%d,%d\n",cursorX,cursorY);
                removeStreakV(cursorX,cursorY);
                animating=0;
                cursorSelectX=0;
                cursorSelectY=0;
                action=0;
                selecting=false;
            }else
            if(isStreakV(cursorX+cursorSelectX,cursorY+cursorSelectY) > 1){
                //printf("V Streak:%d,%d\n",cursorX+cursorSelectX,cursorY+cursorSelectY);
                removeStreakV(cursorX+cursorSelectX,cursorY+cursorSelectY);
                animating=0;
                cursorSelectX=0;
                cursorSelectY=0;
                action=0;
                selecting=false;
            }else{
                //printf("No Streak\n");
                animateLR(cursorX, cursorY, cursorX+cursorSelectX, cursorY+cursorSelectY);
                animating=true;
                int temp = levelGrid[cursorX+8*cursorY];
                levelGrid[cursorX+8*cursorY] = levelGrid[(cursorX+cursorSelectX)+8*(cursorY+cursorSelectY)];
                levelGrid[(cursorX+cursorSelectX)+8*(cursorY+cursorSelectY)] = temp;
            }
            checkAll();
        }

        drawGrid();

        if(action==0){
            // Cursor dawing
            if(cursorSelectY==0 && cursorSelectX==0){
                Pokitto::Display::drawSprite(GRIDLEFT+cursorX*JOESIZE, GRIDTOP+cursorY*JOESIZE, ball[6]);
            }else
            if(cursorSelectY==-1 && cursorSelectX==0){
                Pokitto::Display::drawSprite(GRIDLEFT+cursorX*JOESIZE, GRIDTOP+(cursorY-1)*JOESIZE, cursor[1]);
            }else
            if(cursorSelectY==1 && cursorSelectX==0){
                Pokitto::Display::drawSprite(GRIDLEFT+cursorX*JOESIZE, GRIDTOP+cursorY*JOESIZE, cursor[1]);
            }else
            if(cursorSelectY==0 && cursorSelectX==-1){
                Pokitto::Display::drawSprite(GRIDLEFT+(cursorX-1)*JOESIZE, GRIDTOP+cursorY*JOESIZE, cursor[0]);
            }else
            if(cursorSelectY==0 && cursorSelectX==1){
                Pokitto::Display::drawSprite(GRIDLEFT+cursorX*JOESIZE, GRIDTOP+cursorY*JOESIZE, cursor[0]);
            }
        }

        if(animating){
            // doing the animation to move the dots
            if(anim_d==0){
                anim_x1--;
                anim_x2++;
                animCount1 ++;
                if(animCount1==4){
                    animCount1=0;
                }
                animFrame1 = walkFrame[animCount1];
                animFrame2 = walkFrame[animCount1];
                animFlip1 = 1;
                animFlip2 = 0;
            }
            if(anim_d==1){
                anim_x1++;
                anim_x2--;
                animCount1 ++;
                if(animCount1==4){
                    animCount1=0;
                }
                animFrame1 = walkFrame[animCount1];
                animFrame2 = walkFrame[animCount1];
                animFlip1 = 0;
                animFlip2 = 1;
            }
            if(anim_d==2){
                anim_y1--;
                anim_y2++;
                animFrame1 = 4;
                animFrame2 = 5;
            }
            if(anim_d==3){
                anim_y1++;
                anim_y2--;
                animFrame1 = 5;
                animFrame2 = 4;
            }

            Pokitto::Display::drawSprite(anim_x1, anim_y1, ball[animFrame1 + animCol1*7], 0, animFlip1);
            Pokitto::Display::drawSprite(anim_x2, anim_y2, ball[animFrame2 + animCol2*7], 0, animFlip2);

            if(anim_time < 15){
                anim_time++;
            }else{
                animating=0;
                action++;
                if(action==4){
                    cursorSelectX=0;
                    cursorSelectY=0;
                    action=0;
                }
            }
        }
        

        char tempText[20];
        sprintf(tempText,"FPS:%d    ",fpsCount);
        myPrint(0,0,tempText);

        //sprintf(tempText,"Chain:%d ",multiChain);
        //myPrint(0,8,tempText);

        if(frameCount%4==0){
            if(myScore<score)myScore++;
        }
        if(scoreAdder){
            score++;
            scoreAdder--;
        }
        
        //sprintf(tempText,"%d",myScore);
        //myPrint(11,48,tempText);
        
        int scoreX = 11;
        int scoreY = 48;
        int tempScore = myScore;
        int strWidth = 0;

        sprintf(tempText,"%d",myScore);

        for(int t=0; t<strlen(tempText); t++){
            int tempDigit = tempText[t]-48;
            strWidth+=numberWidth[tempDigit];
        }
        scoreX = 40-(strWidth/2);
        for(int t=0; t<strlen(tempText); t++){
            int tempDigit = tempText[t]-48;
            Pokitto::Display::drawSprite(scoreX, scoreY, score_number[tempDigit]);
            scoreX+=numberWidth[tempDigit];
        }

        // score adder
        if(lastBonus>0){
            scoreY = 102;
            sprintf(tempText,":%d",lastBonus);
            for(int t=0; t<strlen(tempText); t++){
                int tempDigit = tempText[t]-48;
                strWidth+=bigNumberWidth[tempDigit];
            }
            scoreX = 44-(strWidth/2);
            for(int t=0; t<strlen(tempText); t++){
                int tempDigit = tempText[t]-48;
                Pokitto::Display::drawSprite(scoreX, scoreY, bignumber[tempDigit]);
                scoreX+=bigNumberWidth[tempDigit];
            }
        }
    
}

void titleScreen(){
    //Pokitto::Display::drawSprite(0, 0, titlescreen);
    if(_A[NEW]){gamemode=10;}
}

int main(){
    using PC=Pokitto::Core;
    using PD=Pokitto::Display;
    using PB=Pokitto::Buttons;

    PC::begin();
    PD::load565Palette(background_Tiles_pal);
    PD::invisiblecolor = 0;
    PD::persistence = true;
    PD::adjustCharStep = 0;
    PD::adjustLineStep = 0;
    
    //PC::setFrameRate(5);
    //setFPS(30);

    frameCount=0;

    if(is8Bit){
        PD::lineFillers[0] = myBGFiller8bit; // map layer
    }else{
        PD::lineFillers[0] = myBGFiller16bit; // map layer
    }
    //PD::lineFillers[3] = spriteFill; // sprite layer

    for(int y=0; y<8; y++){
        for(int x=0; x<8; x++){
            levelGrid[x+8*y] = random(6000)/1000;
            while(isStreakV(x,y) || isStreakH(x,y)){
                levelGrid[x+8*y] = random(6000)/1000;
            }
        }
    }

    checkAll();

    copyGrid();
    selecting=0;

    updateButtons();
    while(_A[HELD] || _A[NEW]){
        updateButtons();
    }


    bool needsChecked = false;
    action=0;

    globalMap = &titlescreen_map[0];
    globalTile = &titlescreen_Tiles[0];

    while( PC::isRunning() ){
        
        if( !PC::update() ) 
            continue;

        updateButtons();

        int oldGameMode = gamemode;
        switch(gamemode){
            case 0:
                titleScreen();
                break;
            case 10:
                gameLogic();
                break;
        }

        if(gamemode != oldGameMode){
            switch(gamemode){
                case 0:
                    globalMap = &titlescreen_map[2];
                    globalTile = &titlescreen_Tiles[0];
                    break;
                case 10:
                    globalMap = &background_map[2];
                    globalTile = &background_Tiles[0];
                    break;
            }
            
        }

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
