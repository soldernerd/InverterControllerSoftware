
#include "encoder.h"
#include "config.h"

uint32_t time;
uint8_t rollover_cnt;

void encoder_init(void)
{
    rollover_cnt = 0;
    
    os.encoder = 0;
    os.encoder_last = 0;
    os.encoder_difference = 0;
    os.capture = 0;
    os.ready = 0;
    os.time_sum_last = 0;
    os.time_sum = 0;
    os.speed = 0;
    
    //Configure timer1
    T1CLKbits.CS = 0b0001; //Clock Source = Fosc/4, i.e 8 MHz
    T1CONbits.CKPS = 0b00; //Prescaler=1
    T1CONbits.RD16 = 1; //Enable buffered 16bit read
    T1CONbits.ON = 1; //Enable timer
    
    //Configure interrupts on timer 1
    INTCONbits.GIE = 1; //Enable interrupts
    INTCONbits.PEIE = 1; //Enable peripheral interrupts
    PIE4bits.TMR1IE = 1; //Enable timer1 interrupts
    
    //Configure capture module 1
    CCP1CONbits.MODE = 0b0011; //Capture every edge
    CCP1CAPbits.CTS = 0b000; //Input = CCP1PPS
    CCP1CONbits.EN = 1; //Enable CCP1 module
    
    //Configure capture module 2
    CCP2CONbits.MODE = 0b0011; //Capture every edge
    CCP2CAPbits.CTS = 0b000; //Input = CCP2PPS
    CCP2CONbits.EN = 1; //Enable CCP2 module
    
    //Enable capture interrupts
    INTCONbits.GIE = 1; //Enable interrupts
    INTCONbits.PEIE = 1; //Enable peripheral interrupts
    PIR6bits.CCP1IF = 0; //Clear interrupt flag A
    PIE6bits.CCP1IE = 1; //Enable CCP1 interrupts
    PIR6bits.CCP2IF = 0; //Clear interrupt flag B
    PIE6bits.CCP2IE = 1; //Enable CCP2 interrupts
}

inline void timer_1_isr(void)
{
    ++rollover_cnt;
    
    //Time for next measurement
    if((rollover_cnt&0b11111) == 0)
    {
        //Correct for rollover
        if(rollover_cnt==0)
        {
            os.time_sum -= 536870912; //(256*32)<<16
        }
        //Record number of pulses since last observation
        os.encoder_difference = os.encoder - os.encoder_last;
        os.encoder_last = os.encoder;
        //Capture time
        os.capture = 32;  
        os.time_sum_last = os.time_sum;
        os.time_sum = 0;
    }
    
    
    //Check if there is still an measurement in progress
    if((rollover_cnt&0b11111) == 0b11111)
    {
        //Cancel any ongoing measurement
        if(os.capture!=0)
        {
            //Stop capturing time data
            os.capture = 0;
            //Indicate that the current time_sum is invalid
            os.time_sum = 0;
            //Indicate that new data is available
            os.ready = 1;
        }
    }
    
    //Clear interrupt flag
    PIR4bits.TMR1IF = 0;
}

inline void capture_A_isr(void)
{
    //Capture time if necessary
    if(os.capture)
    {
        //Calculate 32-bit time
        time = rollover_cnt;
        time <<= 8;
        time |= CCPR1H;
        time <<= 8;
        time |= CCPR1L;
        //Add time to time sum
        os.time_sum += time;
        //Decrement counter
        --os.capture;
        //Set ready flag if we are done
        if(os.capture==0)
        {
            os.ready = 1;
        }
    }

    if(ENCODER_A_PIN)
    {
        if(ENCODER_B_PIN)
        {
            #ifdef INVERSE_DIRECTION
                ++os.encoder;
            #else
                --os.encoder;
            #endif
        }
        else
        {
            #ifdef INVERSE_DIRECTION
                --os.encoder;
            #else
                ++os.encoder;
            #endif
        }
    }
    else
    {
        if(ENCODER_B_PIN)
        {
            #ifdef INVERSE_DIRECTION
                --os.encoder;
            #else
                ++os.encoder;
            #endif
        }
        else
        {
            #ifdef INVERSE_DIRECTION
                ++os.encoder;
            #else
                --os.encoder;
            #endif
        }
    }
    PIR6bits.CCP1IF = 0;
}
 

inline void capture_B_isr(void)
{
    //Capture time if necessary
    if(os.capture)
    {
        //Calculate 32-bit time
        time = rollover_cnt;
        time <<= 8;
        time |= CCPR2H;
        time <<= 8;
        time |= CCPR2L;
        //Add time to time sum
        os.time_sum += time;
        //Decrement counter
        --os.capture;
        //Set ready flag if we are done
        if(os.capture==0)
        {
            os.ready = 1;
        }
    }

    if(ENCODER_B_PIN)
    {
        if(ENCODER_A_PIN)
        {
            #ifdef INVERSE_DIRECTION
                --os.encoder;
            #else
                ++os.encoder;
            #endif
        }
        else
        {
            #ifdef INVERSE_DIRECTION
                ++os.encoder;
            #else
                --os.encoder;
            #endif
        }
    }
    else
    {
        if(ENCODER_A_PIN)
        {
            #ifdef INVERSE_DIRECTION
                ++os.encoder;
            #else
                --os.encoder;
            #endif
        }
        else
        {
            #ifdef INVERSE_DIRECTION
                --os.encoder;
            #else
                ++os.encoder;
            #endif
        }
    }
    PIR6bits.CCP2IF = 0;
}
