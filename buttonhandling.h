// 
//  I like to use this button handling routine, it tells me if
//  a button had just been pressed, held or released
// 
//------------------------[ Button handling, very accurate ]------------------------
#define HELD 0
#define NEW 1
#define RELEASED 2
uint8_t CompletePad, ExPad, TempPad, myPad;
bool _BA[3], _BB[3], _BC[3], _BUp[3], _BDown[3], _BLeft[3], _BRight[3];

void UPDATEPAD(int pad, int var) {
        _BC[pad] =      (var)&1;
        _BB[pad] = (var >> 1)&1;
        _BA[pad] = (var >> 2)&1;
     _BDown[pad] = (var >> 3)&1;
     _BLeft[pad] = (var >> 4)&1;
    _BRight[pad] = (var >> 5)&1;
       _BUp[pad] = (var >> 6)&1;
}

void UpdatePad(int joy_code){
    ExPad = CompletePad;
    CompletePad = joy_code;
    UPDATEPAD(HELD, CompletePad); // held
    UPDATEPAD(RELEASED, (ExPad & (~CompletePad))); // released
    UPDATEPAD(NEW, (CompletePad & (~ExPad))); // newpress
}

uint8_t updateButtons(){
    Pokitto::Buttons::update(); // update the current button states
    uint8_t var = 0;
    if (    Pokitto::Buttons::cBtn()) var |= 1;
    if (    Pokitto::Buttons::bBtn()) var |= (1<<1);
    if (    Pokitto::Buttons::aBtn()) var |= (1<<2);
    if ( Pokitto::Buttons::downBtn()) var |= (1<<3);
    if ( Pokitto::Buttons::leftBtn()) var |= (1<<4);
    if (Pokitto::Buttons::rightBtn()) var |= (1<<5);
    if (   Pokitto::Buttons::upBtn()) var |= (1<<6);
    UpdatePad(var);
    return var;
}
