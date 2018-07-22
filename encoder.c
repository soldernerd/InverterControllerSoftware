
#include "encoder.h"
#include "config.h"

void encoder_init(void)
{
    //Configure timer1
    
    
    
    
    //Configure interrupts on change
    PIE0bits.IOCIE = 1; //Enable interrupts on change
    //Clear flags
    ENCODER_A_IOC_FLAG = 0;
    ENCODER_B_IOC_FLAG = 0;
    //Enable individual edges
    ENCODER_A_IOC_RISING = 1;
    ENCODER_A_IOC_FALLING = 1;
    ENCODER_B_IOC_RISING = 1;
    ENCODER_B_IOC_FALLING = 1;
}

inline void encoder_isr(void)
{
    if(ENCODER_A_IOC_FLAG)
    {
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
        ENCODER_A_IOC_FLAG = 0;
    }
    
    if(ENCODER_B_IOC_FLAG)
    {
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
        ENCODER_B_IOC_FLAG = 0;
    }
}
