/*
 *
 *
 * The VCO supports the following MIDI messages:
 *   NOTE OFF
 *   NOTE ON
 *   PITCH BEND
 *   CONTROL - ALL SOUND OFF
 *   CONTROL - ALL NOTES OFF
 *   TUNE REQUEST
 *   ACTIVE SENSING
 *
 */

//******************************************************************************
// INCLUDES ********************************************************************
//******************************************************************************

#include <msp430.h>
#include <cfg.h>
#include <mcu_vco.h>
#include <midi.h>
#include <midi_luts.h>
#include <float.h>


//******************************************************************************
// GLOBAL VARIABLES ************************************************************
//******************************************************************************

// midi channel
unsigned char midi_channel  = 0;

// midi event flags
unsigned char f_midi_note_on    = 0;
unsigned char f_midi_note_off   = 0;
unsigned char f_midi_pitch_bend = 0;

// midi note stack
struct note midi_notes[SIZE_NOTE_STACK];
unsigned char ptr_note = 0;

// midi pitch bend value
int midi_pitch_bend_val = 0;

// midi rx values
unsigned char midi_note_val = 0;
unsigned char midi_note_vel = 0;

// Debug UART terminal transmit variables
#if DEBUG == 1
    char debug_msg[SIZE_MESSAGE];  // midi event message
    unsigned int TXbytes = 0;       // number of TX bytes to transmit
    unsigned char f_print_start = 1;
    char header_msg[800];
#endif

//******************************************************************************
// MAIN ************************************************************************
//******************************************************************************
int main(void)
{
	
    // stop watchdog time
	WDTCTL = WDTPW | WDTHOLD;

	// call device initialization functions
	initGPIO();
	initClockTo16MHz();
	initUARTs();
	initDACs();
	initMIDINotes(midi_notes);

	// print header to debug terminal
    #if DEBUG == 1
	    sprintf(header_msg,HEADER);
	    UCA1IE |= UCTXIE;
    #endif

	SET_DAC0(0);

	// Enable interrupts
	  __bis_SR_register(GIE);

	while(1)
	{
	    if (f_midi_pitch_bend == 4)
	    {
	        f_midi_pitch_bend = 8;
	    }

	    if (f_midi_note_on == 4)
	    {
	        midi_notes[ptr_note].on       = 1;
	        midi_notes[ptr_note].value    = midi_note_val;
	        midi_notes[ptr_note].velocity = midi_note_vel;

	        f_midi_note_on = 8;
	     }

	     if (f_midi_note_on == 8)
	     {
	         unsigned int dac_val = conv_midi_to_dac(midi_notes[ptr_note].value);

	         // report note on for debug
	         #if DEBUG == 1
	            sprintf(debug_msg, "N = %d  V = %d ON DAC = %d\r\n", midi_notes[ptr_note].value, midi_notes[ptr_note].velocity, dac_val);
	            UCA1IE |= UCTXIE;
	         #endif

	         // Set CV DAC value
	         SET_DAC0(dac_val & 0x0FFF);

	         f_midi_note_on = 0;
	         ptr_note ++;
	     }

	     if (f_midi_note_off == 4)
	     {
	         // report note off for debug
	         #if DEBUG == 1
	             sprintf(debug_msg, "N = %d  V = %d OFF\r\n", midi_note_val, midi_note_vel);
	             UCA1IE |= UCTXIE;
	         #endif

	         f_midi_note_off = 0;

	         // turn off note and shift on notes up in the stack
	         unsigned int i;
	         for (i=0; i < SIZE_NOTE_STACK-1; ++i)
	         {
	             // find note in the stack and turn it off
	             if (midi_notes[i].value == midi_note_val)
	             {
	                 midi_notes[i].on = 0;
	             }
	             // if last note reached or next note is off
	             if (i==SIZE_NOTE_STACK-1 || !midi_notes[i+1].on)
	             {
	                 if (i>0) ptr_note = i-1;
	                 else ptr_note = 0;
	                 if (midi_notes[0].on) f_midi_note_on = 8;
	                 if (midi_pitch_bend_val != 0) f_midi_pitch_bend = 4;
	                 break;
	             }
	             // shift up if current note off and next on
	             else if (!midi_notes[i].on && midi_notes[i+1].on)
	             {
	                 midi_notes[i] = midi_notes[i+1];
	                 midi_notes[i+1].on = 0;
	             }
	         }

	         // turn DAC output off if no note is currently played
	         if (midi_notes[0].on == 0)
	         {
	             SET_DAC0(0);
	         }
	     }

         // adjust DAC output if pitch bend received
         if (f_midi_pitch_bend == 8)
         {
             // report pitch bend message for debug
             #if DEBUG == 1
                 sprintf(debug_msg, "PB = %d \r\n", midi_pitch_bend_val);
                 UCA1IE |= UCTXIE;
             #endif

             // clear pitch bend flag
             f_midi_pitch_bend = 0;

             // if a note is on, bend it
             if (midi_notes[ptr_note-1].on)
             {
                 // calculate DAC output: note + bend
                 unsigned int dac_val = (unsigned int)(   conv_midi_to_dac(midi_notes[ptr_note-1].value)
                                                        + DAC_ADJ_SCALE * midi_pitch_bend_val            );
                 // Set CV DAC value
                 SET_DAC0(dac_val & 0x0FFF);
             }
         }
	}
}


//******************************************************************************
// UART Interrupts ***********************************************************
//******************************************************************************

// MIDI RX
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;

    case USCI_UART_UCRXIFG:
      while(!(UCA0IFG&UCTXIFG));
      if (UCA0RXBUF != MIDI_CLOCK_SYNC)   // ignore sync clock messages
      {
          // Note On or Note Off if velocity = 0 (as in Organelle)
          if (UCA0RXBUF == MIDI_NOTE_ON_BASE + midi_channel)
          {
              f_midi_note_on = 1;
          }
          else if (f_midi_note_on == 1)   // Note on value
          {
              midi_note_val = UCA0RXBUF;
              f_midi_note_on = 2;
          }
          else if (f_midi_note_on == 2)   // Note on velocity
          {
              midi_note_vel = UCA0RXBUF;
              if (midi_note_vel == 0)
              {
                  f_midi_note_on  = 0;
                  f_midi_note_off = 4;
              }
              else f_midi_note_on = 4;
          }

          // Note Off
          if (UCA0RXBUF == MIDI_NOTE_OFF_BASE + midi_channel)
          {
              f_midi_note_off = 1;
          }
          else if (f_midi_note_off == 1)
          {
              f_midi_note_off = 2;
              midi_note_val = UCA0RXBUF;
          }
          else if (f_midi_note_off == 2)
          {
              f_midi_note_off = 4;
              midi_note_vel = UCA0RXBUF;
          }

          // Pitch Bend
          if (UCA0RXBUF == MIDI_PITCH_BEND_BASE + midi_channel)
          {
              f_midi_pitch_bend = 1;
          }
          else if (f_midi_pitch_bend == 1)
          {
              f_midi_pitch_bend = 2;
              midi_pitch_bend_val = (int)(UCA0RXBUF) >> 2;   // drop 2 LSBs for 12-bit value
          }
          else if (f_midi_pitch_bend == 2)
          {
              f_midi_pitch_bend = 4;
              midi_pitch_bend_val += 32 * (int)(UCA0RXBUF) - 2048;  // add MSB to the 5 LSBs already received and center about 2^11 for -2048 to +2047 range
          }
      }
      __no_operation();
      break;

    case USCI_UART_UCTXIFG: break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
  }
}

// Debug RX & TX
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A1_VECTOR))) USCI_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCA1IV, USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;

    case USCI_UART_UCRXIFG:
      while(!(UCA1IFG&UCTXIFG));
      UCA1TXBUF = UCA1RXBUF;
      __no_operation();
      break;

    case USCI_UART_UCTXIFG:
      // Transmit the byte
      if(f_print_start)
      {
          UCA1TXBUF = header_msg[TXbytes++];
          // If last byte sent, disable the interrupt
          if(TXbytes == 800)
          {
              UCA1IE &= ~UCTXIE;
              TXbytes = 0;
              f_print_start = 0;
          }
      }
      else
      {
          UCA1TXBUF = debug_msg[TXbytes++];
          // If last byte sent, disable the interrupt
          if(debug_msg[TXbytes-1] == '\n' || TXbytes == SIZE_MESSAGE)
          {
              UCA1IE &= ~UCTXIE;
              TXbytes = 0;
          }
      }

      break;

    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
  }
}
