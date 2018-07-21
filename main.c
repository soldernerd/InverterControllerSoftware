/*
 * File:   main.c
 * Author: Luke
 *
 * Created on 21. July 2018, 22:30
 */

/** INCLUDES *******************************************************/


#include <xc.h>
#include <stdint.h>

#include "config.h"
#include "display.h"

/*
 * Function prototypes
 */

void system_init(void);

void main(void)
{ 
    uint8_t cntr;

    //This is a user defined function
    system_init();
    
    RELAY1_PIN = 0;
    RELAY2_PIN = 0;
    RELAY3_PIN = 0;

    display_set(8901);
    

    while(1)
    {
        
        display_update(cntr);
        ++cntr;

//        if(!os.done)
//        { 
//
//            //Take care of beep
//            if(os.beep_count)
//            {
//                --os.beep_count;
//                if(!os.beep_count)
//                {
//                    BUZZER_PIN = 0;
//                }
//            }
//            
//            //Run periodic tasks
//            switch(os.timeSlot&0b00001111)
//            {       
//                case 0: 
//                    break;
//                case 8:
//                    //PORTBbits.RB3 = 1;
//                    break;
//            }
//            os.done = 1;
//        }
    }//end while(1)
    
}//end main

void system_init(void)
{
    RELAY1_TRIS = PIN_OUTPUT;
    RELAY2_TRIS = PIN_OUTPUT;
    RELAY3_TRIS = PIN_OUTPUT;
    
    EXTERNAL_ENABLE_TRIS = PIN_INPUT;
    EXTERNAL_SPINDLE_TRIS = PIN_INPUT;
    EXTERNAL_REVERSE_TRIS = PIN_INPUT;
    
    DISPLAY_A_TRIS = PIN_OUTPUT;
    DISPLAY_B_TRIS = PIN_OUTPUT;
    DISPLAY_C_TRIS = PIN_OUTPUT;
    DISPLAY_D_TRIS = PIN_OUTPUT;
    DISPLAY_E_TRIS = PIN_OUTPUT;
    DISPLAY_F_TRIS = PIN_OUTPUT;
    DISPLAY_G_TRIS = PIN_OUTPUT;
    DISPLAY_DP_TRIS = PIN_OUTPUT;
    DISPLAY_SEG1_TRIS = PIN_OUTPUT;
    DISPLAY_SEG2_TRIS = PIN_OUTPUT;
    DISPLAY_SEG3_TRIS = PIN_OUTPUT;
    DISPLAY_SEG4_TRIS = PIN_OUTPUT;
            
    FORWARD_TRIS = PIN_INPUT;
    REVERSE_TRIS = PIN_INPUT;
    ENCODER_A_TRIS = PIN_INPUT;
    ENCODER_B_TRIS = PIN_INPUT;
}

