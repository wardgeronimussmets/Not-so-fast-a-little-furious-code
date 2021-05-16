/*********************************************************************
 *
 *                  Not so fast a little furious plz
 *
 *********************************************************************
 * Processor:       PIC18F25K50
 * Compiler:        XC8 1.35+
 * Author:          Jeroen Van Aken
 * Updated:         03/03/2020
 ********************************************************************/
/** I N C L U D E S *************************************************/
#include "config.h"

/**DEFINES*/
#define HIGH 1
#define LOW 0
#define PRESSED 1
#define RELEASED 0
#define TRUE 1
#define FALSE 0




/** P R I V A T E   V A R I A B L E S *******************************/

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
    current_mult = MULT_A0;
    MULT_ENABLE = 0;
    
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
    ///leds
    CONTR_BD1 = BDLED1_out;
    //CONTR_BD1 = 1;
    CONTR_BD2 = BDLED2_out;
    //CONTR_BD2 = 1;
    //REV1 = 1;
    //REV2 = 1;
    //LED2_OUT
    
    START_LED_GR = LEDGr_out;
    START_LED_RED = LEDRed_out;
    
    
    //switches
    ENDLOOP_StartS2 = END_START_SWITCH2;
    
    
    
        
       
    
    
    ///Multiplexing
    switch(current_mult){
        
        case MULT_A0://connected to a start end loop switch and the end loop switch
        
            ENDLOOP_StartS1 = CONTR_OUT1;
            ENDLOOP_FinishS = CONTR_OUT2;
            
            MULT_S0 = HIGH;
            MULT_S1 = LOW;
            MULT_S2 = LOW;
            MULT_ENABLE = LOW;
            current_mult = MULT_A1;            
            break;
        case MULT_A1://connected to clutch of corresponding controller
            CONT1_CLUTCH = CONTR_OUT1;
            CONT2_CLUTCH = CONTR_OUT2;
            
            MULT_S0 = LOW;
            MULT_S1 = HIGH;
            MULT_S2 = LOW;
            MULT_ENABLE = LOW;
            current_mult = MULT_A2;
            break;
        case MULT_A2://connected to gear 6
            CONT1_GEAR6 = CONTR_OUT1;
            CONT2_GEAR6 = CONTR_OUT2;
            
            MULT_S0 = HIGH;
            MULT_S1 = HIGH;
            MULT_S2 = LOW;
            MULT_ENABLE = LOW;
            current_mult = MULT_A3;
            break;
        case MULT_A3://connected to gear 5
            CONT1_GEAR5 = CONTR_OUT1;
            CONT2_GEAR5 = CONTR_OUT2;
            
            MULT_S0 = LOW;
            MULT_S1 = LOW;
            MULT_S2 = HIGH;
            MULT_ENABLE = LOW;
            current_mult = MULT_A4;
            break;
        case MULT_A4://connected to gear 4
            CONT1_GEAR4 = CONTR_OUT1;
            CONT2_GEAR4 = CONTR_OUT2;
            
            MULT_S0 = HIGH;
            MULT_S1 = LOW;
            MULT_S2 = HIGH;
            MULT_ENABLE = LOW;
            current_mult = MULT_A5;
            break;
        case MULT_A5://connected to gear 3
            CONT1_GEAR3 = CONTR_OUT1;
            CONT2_GEAR3 =CONTR_OUT2;
            
            MULT_S0 = LOW;
            MULT_S1 = HIGH;
            MULT_S2 = HIGH;
            MULT_ENABLE = LOW;
            current_mult = MULT_A6;
            break;
        case MULT_A6://connected to gear 2
            CONT1_GEAR2 = CONTR_OUT1;
            CONT2_GEAR2 = CONTR_OUT2;
            
            MULT_S0 = HIGH;
            MULT_S1 = HIGH;
            MULT_S2 = HIGH;
            MULT_ENABLE = LOW;
            current_mult = MULT_A7;
            break;
        case MULT_A7://connected to gear 1
            CONT1_GEAR1 = CONTR_OUT1;
            CONT2_GEAR1 = CONTR_OUT2;
            
            MULT_S0 = LOW;
            MULT_S1 = LOW;
            MULT_S2 = LOW;
            MULT_ENABLE = LOW;
            current_mult = MULT_A0;
            break;
        case MULT_None:
            MULT_ENABLE = HIGH;
            current_mult = MULT_A0;
        default:
            current_mult = MULT_None;
            break;
    }
    
}
