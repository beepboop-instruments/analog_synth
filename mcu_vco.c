/*
 * mcu_vco.c
 *
 *  Created on: Jul 25, 2021
 *      Author: tyler
 */

#include <mcu_vco.h>

// Initialize CPU clock to 16 MHz
void initClockTo16MHz()
{
    // Configure one FRAM waitstate as required by the device datasheet for MCLK
    // operation beyond 8MHz _before_ configuring the clock system.
    FRCTL0 = FRCTLPW | NWAITS_1;

    __bis_SR_register(SCG0);    // disable FLL
    CSCTL3 |= SELREF__REFOCLK;  // Set REFO as FLL reference source
    CSCTL0 = 0;                 // clear DCO and MOD registers
    CSCTL1 &= ~(DCORSEL_7);     // Clear DCO frequency select bits first
    CSCTL1 |= DCORSEL_5;        // Set DCO = 16MHz
    CSCTL2 = FLLD_0 + 487;      // set to fDCOCLKDIV = (FLLN + 1)*(fFLLREFCLK/n)
                                //                   = (487 + 1)*(32.768 kHz/1)
                                //                   = 16 MHz
    __delay_cycles(3);
    __bic_SR_register(SCG0);                        // enable FLL
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));      // FLL locked

    P2SEL1 |= BIT6 | BIT7;                  // P2.6~P2.7: crystal pins
    do
    {
        CSCTL7 &= ~(XT1OFFG | DCOFFG);      // Clear XT1 and DCO fault flag
        SFRIFG1 &= ~OFIFG;
    }while (SFRIFG1 & OFIFG);               // Test oscillator fault flag

    __bis_SR_register(SCG0);                // Disable FLL
    CSCTL3 = SELREF__XT1CLK;                // Set XT1 as FLL reference source
    CSCTL4 = SELMS__DCOCLKDIV | SELA__XT1CLK;  // set ACLK = XT1CLK = 32768Hz
                                               // DCOCLK = MCLK and SMCLK source
}


// Configure USCI_A0 & A1 for UART mode
void initUARTs()
{
    // A0 = MIDI UART
    UCA0CTLW0 |= UCSWRST;                     // Put eUSCI in reset when making changes
    UCA0CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK
    UCA0BRW = 32;                             // Baud Rate calculation: (16 MHz)/(16)/(31250) = 32
    UCA0MCTLW |= UCOS16;                      // enable 16 clock oversampling
    UCA0CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
    UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

    // A1 = Debug UART
    UCA1CTLW0 |= UCSWRST;                     // Put eUSCI in reset when making changes
    UCA1CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK
    UCA1BRW = 8;                              // Baud Rate Setting: Use MSP430 family ref manual
    UCA1MCTLW |= UCOS16 | UCBRF_10 | 0xF700;  //0xF700 is UCBRSx = 0xF7
    UCA1CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
    UCA1IE |= UCRXIE;                         // Enable USCI_A1 RX interrupt
}

// Set pin directions
void initGPIO()
{
    // LEDs
    LED0_EN; LED0_ON;
    LED1_EN; LED1_ON;

    // DACs
    DAC0_OUT_EN;

    // Configure GPIO
    P1SEL1 &= ~(BIT6);                        // USCI_A0 UART operation (RXD only)
    P1SEL0 |= BIT6;
    P1DIR  |= BIT4;                           // P1.4 is HARD SYNC output

    P2SEL0 |= BIT2;                           // P2.2 selected as TB1CLK

    P4SEL1 &= ~(BIT2 | BIT3);                 // USCI_A1 UART operation
    P4SEL0 |= BIT2 | BIT3;

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;
}

// Initialize DACs
void initDACs()
{
    #if DAC_REF == DAC_REF_2V5
        SET_INT_REF_2V5;
    #elif DAC_REF == DAC_REF_2V0
        SET_INT_REF_2V0;
    #elif DAC_REF == DAC_REF_1V5
        SET_INT_REF_1V5;
    #else
        #error Define valid DAC_REF
    #endif

    DAC0_CFG;
    DAC1_CFG;
    DAC2_CFG;
}


// Initialize midi note stack to empty
void initMIDINotes(struct note *notes)
{
    // loop through each note and set values to defaults
    unsigned int i;
    for (i=sizeof(notes)-1; i>0 ; i--)
    {
        notes[i].value    =  0;
        notes[i].velocity =  0;
        notes[i].on       =  0;
        sprintf(notes[i].name, "");
    }
}


// Initialize and start the frequency counter timer
void initFreqCtr()
{
    // 1. Write 1 to TBCLR to clear TBxR, clock divider state, and the counter direction
    TB0CTL = TBCLR;
    TB1CTL = TBCLR;

    // 2. If necessary, write initial counter value to TBxR
    // not necessary because we want both at 0 as set by TBCLR
    TB1R = 0xFFFF;

    // 3. Initialize TBxCCRn
    // not using capture compare for TB0
    //TB1CCR1 = 1;        // count up to 1 clock

    // 4. Apply desired configuration to TBxIV, TBIDEX, and TBxCCTLn
    // not using TBxIV
    //TB1CCTL1 = CCIE;    // enable interrupts
    TB0EX0 = TBIDEX_7;  // divide by 8

    // 5. Apply desired configuration to TBxCTL including the MC bits
    TB0CTL =  ID_3 | TBSSEL_2;         // divide by 8, use SMCLK
    TB1CTL = TBSSEL_0 | MC_2 | TBIE;          // TBR1CLK, continuous mode
}

