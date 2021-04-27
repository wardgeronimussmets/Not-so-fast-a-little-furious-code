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

#define HIGH 1
#define LOW 0
#define TRUE 1
#define FALSE 0

#define RedLDWait 3000
#define GreenLDWait 5000

/** P R I V A T E   V A R I A B L E S *******************************/
static unsigned char greenLDWasOn = TRUE;
static unsigned char gear1 = 0;
static unsigned char gear2 = 0;
static unsigned char newGear1 = 0;
static unsigned char newGear2 = 0;

static unsigned int counter = 0;

static enum {FSM_IDLE,FSM_INITIALISE,FSM_GO,FSM_FORWARD,FSM_CARS,BREAK,FSM_CARS_BURST,FSM_BACKWARDS,FSM_GAMEOVER
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
  
    counter = 0;
    DC2Fw_out = LOW;
    DC1Bw_out = LOW;
    DC2Bw_out = LOW;
    BDLED1_out = LOW;
    BDLED2_out = LOW;
    CONTR_VU1 = LOW;
    CONTR_VU2 = LOW;
    LEDGr_out = LOW;
    LEDRed_out = LOW;
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
        case FSM_IDLE:
            /*DC1Fw_out = LOW;
            DC2Fw_out = LOW;
            DC1Bw_out = LOW;
            DC2Bw_out = LOW;
            BDLED1_out = LOW;
            BDLED2_out = LOW;
            CONTR_VU1 = LOW;
            CONTR_VU2 = LOW;
            LEDGr_out = LOW;
            LEDRed_out = LOW;*/
            
            if(CONT1_CLUTCH == PUSHED && CONT2_CLUTCH == PUSHED)
            {
                current_state = FSM_INITIALISE;
                counter = 0;
            }
        case FSM_INITIALISE:
            /*DC1Fw_out = LOW;
            DC2Fw_out = LOW;
            DC1Bw_out = LOW;
            DC2Bw_out = LOW;
            BDLED1_out = LOW;
            BDLED2_out = LOW;
            CONTR_VU1 = LOW;
            CONTR_VU2 = LOW;
            LEDGr_out = LOW;*/
            LEDRed_out = HIGH;
            
            counter ++;
            if(counter > RedLDWait)
            {
                current_state = FSM_GO;
                counter = 0;
            }
            
        case FSM_GO:
            LEDRed_out = LOW;
            LEDGr_out = HIGH;
            greenLDWasOn = FALSE;
            current_state = FSM_FORWARD;
            
       case FSM_FORWARD:
           //check if green led needs to be on
           if(!greenLDWasOn)
           {
               counter ++;
               if(counter > GreenLDWait)
               {
                   greenLDWasOn = TRUE;
                   LEDGr_out = LOW;
               }
           }   
           //check if a car has finished
           if(ENDLOOP_FinishS == PUSHED)
               current_state = FSM_GAMEOVER;
           
           
           //check if car has shifted a gear          
        newGear1 = 0;
        if(CONT1_GEAR1 == PUSHED)  newGear1 =  1;
        if(CONT1_GEAR2 ==  PUSHED) newGear1 =  2;
        if(CONT1_GEAR3 ==  PUSHED) newGear1 =  3;
        if(CONT1_GEAR4 ==  PUSHED) newGear1 =  4;
        if(CONT1_GEAR5 ==  PUSHED) newGear1 =  5;
        if(CONT1_GEAR6 ==  PUSHED) newGear1 =  6;
    
        newGear2 = 0;
        if(CONT2_GEAR1 ==  PUSHED) newGear2 =  1;
        if(CONT2_GEAR2 ==  PUSHED) newGear2 =  2;
        if(CONT2_GEAR3 ==  PUSHED) newGear2 =  3;
        if(CONT2_GEAR4 ==  PUSHED) newGear2 =  4;
        if(CONT2_GEAR5 ==  PUSHED) newGear2 =  5;
        if(CONT2_GEAR6 ==  PUSHED) newGear2 =  6;
        
        if(newGear1 == gear1)
        {
            //gear1 no gear change
            
        }
        else
        {
            //gear1 has been changed
        }
    
           
        default:
            current_state = FSM_IDLE;
            break;
    }
    
    
    
}
