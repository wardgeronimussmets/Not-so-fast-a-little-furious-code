/*********************************************************************
 *
 *                  Not so fast a little furious  
 *
 *********************************************************************
 * Processor:       PIC18F25K50
 * Compiler:        XC8 1.35+
 * Author:          Team A7
 * Updated:         03/03/2020
 * 
 ********************************************************************/
/** I N C L U D E S *************************************************/
#include "config.h"
#include <stdlib.h>

/** D E F I N E S ***************************************************/
#define PUSHED 1
#define RELEASED 0

#define HIGH 1
#define LOW 0
#define TRUE 1
#define FALSE 0

#define DCout 1

#define RedLDWait 13000  // we want 10 sec for the little show to happen in the beggining
#define GreenLDWait 20000 
#define BURSTWaitTime 20
#define BURSTWaitTimeOFF 100
#define VUWaitTime 1
#define VUWaitTimeOFF 40
#define VUTargetCounter 1000
#define maxBurst 500
#define GameEndWaitTime 5000

/** P R I V A T E   V A R I A B L E S *******************************/
static unsigned char greenLDWasOn = TRUE;
static unsigned char gear1 = 0;
static unsigned char gear2 = 0;
static unsigned char newGear1 = 0;
static unsigned char newGear2 = 0;
static unsigned char clutch1Was = RELEASED;
static unsigned char clutch2Was = RELEASED;

static unsigned int counter = 0; // highest possible number is 65K
static unsigned int counter1 = 0;
static unsigned int counter2 = 0;

static unsigned int counter7Hz1 = 0;
static unsigned int counter7Hz2 = 0;
static unsigned int increasinghzs = 8;
static unsigned int X = 0;
static unsigned int Y = 0;
static unsigned int Z = 0;
static unsigned int counter250 = 0;
static unsigned int counter500 = 0;
static unsigned int decreasinghzs = 25;

static unsigned char permission = FALSE;

static unsigned char gameWasWon = FALSE;
static unsigned char car1HasBD = FALSE;
static unsigned char car2HasBD = FALSE;

static unsigned char burst1Time = 0;
static unsigned char burst2Time = 0;
static unsigned int vuCounter1 = 0;
static unsigned int vuCounter2 = 0;
static unsigned int vuCounter1Limit = 0;
static unsigned int vuCounter2Limit = 0;
static unsigned int servoCounter1 = 0;
static unsigned int servoCounter2 = 0;
static unsigned char servo1DirectionRight = TRUE;
static unsigned char servPos = 0;

static unsigned char wasPRGBUTTON = 0;

static enum{FSM_GAME_IDLE,FSM_GAME_INITIALISE,FSM_GAME_GO,FSM_GAME_WAIT,FSM_GAME_GAMEOVER}current_state_game;

static enum {FSM_1_IDLE,FSM_1_FORWARD,FSM_1_BURST,FSM_1_BACKWARDS,FSM_1_GAMEOVER,FSM_1_BREAKDOWN
             } current_state_car1;

static enum{FSM_2_IDLE,FSM_2_FORWARD,FSM_2_BURST,FSM_2_BACKWARDS,FSM_2_GAMEOVER,FSM_2_BREAKDOWN
            } current_state_car2;
static enum{FSM_VU1_UP,FSM_VU1_IDLE,FSM_VU1_DOWN} current_state_vu1;
static enum{FSM_VU2_UP,FSM_VU2_IDLE,FSM_VU2_DOWN} current_state_vu2;
            
/********************************************************************
 * Function:        void fsm_game_init(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        This init sets the start state for this FSM and
 *                  may initialize some counters          
 ********************************************************************/
void fsm_game_init(void) {
	current_state_game = FSM_GAME_IDLE;
    current_state_car1 = FSM_1_IDLE;
    current_state_car2 = FSM_2_IDLE;
  
    counter = 0;
    DC2Fw_out = LOW;
    DC1Bw_out = LOW;
    DC2Bw_out = LOW;
    BDLED1_out = LOW;
    BDLED2_out = LOW;
    //SERVO_1_OUT = LOW;
    //SERVO_2_OUT = LOW;
    LEDGr_out = LOW;
    LEDRed_out = LOW;
    
    SERVO_setPosition(1,0);
}
void fsm_vu_init(void){

}

/********************************************************************
 * Function:        void fsm_game(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        An implementation for a simple reaction game
 ********************************************************************/
void fsm_vu(void){
    
}
void fsm_game(void) {
    
    switch (current_state_game) {                
        case FSM_GAME_IDLE:          
            AUDIO_stop();
            LEDGr_out = LOW;
            LEDRed_out = LOW;
            if(CONT1_CLUTCH == PUSHED || CONT2_CLUTCH == PUSHED)
            {
                current_state_game = FSM_GAME_INITIALISE;
                counter = 0;
            }
            break;
        case FSM_GAME_INITIALISE:
            // considering the fact that the case will be coming 10^3 every second.
            // 7HZ for 2sec. 
            counter7Hz1++; 
            counter7Hz2++;

            if (counter7Hz1 % 71 == 0 && counter7Hz1 < 2000) //Note: 71.5 * 14 = 1000. We use 14 because we need on and off.
            { AUDIO_OUT = (unsigned) !AUDIO_OUT;
            counter7Hz2 = 0;
            }

            //increase by 1 hz every 0.25 secs until 25hz. (for 4.5 secs)
            if (counter7Hz1 > 1999 && X < 13)
            {
            X = 0.25 * 2 * increasinghzs; //initial value of increasinghzs is 8hzs
            Y = 250/X;
            counter250++;
            Z = (unsigned int) Y;
            if (counter7Hz2 % Z == 0)
            { AUDIO_OUT = (unsigned) !AUDIO_OUT;

            }
            if (counter250 == 250)
            { counter7Hz2 = 0;
                    counter250 = 0;
                    increasinghzs++;
            }
            }
        
            if (X > 13)
            { permission = TRUE;
            }
           
            //decrease by 1hz every 0.5 secs until to 7hz
             if (X > 3 && permission)
            {
                X = 0.25 * 2 * decreasinghzs; //initial value of the decreasing hzs is 25hzs
            Y = 500/X;
            Z = (unsigned int) Y;
            counter500++;
            if (counter7Hz2 % Z == 0)
            { AUDIO_OUT = (unsigned) !AUDIO_OUT;

            }
            if (counter500 == 500)
            { counter7Hz2 = 0;
                    
                    counter500 = 0;
                    decreasinghzs--;
            }
            }
            
             if (decreasinghzs == 7)
                   { counter7Hz1 = 0; }
               
            
           
            LEDRed_out = HIGH;
            
            counter ++;
            if(counter > RedLDWait)
            {
                current_state_game = FSM_GAME_GO;
                counter = 0;
            }
            break;
        case FSM_GAME_GO:
            AUDIO_play(D2);
            LEDRed_out = LOW;
            LEDGr_out = HIGH;
            greenLDWasOn = FALSE;
            GAME_STARTED = TRUE;
            //check if green led needs to be on
           if(!greenLDWasOn)
           {
               counter ++;
               if(counter > GreenLDWait)
               {
                   greenLDWasOn = TRUE;
                   LEDGr_out = LOW;
                   current_state_game = FSM_GAME_WAIT;
               }
           }  
            break;
        case FSM_GAME_WAIT:
            if((current_state_car1 == FSM_1_IDLE)&&(current_state_car2 == FSM_2_IDLE))
            {
                gear1 = 0;
                gear2 = 0;
                current_state_game = FSM_GAME_IDLE;
                void setDirectionDCFW();
            }
            break;
        default:
            current_state_game = FSM_GAME_IDLE;
            break;
    }
    //check if car has finished or if two cars have broken down
    if(((car1HasBD == TRUE) && (car2HasBD == TRUE))||(gameWasWon== TRUE))
    {
        current_state_car1 = FSM_1_BACKWARDS;
        current_state_car2 = FSM_1_BACKWARDS;
        void setDirectionDCBW();
    }
    /*********************************************************************************************************************/
    switch(current_state_car1)
    {
        
        case FSM_1_IDLE:
            DC1Bw_out = LOW;
            DC1Fw_out = LOW;
            BDLED1_out = LOW;
            
            if((GAME_STARTED==TRUE) && (CONT1_GEAR1 == PUSHED) && (CONT1_CLUTCH == RELEASED)) 
            {
                
                current_state_car1 = FSM_1_FORWARD;
                gear1 = 1;
            }
            else if((GAME_STARTED == FALSE)&&(CONT1_GEAR1 == PUSHED) && (CONT1_CLUTCH == RELEASED))
            {
                //false start
                current_state_car1 = FSM_1_BREAKDOWN;
            }
            break;
        case FSM_1_FORWARD:
           
           DC1Fw_out = 0.5f*DCout; 
           
          
           //check if a car has finished
           if(ENDLOOP_FinishS == PUSHED)
               current_state_car1 = FSM_1_GAMEOVER;
           
           
            //check if car has shifted a gear          
            newGear1 = 0;
            if(CONT1_GEAR1 ==  PUSHED) newGear1 =  1;
            if(CONT1_GEAR2 ==  PUSHED) newGear1 =  2;
            if(CONT1_GEAR3 ==  PUSHED) newGear1 =  3;
            if(CONT1_GEAR4 ==  PUSHED) newGear1 =  4;
            if(CONT1_GEAR5 ==  PUSHED) newGear1 =  5;
            if(CONT1_GEAR6 ==  PUSHED) newGear1 =  6;
            
            
            
        if(newGear1 == gear1 || newGear1 == 0 ||CONT1_CLUTCH == PUSHED )
        {
            //no gear change
            
        }
        else
        {
            //gear1 has been changed
            if(newGear1 - gear1 == 1  && clutch1Was == PUSHED)
            {
                //correct shift
                vuCounter1Limit = 0;
                counter1 = 0;
                //determine length of burst
                if(VUTargetCounter-vuCounter1Limit>0)
                    burst1Time = maxBurst - (VUTargetCounter-vuCounter1Limit);
                else
                    burst1Time = maxBurst + (VUTargetCounter-vuCounter1Limit);
                current_state_car1 = FSM_1_BURST;
                gear1 = newGear1;
            }
            else
            {
                //breakdown
                current_state_car1 = FSM_1_BREAKDOWN;
            }
        }
        
           clutch1Was = CONT1_CLUTCH; 
    
        break;
        case FSM_1_BURST:
            DC1Fw_out = DCout;
            counter1 ++;
            LEDRed_out = HIGH;
            if(counter1>BURSTWaitTime+burst1Time)
            {
                current_state_car1 = FSM_1_FORWARD;
                LEDRed_out = LOW;
            }
            
            break;
        case FSM_1_BREAKDOWN:
            DC1Fw_out = LOW;
            CAR1_BREAKDOWN = TRUE;
            car1HasBD = TRUE;
            BDLED1_out = HIGH;
            
            break;
            
        case FSM_1_GAMEOVER:
            //a car has finished
            gameWasWon = TRUE;
            counter1 ++;
            if(counter1>GameEndWaitTime)
            {
                current_state_car1 = FSM_1_BACKWARDS;
            }
            break;
            
        case FSM_1_BACKWARDS:
            DC1Fw_out = LOW;
            DC1Bw_out = DCout;
            if(ENDLOOP_StartS1 == TRUE)
            {
                current_state_car1 = FSM_1_IDLE;
            }
            break;
        default:
            current_state_car1 = FSM_1_IDLE;
            break;
    }
    /*******************************************************************************************************************************************/
    
      switch(current_state_car2)
    {
        case FSM_2_IDLE:
            DC2Bw_out = LOW;
            DC2Fw_out = LOW;
            //BDLED2_out = LOW;
            if((GAME_STARTED==TRUE) && (CONT2_GEAR1 == PUSHED)) 
            {
                current_state_car2 = FSM_2_FORWARD;
                gear2 = 1;
            }
            else if((GAME_STARTED == FALSE)&&(CONT2_GEAR1 == PUSHED))
            {
                //false start
                //current_state_car2 = FSM_2_BREAKDOWN;
            }
            break;
        case FSM_2_FORWARD:
            
           DC2Fw_out = 0.5*DCout;  
           //check if a car has finished
           if(ENDLOOP_FinishS == PUSHED)
               current_state_car2 = FSM_2_GAMEOVER;
           
           
            //check if car has shifted a gear          
            newGear2 = 0;
            if(CONT2_GEAR1 ==  PUSHED) newGear2 =  1;
            if(CONT2_GEAR2 ==  PUSHED) newGear2 =  2;
            if(CONT2_GEAR3 ==  PUSHED) newGear2 =  3;
            if(CONT2_GEAR4 ==  PUSHED) newGear2 =  4;
            if(CONT2_GEAR5 ==  PUSHED) newGear2 =  5;
            if(CONT2_GEAR6 ==  PUSHED) newGear2 =  6;
            
        if(newGear2 == gear2)
        {
            //no gear change
            
        }
        else
        {
            //gear1 has been changed
            if(newGear2 - gear2 == 1 && CONT2_CLUTCH == PUSHED )
            {
                //correct shift
                counter2 = 0;
                current_state_car2 = FSM_2_BURST;
                gear2 = newGear2;
            }
            else
            {
                //breakdown
                current_state_car2 = FSM_2_BREAKDOWN;
            }
        }
            
            
    
        break;
        case FSM_2_BURST:
            DC2Fw_out = DCout;
            counter2 ++;
            if(counter2>BURSTWaitTime+burst2Time)
            {
                current_state_car2 = FSM_2_FORWARD;
            }
            
            break;
        case FSM_2_BREAKDOWN:
            DC2Fw_out = LOW;
            CAR2_BREAKDOWN = TRUE;
            car2HasBD = TRUE;
            BDLED2_out = HIGH;
            break;
        case FSM_2_GAMEOVER:
            //a car has finished
            gameWasWon = TRUE;
            counter2 ++;
            if(counter2>GameEndWaitTime)
            {
                current_state_car2 = FSM_2_BACKWARDS;
            }
            break;
        case FSM_2_BACKWARDS:
            DC2Fw_out = LOW;
            DC2Bw_out = DCout;
            if(ENDLOOP_StartS2 == TRUE)
            {
                current_state_car2 = FSM_2_IDLE;
            }
            break;
        default:
            current_state_car2 = FSM_2_IDLE;
            break;
    } 
    
      /*******************Difficulty with vu************************************/
    /*//vu1
    if(current_state_car1 == FSM_1_FORWARD || current_state_car1 == FSM_1_BURST){
        if(SERVO_1_OUT == HIGH){
            if(vuCounter1 > VUWaitTime + vuCounter1Limit){
                SERVO_1_OUT = LOW;
                vuCounter1Limit++;
            }
        }
        else{
            if(vuCounter1 > VUWaitTimeOFF){
                SERVO_1_OUT = HIGH;
                vuCounter1Limit ++;
            }
        }
    }
    else{
        SERVO_1_OUT = LOW;
    }
     
      if(CONT1_CLUTCH == PUSHED) vuCounter1Limit = 0;
      
      vuCounter1++;
      SERVO_1_OUT = HIGH;
      
      
      
      //vu2
    */
      
     //servo test
     /* 
      if(servoCounter1 > 500){
        servoCounter1 = 0;  
        if(LED1_OUT == 1)
            LED1_OUT = 0;
        else
            LED1_OUT = 1;
        unsigned char servPos = SERVO_getPosition(1);
        if(servPos>24){
           servo1DirectionRight = FALSE;
           LED2_OUT = 1;
        }
         if(servPos<1)
            servo1DirectionRight = TRUE;
        if(servo1DirectionRight)
                SERVO_setPosition(1,servPos +1);
        else
            SERVO_setPosition(1,servPos +1);
      }
      
      
      servoCounter1++;
       */
      
      if(PRG_BUTTON == PUSHED && wasPRGBUTTON == RELEASED){
          //servPos = SERVO_getPosition(1);
          //SERVO_setPosition(1,servPos +1);
          
          
          
          if(LED1_OUT == 1){
            LED1_OUT = 0;
            SERVO_setPosition(1,20);
            SERVO_setPosition(2,20);
          }
          else{
            LED1_OUT = 1;
            SERVO_setPosition(1,0);
            SERVO_setPosition(2,0);
          }
          
      }
      //else SERVO_1_OUT = LOW;
      wasPRGBUTTON =  PRG_BUTTON;
          
}
