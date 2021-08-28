/*
 * mcu_vco.h
 *
 * Pin definitions for the MCU VCO implemented on the MSP430FR2355
 *
 *  Created on: Jul 25, 2021
 *      Author: Tyler Huddleston
 */

#include <msp430.h>
#include <float.h>
#include <stdio.h>

#ifndef MCU_VCO_H_
#define MCU_VCO_H_


//******************************************************************************
// Constants *******************************************************************
//******************************************************************************

#define SIZE_NOTE_NAME    4
#define SIZE_NOTE_STACK  16
#define SIZE_MESSAGE     32
#define VOLTS_PER_NOTE   0.083333333  // (1 V)/(12 notes per octave) = 0.0833333
#define CV_SCALE         0.25         // to achieve 10 octaves, scale 10V (max note) to 2.5V (max DAC out)

#define MAX_PITCH_BEND   2            // the max notes the pitch bend wheel goes up or down to
#define DAC_ADJ_SCALE    0.033203125  // difference in DAC between notes = 34
                                      // pitch bend range is -2048 to 2047
                                      // DAC_ADJ_SCALE = DAC_diff_between_notes * MAX_PITCH_BEND / MAX_PITCH_BEND_RANGE

#define HEADER "\033[2J\033[2H"                                                                      \
               "  __                              __\r\n"                                         \
               " |  | ___   ____    ____   ____  |  | ___    ___     ___   ____\r\n"              \
               " |  |/   \\ /     \\ /     \\|     \\|  |/   \\ /     \\ /     \\|     \\\r\n"    \
               " |      ^ |    ^_/|    ^_/|    ^ |      ^ |    ^  |    ^  |    ^ |\r\n"           \
               " |__|____/ \\_____\\ \\_____\\|  | _/|__|____/ \\ ___ / \\ ___ /|  | _/\r\n"      \
               " /////////////////////////|__|////////////////////////////|__|\r\n"               \
               " Analog Synthesizer v.0.0\r\n"                                                    \
               "//////////////////////////////////////////////////////////////\r\n\r\n"



//******************************************************************************
// LED Config ******************************************************************
//******************************************************************************

#if BOARD_MODE == BOARD_LAUNCHPAD
    // Red LED at P1.0
    #define LED0_OUT    P1OUT
    #define LED0_DIR    P1DIR
    #define LED0_PIN    BIT0
    #define LED0_OFF    LED0_OUT &= ~LED0_PIN
    #define LED0_ON     LED0_OUT |= LED0_PIN

    #define LED0_EN            \
        LED0_OFF;              \
        LED0_DIR |= LED0_PIN

    // Green LED at P6.6
    #define LED1_OUT    P6OUT
    #define LED1_DIR    P6DIR
    #define LED1_PIN    BIT6
    #define LED1_OFF    LED1_OUT &= ~LED1_PIN
    #define LED1_ON     LED1_OUT |= LED1_PIN

    #define LED1_EN            \
        LED1_OFF;              \
        LED1_DIR |= LED1_PIN

#endif



//******************************************************************************
// DAC Config ******************************************************************
//******************************************************************************

#define DAC0_OUT_EN                                                                   \
    P1SEL0 |= BIT1;                                /* Select P1.1 as OA0O function */ \
    P1SEL1 |= BIT1;                                /* OA is used as buffer for DAC */


#define DAC0_CFG                                                                                                                    \
    SAC0DAC = DACSREF_1 + DACLSEL_0 + DACIE;       /* Select DAC ref = int vref, DAC trigger = writing SAC0DAT, enable interrupt */ \
    SAC0DAT = 0;                                   /* Set initial DAC output to 0 */                                                \
    SAC0DAC |= DACEN;                              /* Enable DAC */                                                                 \
    SAC0OA = NMUXEN + PMUXEN + PSEL_1 + NSEL_1;    /* Select positive and negative pin input */                                     \
    SAC0OA |= OAPM;                                /* Select low speed and low power mode */                                        \
    SAC0PGA = MSEL_1;                              /* Set OA as buffer mode */                                                      \
    SAC0OA |= SACEN + OAEN                         /* Enable SAC and OA */

#define SET_DAC0(x) SAC0DAT = (x)

#define DAC1_OUT_EN                                                                    \
    P1SEL0 |= BIT5;                                 /* Select P1.5 as OA1O function */ \
    P1SEL1 |= BIT5                                  /* OA is used as buffer for DAC */

#define DAC1_CFG                                                                                                                    \
    SAC1DAC = DACSREF_1 + DACLSEL_0 + DACIE;       /* Select DAC ref = int vref, DAC trigger = writing SAC1DAT, enable interrupt */ \
    SAC1DAT = 0;                                   /* Set initial DAC output to 0 */                                                \
    SAC1DAC |= DACEN;                              /* Enable DAC */                                                                 \
    SAC1OA = NMUXEN + PMUXEN + PSEL_1 + NSEL_1;    /* Select positive and negative pin input */                                     \
    SAC1OA |= OAPM;                                /* Select low speed and low power mode */                                        \
    SAC1PGA = MSEL_1;                              /* Set OA as buffer mode */                                                      \
    SAC1OA |= SACEN + OAEN                         /* Enable SAC and OA */

#define SET_DAC1(x) SAC1DAT = (x)

#define DAC2_OUT_EN                                                                   \
    P3SEL0 |= BIT1;                                /* Select P3.1 as OA2O function */ \
    P3SEL1 |= BIT1                                 /* OA is used as buffer for DAC */

#define DAC2_CFG                                                                                                                    \
    SAC2DAC = DACSREF_1 + DACLSEL_0 + DACIE;       /* Select DAC ref = int vref, DAC trigger = writing SAC2DAT, enable interrupt */ \
    SAC2DAT = 0;                                   /* Set initial DAC output to 0 */                                                \
    SAC2DAC |= DACEN;                              /* Enable DAC */                                                                 \
    SAC2OA = NMUXEN + PMUXEN + PSEL_1 + NSEL_1;    /* Select positive and negative pin input */                                     \
    SAC2OA |= OAPM;                                /* Select low speed and low power mode */                                        \
    SAC2PGA = MSEL_1;                              /* Set OA as buffer mode */                                                      \
    SAC2OA |= SACEN + OAEN                         /* Enable SAC and OA */

#define SET_DAC2(x) SAC2DAT = (x)

#define DAC3_OUT_EN                                                                   \
    P3SEL0 |= BIT5;                                /* Select P3.5 as OA3O function */ \
    P3SEL1 |= BIT5                                 /* OA is used as buffer for DAC */

#define DAC3_CFG                                                                                                                    \
    SAC3DAC = DACSREF_1 + DACLSEL_0 + DACIE;       /* Select DAC ref = int vref, DAC trigger = writing SAC3DAT, enable interrupt */ \
    SAC3DAT = 0;                                   /* Set initial DAC output to 0 */                                                \
    SAC3DAC |= DACEN;                              /* Enable DAC */                                                                 \
    SAC3OA = NMUXEN + PMUXEN + PSEL_1 + NSEL_1;    /* Select positive and negative pin input */                                     \
    SAC3OA |= OAPM;                                /* Select low speed and low power mode */                                        \
    SAC3PGA = MSEL_1;                              /* Set OA as buffer mode */                                                      \
    SAC3OA |= SACEN + OAEN                         /* Enable SAC and OA */

#define SET_DAC3(x) SAC3DAT = (x)

#define SET_INT_REF_1V5                                                           \
    PMMCTL0_H = PMMPW_H;               /* Unlock the PMM regs */                  \
    PMMCTL2 = INTREFEN | REFVSEL_0;    /* Enable internal 1.5V reference*/        \
    while(!(PMMCTL2 & REFGENRDY))      /* Poll until internal reference settles*/

#define SET_INT_REF_2V0                                                           \
    PMMCTL0_H = PMMPW_H;               /* Unlock the PMM regs*/                   \
    PMMCTL2 = INTREFEN | REFVSEL_1;    /* Enable internal 2.0V reference*/        \
    while(!(PMMCTL2 & REFGENRDY))      /* Poll until internal reference settles*/

#define SET_INT_REF_2V5                                                           \
    PMMCTL0_H = PMMPW_H;               /* Unlock the PMM regs*/                   \
    PMMCTL2 = INTREFEN | REFVSEL_2;    /* Enable internal 2.5V reference*/        \
    while(!(PMMCTL2 & REFGENRDY))      /* Poll until internal reference settles*/



//******************************************************************************
// Structures ******************************************************************
//******************************************************************************

// MIDI note structure
struct note {
    unsigned char value;
    unsigned char velocity;
    unsigned char on;
    char name[SIZE_NOTE_NAME];
};



//******************************************************************************
// Function Definitions ********************************************************
//******************************************************************************

void initClockTo16MHz(void);             // Initialize CPU clock to 16 MHz
void initUARTs(void);                    // Configure USCI_A0 & A1 for UART mode
void initGPIO(void);                     // Set pin directions
void initDACs(void);                     // Initialize DACs
void initMIDINotes(struct note *notes);  // Return empty MIDI note stack



#endif /* MCU_VCO_H_ */
