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
#include "encoder.h"

/*
 * Function prototypes
 */

static void _system_init(void);
static void _handle_relays(void);
static void _calculate_speed(void);


void main(void)
{ 
    uint16_t cntr;
    _system_init();
    os.encoder = 0;
    os.encoder_last = 0;
    os.encoder_difference = 0;
    

    while(1)
    {
        if(!os.done)
        { 
            _handle_relays();
            
            if(os.ready)
            {
                _calculate_speed();
                os.ready = 0;
                
                //Update display
                if(os.speed>0)
                {
                    display_set((uint16_t) os.speed);
                }
                else
                {
                    display_set((uint16_t) -os.speed);
                }
            }

            //Run periodic tasks
            switch(os.timeSlot&0b00001111)
            {       
                case 0: 
                    ++cntr;
                    break;
                case 8:
                    break;
            }
            os.done = 1;
        }
    }//end while(1)
    
}//end main


void interrupt _isr(void)
{
    //Interrupt on change: Encoder
//    if(PIR0bits.IOCIF)
//    {
//        encoder_isr();
//    }
    
    if(PIR6bits.CCP1IF || PIR6bits.CCP2IF)
    {
        capture_isr();
    }
    
    //Timer1 interrupt: Encoder
    if(PIR4bits.TMR1IF)
    {
        timer_1_isr();
    }
    
    //Timer2 interrupt: Display
    if(PIR4bits.TMR2IF)
    {
        //Update display
        display_update();
        //Update timeslot and clear done flag
        ++os.timeSlot;
        os.done = 0;
        //Clear interrupt flag
        PIR4bits.TMR2IF = 0;
    }
}

static void _system_init(void)
{
    RELAY1_TRIS = PIN_OUTPUT;
    RELAY2_TRIS = PIN_OUTPUT;
    RELAY3_TRIS = PIN_OUTPUT;
    
    EXTERNAL_ENABLE_TRIS = PIN_INPUT;
    EXTERNAL_ENABLE_ANSEL = PIN_DIGITAL;
    EXTERNAL_SPINDLE_TRIS = PIN_INPUT;
    EXTERNAL_SPINDLE_ANSEL = PIN_DIGITAL;
    EXTERNAL_REVERSE_TRIS = PIN_INPUT;
    EXTERNAL_REVERSE_ANSEL = PIN_DIGITAL;
    
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
    FORWARD_ANSEL = PIN_DIGITAL;
    REVERSE_TRIS = PIN_INPUT;
    REVERSE_ANSEL = PIN_DIGITAL;
    
    ENCODER_A_TRIS = PIN_INPUT;
    ENCODER_A_ANSEL = PIN_DIGITAL;
    ENCODER_B_TRIS = PIN_INPUT;
    ENCODER_B_ANSEL = PIN_DIGITAL;
    
    PPSUnLock();
    CCP1PPS = ENCODER_A_PPS_VALUE;
    CCP2PPS = ENCODER_B_PPS_VALUE;
    PPSLock();
    
    display_init();
    encoder_init();
    
}

static void _handle_relays(void)
{
    if(EXTERNAL_ENABLE_PIN)
    {
        //We are in remote control mode
        //Use external speed signal
        RELAY_SPEED = 1;
        if(EXTERNAL_SPINDLE_PIN)
        {
            //Take care of forward/reverse relays
            if(EXTERNAL_REVERSE_PIN)
            {
                RELAY_FORWARD = 0;
                RELAY_REVERSE = 1;
            }
            else
            {
                RELAY_REVERSE = 0; 
                RELAY_FORWARD = 1;
            }
        }
        else
        {
            //Turn motor off
            RELAY_FORWARD = 0;
            RELAY_REVERSE = 0;
        }
    }
    else
    {
        //We are in local control mode
        //Use external speed signal
        RELAY_SPEED = 0;
        if(FORWARD_PIN)
        {
            RELAY_REVERSE = 0; 
            RELAY_FORWARD = 1;
        }
        else if(REVERSE_PIN)
        {
            RELAY_FORWARD = 0;
            RELAY_REVERSE = 1;
        }
        else
        {
            RELAY_FORWARD = 0;
            RELAY_REVERSE = 0;
        }
    }
}

static void _calculate_speed(void)
{
    uint32_t distance;
    uint32_t time_difference;
    float speed;

    if(os.encoder_difference>64)
    {
        distance = (uint32_t) os.encoder_difference;
        distance *= 60; //Seconds per minute

        time_difference = os.time_sum - os.time_sum_last;
        //Account for averaging
        time_difference += 16;
        time_difference >>= 5; 

        speed = (float) distance;
        speed /= time_difference;
        speed /= 256; //Pulses per rotation
        speed *= 8000000.0; //Timer frequency

        os.speed = (int32_t) speed;  
    }
    else
    {
        os.speed = 0;
    }
}
