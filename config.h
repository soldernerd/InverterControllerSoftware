/* 
 * File:   system.h
 * Author: Luke
 *
 * Created on 5. September 2016, 21:17
 */

#ifndef OS_H
#define	OS_H

#include <stdint.h>

/*
 * Configuration bits
 */

// CONFIG1
#pragma config FEXTOSC = HS     // External Oscillator mode selection bits (HS (crystal oscillator) above 4MHz; PFM set to high power)
#pragma config RSTOSC = EXT4X   // Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)
#pragma config CLKOUTEN = OFF   // Clock Out Enable bit (CLKOUT function is disabled; i/o or oscillator function on OSC2)
#pragma config CSWEN = ON      // Clock Switch Enable bit (The NOSC and NDIV bits cannot be changed by user software)
#pragma config FCMEN = OFF       // Fail-Safe Clock Monitor Enable bit (FSCM timer enabled)

// CONFIG2
#pragma config MCLRE = ON       // Master Clear Enable bit (MCLR pin is Master Clear function)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config LPBOREN = OFF    // Low-Power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = ON       // Brown-out reset enable bits (Brown-out Reset Enabled, SBOREN bit is ignored)
#pragma config BORV = HI        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (VBOR) is set to 2.7V)
#pragma config ZCD = OFF        // Zero-cross detect disable (Zero-cross detect circuit is disabled at POR.)
#pragma config PPS1WAY = OFF    // Peripheral Pin Select one-way control (The PPSLOCK bit can be set and cleared repeatedly by software)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable bit (Stack Overflow or Underflow will cause a reset)

// CONFIG3
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF       // WDT operating mode (WDT Disabled, SWDTEN is ignored)
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC      // WDT input clock selector (Software Control)

// CONFIG4
#pragma config WRT = ON        // UserNVM self-write protection bits (Write protection off)
#pragma config SCANE = available// Scanner Enable bit (Scanner module is available for use)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (Low Voltage programming enabled. MCLR/Vpp pin function is MCLR.)

// CONFIG5
#pragma config CP = OFF         // UserNVM Program memory code protection bit (Program Memory code protection disabled)
#pragma config CPD = OFF        // DataNVM code protection bit (Data EEPROM code protection disabled)

/*
 * General definitions
 */

#define _XTAL_FREQ 8000000

//#define NUMBER_OF_TIMESLOTS 16

#define  PPSUnLock()    {PPSLOCK = 0x55; PPSLOCK = 0xAA; PPSLOCKbits.PPSLOCKED = 0;}
#define  PPSLock() 		{PPSLOCK = 0x55; PPSLOCK = 0xAA; PPSLOCKbits.PPSLOCKED = 1;}

//#define PPS_FUNCTION_CCP1_OUTPUT 14
//#define PPS_FUNCTION_CCP2_OUTPUT 18

#define PIN_INPUT           1
#define PIN_OUTPUT          0
#define PIN_DIGITAL         0
#define PIN_ANALOG          1

#define RELAY1_TRIS TRISAbits.TRISA1
#define RELAY1_PIN LATAbits.LATA1

#define RELAY2_TRIS TRISAbits.TRISA2
#define RELAY2_PIN LATAbits.LATA2

#define RELAY3_TRIS TRISAbits.TRISA0
#define RELAY3_PIN LATAbits.LATA0

#define RELAY_SPEED RELAY3_PIN
#define RELAY_FORWARD RELAY1_PIN
#define RELAY_REVERSE RELAY2_PIN

#define EXTERNAL_ENABLE_TRIS TRISAbits.TRISA3
#define EXTERNAL_ENABLE_ANSEL ANSELAbits.ANSA3
#define EXTERNAL_ENABLE_PIN PORTAbits.RA3

#define EXTERNAL_SPINDLE_TRIS TRISAbits.TRISA4
#define EXTERNAL_SPINDLE_ANSEL ANSELAbits.ANSA4
#define EXTERNAL_SPINDLE_PIN PORTAbits.RA4

#define EXTERNAL_REVERSE_TRIS TRISAbits.TRISA5
#define EXTERNAL_REVERSE_ANSEL ANSELAbits.ANSA5
#define EXTERNAL_REVERSE_PIN PORTAbits.RA5

#define DISPLAY_A_TRIS TRISCbits.TRISC2
#define DISPLAY_A_PIN LATCbits.RC2

#define DISPLAY_B_TRIS TRISCbits.TRISC1
#define DISPLAY_B_PIN LATCbits.RC1

#define DISPLAY_C_TRIS TRISCbits.TRISC7
#define DISPLAY_C_PIN LATCbits.RC7

#define DISPLAY_D_TRIS TRISCbits.TRISC5
#define DISPLAY_D_PIN LATCbits.RC5

#define DISPLAY_E_TRIS TRISCbits.TRISC4
#define DISPLAY_E_PIN LATCbits.RC4

#define DISPLAY_F_TRIS TRISCbits.TRISC3
#define DISPLAY_F_PIN LATCbits.RC3

#define DISPLAY_G_TRIS TRISCbits.TRISC0
#define DISPLAY_G_PIN LATCbits.RC0

#define DISPLAY_DP_TRIS TRISCbits.TRISC6
#define DISPLAY_DP_PIN LATCbits.RC6

#define DISPLAY_SEG1_TRIS TRISBbits.TRISB7
#define DISPLAY_SEG1_PIN LATBbits.LATB7

#define DISPLAY_SEG2_TRIS TRISBbits.TRISB6
#define DISPLAY_SEG2_PIN LATBbits.LATB6

#define DISPLAY_SEG3_TRIS TRISBbits.TRISB5
#define DISPLAY_SEG3_PIN LATBbits.LATB5

#define DISPLAY_SEG4_TRIS TRISBbits.TRISB4
#define DISPLAY_SEG4_PIN LATBbits.LATB4

#define FORWARD_TRIS TRISBbits.TRISB0
#define FORWARD_ANSEL ANSELBbits.ANSB0
#define FORWARD_PIN PORTBbits.RB0

#define REVERSE_TRIS TRISBbits.TRISB3
#define REVERSE_ANSEL ANSELBbits.ANSB3
#define REVERSE_PIN PORTBbits.RB3

#define ENCODER_A_TRIS TRISBbits.TRISB2
#define ENCODER_A_ANSEL ANSELBbits.ANSB2
#define ENCODER_A_PIN PORTBbits.RB2
#define ENCODER_A_IOC_RISING IOCBPbits.IOCBP2
#define ENCODER_A_IOC_FALLING IOCBNbits.IOCBN2
#define ENCODER_A_IOC_FLAG IOCBFbits.IOCBF2
#define ENCODER_A_PPS_VALUE 0x0A

#define ENCODER_B_TRIS TRISBbits.TRISB1
#define ENCODER_B_ANSEL ANSELBbits.ANSB1
#define ENCODER_B_PIN PORTBbits.RB1
#define ENCODER_B_IOC_RISING IOCBPbits.IOCBP1
#define ENCODER_B_IOC_FALLING IOCBNbits.IOCBN1
#define ENCODER_B_IOC_FLAG IOCBFbits.IOCBF1
#define ENCODER_B_PPS_VALUE 0x09


/*
 * Type definitions
 */

typedef struct
{
    volatile uint8_t timeSlot;
    volatile uint8_t done;
    volatile int16_t encoder;
    volatile int16_t encoder_last;
    volatile int16_t encoder_difference;
    volatile uint8_t capture;
    volatile uint32_t time_last;
    volatile uint32_t time_difference;
    volatile uint8_t ready;
    volatile uint32_t time_sum_last;
    volatile uint32_t time_sum;
    int16_t speed;
    
} os_t;


/*
 * Global variables
 */

os_t os;


/*
 * Function prototypes
 */


//void tmr0_isr(void);
//void system_init(void);

#endif	/* OS_H */

