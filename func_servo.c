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
#include "func_servo.h"
#if ENABLE_SERVO == TRUE

/** P U B L I C   V A R I A B L E S *********************************/
// in order for the variable to be used in other file, it also has to
// be declared as 'extern' in the corresponding .h file
unsigned char servo_setpoint[SERVO_CHANNELS];

/** P R I V A T E   V A R I A B L E S *******************************/
// 'static' implies that the variable can only be used in this file
// (cfr. 'private' in Java)
static unsigned char servo_counter;

/** P R I V A T E   P R O T O T Y P E S *****************************/
// 'static' implies that the function can only be used in this file
// (cfr. 'private' in Java)
static void OpenTimer3(unsigned char intEnable);

/********************************************************************/
/** P U B L I C   D E C L A R A T I O N S ***************************/
/********************************************************************
 * Function:        void SERVO_init(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        Initializes all output channels for the PWM process
 ********************************************************************/
void SERVO_init() {
	servo_counter = 0;
	for(unsigned char i = 0; i < SERVO_CHANNELS; i++) {
        servo_setpoint[i] = 0u;
    }
    OpenTimer3(TRUE);
}

/********************************************************************
 * Function:        void SERVO_setPosition(...)
 * PreCondition:    SERVO_init()
 * Input:           channel : the channel to be set
 *                  setpoint : the new setpoint
 * Output:          None
 * Overview:        Changes the setpoint for a certain servo and checks
 *                  if the setpoint is within the limits
 ********************************************************************/
void SERVO_setPosition(unsigned char channel, unsigned char setpoint) {
    if (setpoint < 1) setpoint = 1;
    if (setpoint > 25) setpoint = 25;
    if (channel < SERVO_CHANNELS) {
        servo_setpoint[channel] = setpoint;
    }
}

/********************************************************************
 * Function:        void SERVO_getPosition(...)
 * PreCondition:    SERVO_init()
 * Input:           channel : the channel to be read
 * Output:          the current setpoint
 * Overview:        reads out the setpoint for a certain servo
 ********************************************************************/
unsigned char SERVO_getPosition(unsigned char channel) {
    if (channel < SERVO_CHANNELS) {
        return servo_setpoint[channel];
    } else {
        return 0;
    }
}

/********************************************************************
 * Function:        void SERVO_ISR(void)
 * PreCondition:    SERVO_init()
 * Input:           None
 * Output:          None
 * Overview:        SERVO Interrupt service routine will process all
 *                  setpoints and set the outputs accordingly
 ********************************************************************/
void SERVO_ISR(void) {
    if (PIR2bits.TMR3IF == 1) {
        
        servo_counter = (servo_counter + 1u) % 30;
        
        /**** PWM logic for toggling on setpoint *******/
        SERVO_0_OUT = (unsigned)(servo_setpoint[0] > servo_counter);
        //SERVO_1_OUT = (unsigned)(servo_setpoint[1] > servo_counter);
        //SERVO_2_OUT = (unsigned)(servo_setpoint[2] > servo_counter);
        /**********************************************/
        
        if (servo_counter == 0) { 
            TMR3H = 215;
        }
        else if (servo_counter <= 25) {
            TMR3H = 0xFF;
            TMR3L = 0xD0;
        } else {
            TMR3H = 0;
        }
        
        PIR2bits.TMR3IF = 0;
    }
}

/********************************************************************/
/** P R I V A T E   D E C L A R A T I O N S *************************/
/********************************************************************
 * Function:        void OpenTimer3(unsigned char intEnable)
 * PreCondition:    None
 * Input:           intEnable: enable Timer3 interrupt
 * Output:          None
 * Overview:        Will initialize Timer3 given the parameters
 ********************************************************************/
static void OpenTimer3(unsigned char intEnable) {
    T3CONbits.T3RD16 = 0;
    T3CONbits.T3CKPS = 0b00;
    T3CONbits.TMR3ON = 1;
    T3CONbits.TMR3CS = 1;
    T3CONbits.T3SYNC = 1;
    IPR2bits.TMR3IP = 0;
          
    PIE2bits.TMR3IE = intEnable & 0x01u;
    RCONbits.IPEN = 1;
    INTCONbits.GIEH = (intEnable & 0x01u) | INTCONbits.GIEH;
    INTCONbits.GIEL = (intEnable & 0x01u) | INTCONbits.GIEL;
    PIR2bits.TMR3IF = 0;      // Clear Timer3 overflow flag
}

#endif
//EOF-----------------------------------------------------------------
