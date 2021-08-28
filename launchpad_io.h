/*
 * launchpad_io.h
 *
 *  Created on: Jul 25, 2021
 *      Author: tyler
 */

#ifndef LAUNCHPAD_IO_H_
#define LAUNCHPAD_IO_H_

//******************************************************************************
// LED Config ******************************************************************
//******************************************************************************

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


//******************************************************************************
//******************************************************************************
//******************************************************************************


#endif /* LAUNCHPAD_IO_H_ */
