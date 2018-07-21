


#include "display.h"
#include "config.h"

#define SEG_CENTER 0b00000001
#define SEG_TOPRIGHT 0b00000010
#define SEG_TOP 0b00000100
#define SEG_TOPLEFT 0b00001000
#define SEG_BOTTOMLEFT 0b00010000
#define SEG_BOTTOM 0b00100000
#define SEG_POINT 0b01000000
#define SEG_BOTTOMRIGHT 0b10000000

#define CHAR_0 (SEG_TOP | SEG_TOPRIGHT | SEG_TOPLEFT | SEG_BOTTOMLEFT | SEG_BOTTOM | SEG_BOTTOMRIGHT)
#define CHAR_1 (SEG_TOPRIGHT | SEG_BOTTOMRIGHT)
#define CHAR_2 (SEG_TOP | SEG_TOPRIGHT | SEG_CENTER | SEG_BOTTOMLEFT | SEG_BOTTOM)
#define CHAR_3 (SEG_TOP | SEG_TOPRIGHT  | SEG_BOTTOM | SEG_BOTTOMRIGHT | SEG_CENTER)
#define CHAR_4 (SEG_TOPRIGHT | SEG_TOPLEFT | SEG_BOTTOMRIGHT | SEG_CENTER)
#define CHAR_5 (SEG_TOP | SEG_TOPLEFT | SEG_BOTTOM | SEG_BOTTOMRIGHT | SEG_CENTER)
#define CHAR_6 (SEG_TOP | SEG_TOPLEFT | SEG_BOTTOMLEFT | SEG_BOTTOM | SEG_BOTTOMRIGHT | SEG_CENTER)
#define CHAR_7 (SEG_TOP | SEG_TOPRIGHT | SEG_BOTTOMRIGHT)
#define CHAR_8 (SEG_TOP | SEG_TOPRIGHT | SEG_TOPLEFT | SEG_BOTTOMLEFT | SEG_BOTTOM | SEG_BOTTOMRIGHT | SEG_CENTER)
#define CHAR_9 (SEG_TOP | SEG_TOPRIGHT | SEG_TOPLEFT | SEG_BOTTOM | SEG_BOTTOMRIGHT | SEG_CENTER)

static uint8_t char_lookup[10] = {CHAR_0, CHAR_1, CHAR_2, CHAR_3, CHAR_4, CHAR_5, CHAR_6, CHAR_7, CHAR_8, CHAR_9};
uint8_t characters[4];

void display_set(uint16_t value)
{
    //Character 4
    characters[3] = char_lookup[value%10];
    //Character 3
    value /= 10;
    characters[2] = char_lookup[value%10];
    //Character 2
    value /= 10;
    characters[1] = char_lookup[value%10];
    //Character 1
    value /= 10;
    characters[0] = char_lookup[value%10];
}

inline void display_update(uint8_t segment)
{
    DISPLAY_SEG1_PIN = 1;
    DISPLAY_SEG2_PIN = 1;
    DISPLAY_SEG3_PIN = 1;
    DISPLAY_SEG4_PIN = 1; 
    
    LATC = characters[segment&0b11];
            
    switch(segment&0b11)
    {
        case 0:
            DISPLAY_SEG1_PIN = 0;
            break;
        case 1:
            DISPLAY_SEG2_PIN = 0;
            break;
        case 2:
            DISPLAY_SEG3_PIN = 0;
            break;
        case 3:
            DISPLAY_SEG4_PIN = 0; 
            break;
    }
}