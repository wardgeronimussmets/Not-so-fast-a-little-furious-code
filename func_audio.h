/*********************************************************************
 *
 *                  Not so fast a little furious
 *
 *********************************************************************
 * Processor:       PIC18F25K50
 * Compiler:        XC8 1.35+
 * Author:          Jeroen Van Aken
 * Updated:         17/02/2016
 ********************************************************************/

#ifndef _FUNC_AUDIO_
#define	_FUNC_AUDIO_

/** I N C L U D E S *************************************************/  
#include "config.h"

#if ENABLE_AUDIO == TRUE

/** D E F I N E S ***************************************************/
#ifndef AUDIO_OUT
  #error "Audio enabled, but no output selected"
#endif

// define the notes with the correct pre-loaded value for the timer
// C1 : 261.62 Hz
#define C1    35105
// D1 : 293.66 Hz
#define D1    38383
// E1 : 329.62 Hz
#define E1    41386
// F1 : 349.22 Hz
#define F1    42779
// G1 : 391.99 Hz
#define G1    45270
// A1 : 440.00 Hz
#define A1    47473
// B1 : 493.88 Hz
#define B1    49467
// C2 : 523.25 Hz
#define C2    50365
// D2 : 587.32 Hz
#define D2    52026
// E2 : 659.25 Hz
#define E2    53499
// F2 : 698.45 Hz
#define F2    54192
// G2 : 783.99 Hz
#define G2    55445
// A2 : 880.00 Hz
#define A2    56550
// B2 : 987.76 Hz
#define B2    57555
// C3 : 1046.50 Hz
#define C3    58044

/** P U B L I C   V A R I A B L E S *********************************/
// when a variable is declared 'extern' it also has to be declared in
// the corresponding .c file without the 'extern' keyword

/** P U B L I C   P R O T O T Y P E S *******************************/
void AUDIO_init(void);
void AUDIO_play(unsigned int freq);
void AUDIO_stop(void);
void AUDIO_ISR(void);

#endif

#endif

