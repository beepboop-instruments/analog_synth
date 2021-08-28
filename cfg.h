/*
 * cfg.h
 *
 *  Created on: Jul 25, 2021
 *      Author: tyler
 */

#ifndef CFG_H_
#define CFG_H_

//******************************************************************************
// COMPILE CONFIG **************************************************************
//******************************************************************************

// Debug enable: 1=On, 0=Off
#define DEBUG 1

// Board Mode
#define BOARD_LAUNCHPAD 0
#define BOARD_VCO_0v1   1
#define BOARD_MODE      BOARD_LAUNCHPAD

#if BOARD_MODE == BOARD_LAUNCHPAD
  #include <launchpad_io.h>
#elif BOARD_MODE == BOARD_VCO_0V1
#else
  #error Select a valid Board Mode!
#endif

// Set DAC reference
#define DAC_REF_1V5 1.5
#define DAC_REF_2V0 2.0
#define DAC_REF_2V5 2.5
#define DAC_REF DAC_REF_2V5

#endif /* CFG_H_ */
