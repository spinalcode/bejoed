File file;

long int myDelay;
long int tempTime;
long int frameCount;
uint8_t fpsCount=0;
long int fpsCounter;
long int lastMillis;


int mapX=0;
int mapY=0;


// for my own sprite renderer
#define NUMSPRITES 48
struct SPRITE_DATA {
    const uint16_t *paletteData; // palette data
    const uint8_t *imageData; // image data
    int x;  // x postition
    int y;  // y position
    int hFlip;
    int offset; // tile render pixel offset
    uint8_t bit;
} sprites[NUMSPRITES];
int spritesOnLine[176]; // how many sprites on each line
int spriteCount = -1;

#define PRESCAN 0 // left side of scanline that is off screen
//uint16_t scanLine[396]; // there's an issue with screen garbage for some reason, so the buffer is WAY larger than it needs to be
