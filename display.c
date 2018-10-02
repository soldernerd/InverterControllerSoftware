


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

#define CHAR_NONE 0x00
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

#define CHAR_H (SEG_TOPRIGHT | SEG_TOPLEFT | SEG_BOTTOMLEFT | SEG_BOTTOMRIGHT | SEG_CENTER)
#define CHAR_E (SEG_TOP | SEG_TOPLEFT | SEG_BOTTOMLEFT | SEG_BOTTOM | SEG_CENTER)
#define CHAR_L (SEG_TOPLEFT | SEG_BOTTOMLEFT | SEG_BOTTOM)
#define CHAR_O (SEG_TOP | SEG_TOPRIGHT | SEG_TOPLEFT | SEG_BOTTOMLEFT | SEG_BOTTOM | SEG_BOTTOMRIGHT)

static uint8_t char_lookup[10] = {CHAR_0, CHAR_1, CHAR_2, CHAR_3, CHAR_4, CHAR_5, CHAR_6, CHAR_7, CHAR_8, CHAR_9};
uint8_t characters[4];
volatile uint8_t segment;

void display_init(void)
{
    //Initialize segment counter
    segment = 0;
    
    //Initialize character arrray
    characters[0] = CHAR_0;
    characters[1] = CHAR_0;
    characters[2] = CHAR_0;
    characters[3] = CHAR_0;
    
    //Configure timer 2
    T2CLKCONbits.CS = 0b0001; //Clock source Fosc/4 (8MHz)
    T2CONbits.CKPS = 0b111; //Prescaler=128 (62.5kHz)
    PR2 = 125; // Period=125 (500Hz)
    T2CONbits.OUTPS = 0b0000; //Postscaler=1
    
    //Configure corresponding interrupts
    INTCONbits.GIE = 1; //Enable interrupts
    INTCONbits.PEIE = 1; //Enable peripheral interrupts
    PIE4bits.TMR2IE = 1; //Enable timer2 interrupts
    
    //Enable timer 2
    T2CONbits.ON = 1;
}

void display_set(int16_t value, uint8_t decimal_flags)
{
    uint16_t abs_value;
    uint8_t temp_chars[4];
    
    if(value<0)
    {
        abs_value = -value;
    }
    else
    {
        abs_value = value;
    }
    
    //Empty display
    temp_chars[3] = CHAR_NONE;
    temp_chars[2] = CHAR_NONE;
    temp_chars[1] = CHAR_NONE;
    temp_chars[0] = CHAR_NONE;
    
    // 4th digit
    temp_chars[3] = char_lookup[abs_value%10];
    abs_value /= 10;
    if(abs_value)
    {
        temp_chars[2] = char_lookup[abs_value%10];
        abs_value /= 10;
        if(abs_value)
        {
            temp_chars[1] = char_lookup[abs_value%10];
            abs_value /= 10;
            if(abs_value)
            {
                temp_chars[0] = char_lookup[abs_value%10];
            }
        }
    }

    //Use last decimal point as minus sign
    if(value<0 || (decimal_flags&DECIMAL_3))
    {
        temp_chars[3] |= SEG_POINT;
    }
    if(decimal_flags & DECIMAL_2)
    {
        temp_chars[2] |= SEG_POINT;
    }
    if(decimal_flags & DECIMAL_1)
    {
        temp_chars[1] |= SEG_POINT;
    }
    if(decimal_flags & DECIMAL_0)
    {
        temp_chars[0] |= SEG_POINT;
    }
    
    //Copy values
    characters[0] = temp_chars[0];
    characters[1] = temp_chars[1];
    characters[2] = temp_chars[2];
    characters[3] = temp_chars[3];
}

inline void display_isr(void)
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
    
    ++segment;
    
    //Clear interrupt flag
    PIR4bits.TMR2IF = 0;
}

void display_startup(void)
{
    uint8_t last_value;
    uint8_t position;
    uint16_t cntr;
    
    char tmp[12];
    tmp[0] = CHAR_NONE;
    tmp[1] = CHAR_NONE;
    tmp[2] = CHAR_NONE;
    tmp[3] = CHAR_H;
    tmp[4] = CHAR_E;
    tmp[5] = CHAR_L;
    tmp[6] = CHAR_L;
    tmp[7] = CHAR_O;
    tmp[8] = CHAR_NONE;
    tmp[9] = CHAR_NONE;
    tmp[10] = CHAR_NONE;
    tmp[11] = CHAR_NONE;

    for(position=0; position<9; ++position)
    {
        characters[0] = tmp[position+0];
        characters[1] = tmp[position+1];
        characters[2] = tmp[position+2];
        characters[3] = tmp[position+3];
        
        cntr = 0;
        last_value = segment;
        while(cntr<200)
        {
            if(segment!=last_value)
            {
                ++cntr;
                last_value = segment;
            }
        }
    }
}
