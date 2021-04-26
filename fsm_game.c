/*********************************************************************
 *
 *                  Not so fast a little furious
 *
 *********************************************************************
 * Processor:       PIC18F25K50
 * Compiler:        XC8 1.35+
 * Author:          Jeroen Van Aken
 * Updated:         03/03/2020
 * 
 ********************************************************************/
/** I N C L U D E S *************************************************/
#include "config.h"
#include <stdlib.h>

/** D E F I N E S ***************************************************/
#define PUSHED 0
#define RELEASED 1

/** P R I V A T E   V A R I A B L E S *******************************/




static enum {FSM_IDLE,FSM_INITIALISE,FSM_GO,FSM_CARS_FORWARD,FSM_CARS,BREAK,FSM_CARS_BURST,FSM_CARS_BACKWARDS
            } current_state;
            
/********************************************************************
 * Function:        void fsm_game_init(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        This init sets the start state for this FSM and
 *                  may initialize some counters          
 ********************************************************************/
void fsm_game_init(void) {
	current_state = FSM_IDLE;
}

/********************************************************************
 * Function:        void fsm_game(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        An implementation for a simple reaction game
 ********************************************************************/
void fsm_game(void) {
    
    switch (current_state) { 
        case FSM_IDLE :
        // *** outputs ***
            //led1_output = 1;
            
        // *** transitions ***
            if (PRG_BUTTON == PUSHED) current_state = FSM_IDLE;
            break;

        default:
            current_state = FSM_IDLE;
            break;
    }
    
}
