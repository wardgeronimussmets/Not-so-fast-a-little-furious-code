/*********************************************************************
 *
 *                  Not so fast a little furious
 *
 *********************************************************************
 * Processor:       PIC18F25K50
 * Compiler:        XC8 1.35+
 * Author:          Jeroen Van Aken
 * Updated:         03/03/2020
 ********************************************************************/
/** I N C L U D E S *************************************************/
#include "config.h"

/** P R I V A T E   V A R I A B L E S *******************************/
static unsigned char servo_counter;
static unsigned char stepper_counter;
static unsigned char pwm_counter;
static enum {FSM_SETIO,
             FSM_UPDATE_SERVO,
             FSM_UPDATE_STEPPER_L,
             FSM_UPDATE_STEPPER_H,
            } current_state;
static enum {MULT_A0,MULT_A1,MULT_A2,MULT_A3,MULT_A4,MULT_A5,MULT_A6,MULT_A7,MULT_None}current_mult;
            
/********************************************************************
 * Function:        void fsm_io_init(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        This init sets the start state for this FSM and
 *                  may initialize some counters          
 ********************************************************************/
void fsm_io_init(void) {
	current_state = FSM_SETIO;
    current_mult = MULT_A0;
    
    
    
}

/********************************************************************
 * Function:        fsm_io(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        A simple FSM that will start blinking a led on 
 *                  and off once you pushed a button.          
 ********************************************************************/
void fsm_io(void) {
    
    switch (current_state) {                
        
            
        default:
            current_state = FSM_SETIO;
            break;
    }
    
    //Multiplexing
    switch(current_mult){
        
        case MULT_A0://connected to a start end loop switch and the end loop switch
        
            if(PRG_BUTTON==1)
            {
                ENDLOOP_StartS = 1;
                
            }
            if(CONTR_OUT1>1)
            {
                ENDLOOP_StartS = 1;
            }
            
            
            break;
        case MULT_A1://connected to clutch of corresponding controller
            break;
        case MULT_A2://connected to gear 6
            break;
        case MULT_A3://connected to gear 5
            break;
        case MULT_A4://connected to gear 4
            break;
        case MULT_A5://connected to gear 3
            break;
        case MULT_A6://connected to gear 2
            break;
        case MULT_A7://connected to gear 1
            break;
        
        default:
            current_mult = MULT_None;
            break;
    }
    
}
