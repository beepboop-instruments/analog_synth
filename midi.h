/*
 * midi.h
 *
 *  Created on: Jul 25, 2021
 *      Author: Tyler Huddleston
 *    Resource: https://www.midi.org/specifications-old/item/table-1-summary-of-midi-message
 */

#ifndef MIDI_H_
#define MIDI_H_


// Channel voice messages
#define MIDI_NOTE_OFF_BASE          0x80  // note off event followed by note number (0kkkkkkk) then velocity (0vvvvvvv)
#define MIDI_NOTE_ON_BASE           0x90  // note on event followed by note number (0kkkkkkk) then velocity (0vvvvvvv)
#define MIDI_KEY_PRESSURE_BASE      0xA0  // polyphonic key pressure (aftertouch) - sent when pressing down on a key after it has bottomed out followed by note number (kkkkkkkk) then pressure (0vvvvvvv)
#define MIDI_CONTROL_CHANGE_BASE    0xB0  // when controller (pedal/level) has changed value followed by controller number (0ccccccc) and value (0vvvvvvv) - control number is 0-119
#define MIDI_PROGRAM_CHANGE_BASE    0xC0  // when the patch number has changed followed by new program number (pppppppp)
#define MIDI_CH_PRESSURE_BASE       0xD0  // channel pressure (aftertouch) used to send single greatest pressue of all current depressed keys followed by value (vvvvvvvv)
#define MIDI_PITCH_BEND_BASE        0xE0  // 14-bit pitch bend value followed by LSB (0lllllll) then MSB (0mmmmmmm)

// Channel mode messages
#define MIDI_MODE_MSG_BASE          0xB0  // same as control change, but for c=120-127,followed by control number (0ccccccc) then value (0vvvvvvv)
#define MIDI_CTL_ALL_SOUND_OFF      120   // all oscillators turn off and their volume envelopes set to 0 asap, when v=0
#define MIDI_CTL_RESET_ALL          121   // reset all controllers to default values, recommended v=0
#define MIDI_CTL_LOCAL              122   // when off (v=0), all devices on channel respond only to data received over MIDI, when on (v=127) normal functions of controllers restored
#define MIDI_CTL_ALL_NOTES_OFF      123   // all notes off (v=0)
#define MIDI_CTL_OMNI_MODE_OFF      124   // omni mode is turned off (v=0)
#define MIDI_CTL_OMNI_MODE_ON       125   // omni mode is turn on (v=0)
#define MIDI_CTL_MONO_MODE          126   // mono mode on (poly off) v=M where M is the number of channels omni off
#define MIDI_CTL_POLY_MODE          127   // mono mode off (poly on) (v=0)

// System common messages
#define MIDI_SYS_EXCLUSIVE          0xF0  // system exclusive, manufacturer specific - usually bulk dumps, patcher parameters, etc
#define MIDI_TIME_QTR_FRAME         0XF1  // song time code quarter frame, followed by 0nnndddd, where n=message type, d=values
#define MIDI_SONG_POS_PTR           0xF2  // song positions pointer, followed by 0lllllll 0mmmmmmm, 14-bit value of the number of beats from start of song (1 beat = 6 midi clocks)
#define MIDI_SONG_SELECT            0xF3  // song select, followed by 0sssssss = song or sequence number to be played
#define MIDI_TUNE_REQUEST           0xF6  // tune request, requests all analog synthesizers to tune their oscillators
#define MIDI_SYS_EXCLUSIVE_END      0xF7  // end of System Exclusive message (eg., end of dump)

// System real-time messages
#define MIDI_CLOCK_SYNC             0xF8  // sent 24x per quarter note when synchronization used
#define MIDI_START_SEQ              0xFA  // start the current sequence
#define MIDI_CONT_SEQ               0xFB  // continue the current sequence from where stopped
#define MIDI_STOP_SEQ               0xFC  // stop the current sequence
#define MIDI_ACTIVE_SENSING         0xFE  // intended to be sent repeatedly to tell the receiver that connection is alive
#define MIDI_RESET                  0xFF  // reset all receivers in the system to power-up status


#endif /* MIDI_H_ */
