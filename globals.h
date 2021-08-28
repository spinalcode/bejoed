int score=0;
int myScore=0;
int scoreAdder=0;
int lastBonus=0;
int gamemode=0;
int multiChain=0;
int playTime=0;

int oldAction=0;


uint8_t numberWidth[]={8,4,6,6,7,6,6,6,6,6};
uint8_t bigNumberWidth[]={16,7,11,11,13,12,12,10,11,12,9};

long int myDelay;
long int tempTime;
long int frameCount;
uint8_t fpsCount=0;
long int fpsCounter;
long int lastMillis;

// for my own sprite renderer
#define MAXSPRITES 128
struct SPRITE_DATA {
    const uint16_t *paletteData; // palette data
    const uint8_t *imageData; // image data
    int x;  // x postition
    int y;  // y position
    int hFlip;
    int offset; // tile render pixel offset
    uint8_t bit;
} sprites[MAXSPRITES];
int spritesOnLine[176]; // how many sprites on each line
int spriteCount = -1;

#define PRESCAN 0 // left side of scanline that is off screen
//uint16_t scanLine[396]; // there's an issue with screen garbage for some reason, so the buffer is WAY larger than it needs to be


struct JOE_DATA {
    int x;
    int y;
    int frame;
    int falling=0;
}joeSprite[8][8];

struct BOMB_DATA {
    int x;
    int y;
    int frame;
    int maxFrame = 8;
    bool used=false;
}bombSprite[8*8];
bool exploding = false;

#define GRIDTOP 16
#define GRIDLEFT 80

bool selecting = false;
#define GRIDSIZE 8
#define JOESIZE 16
signed int levelGrid[GRIDSIZE*GRIDSIZE];

int cursorX = 0;
int cursorY = 0;
int cursorSelectX = 0;
int cursorSelectY = 0;

uint8_t walkFrame[]={1,2,3,2};

int animCount1=0;
int animCount2=0;
int animCol1=0;
int animCol2=0;
int anim_x1=0;
int anim_x2=0;
int anim_y1=0;
int anim_y2=0;
int anim_time=0;
int animFrame1=0;
int animFrame2=0;
int animFlip1=0;
int animFlip2=0;
int animating=0;
int anim_d=0;
int action=0;
bool needsChecked=false;
