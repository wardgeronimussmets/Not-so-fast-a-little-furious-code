/*********************************************************************
 *
 *                 Not so fast a little furious
 *
 *********************************************************************
 * Processor:       PIC18F25K50
 * Compiler:        XC8 1.35+
 * Author:          team little furious
 * Updated:         03/03/2020
 ********************************************************************/

#ifndef _CONFIG_H
#define _CONFIG_H

/** D E F I N E S ***************************************************/
#define TRUE 1
#define FALSE 0

/*** ADC Configuration *******************/
/* Uncomment the line below if you want to use the ADC */
// #define ENABLE_ADC TRUE

/* If you are using the ADC specify how many channels you are using
 * for every channel you can read the ADC value from ADC_value[] */
#define ADC_CHANNELS    1


/*** IO expander configuration ***********  
 * BE AWARE: When using the IO expander RC7 and RB1 are used for communication
 * RB0 can only be used as output, in this example we use it as CS */
// #define ENABLE_IOEXP TRUE 
/*
#define IOEXP_DATA          LATCbits.LATC7
#define IOEXP_CLK           LATBbits.LATB1
#define IOEXP_CS            LATBbits.LATB0*/


/*** Audio Configuration *****************/
 #define ENABLE_AUDIO TRUE

/* Specify the pin for your audio output, every digital output can be used */
#define AUDIO_OUT           LATAbits.LATA0


/*** Servo Configuration *****************/
// #define ENABLE_SERVO TRUE

/* If you are using a servo specify how many you are using
 * for every servo you need to alter the SERVO_ISR() method in func_servo.c 
 * to update the outputs for the servo's */

// #define SERVO_1_OUT LATBbits.LATB6
// #define SERVO_2_OUT LATBbits.LATB5

/* Define all out- and inputs with readable names */
//microController
#define PRG_BUTTON      PORTCbits.RC0
#define LED1_OUT        LATCbits.LATC1
#define LED2_OUT        LATCbits.LATC2
//controller
#define MULT_S0         LATBbits.LATB4
#define MULT_S1         LATBbits.LATB3
#define MULT_S2         LATBbits.LATB2
#define MULT_ENABLE     LATAbits.LATA4
#define CONTR_OUT1      PORTBbits.RB7
#define CONTR_OUT2      PORTCbits.RC6
//vu meter
#define CONTR_VU1       LATAbits.LATA7
#define CONTR_VU2       LATBbits.LATB5
//breakdown leds
#define CONTR_BD2       LATAbits.LATA6
#define CONTR_BD1       LATBbits.LATB6
//starting leds
#define START_LED_RED    LATAbits.LATA1
#define START_LED_GR     LATAbits.LATA2


//dc motor
#define DC1_OUT         LATAbits.LATA5
#define DC2_OUT         LATCbits.LATC2
#define DC_DIRECTION    LATAbits.LATA3 //als hoog gaan ze de ene richting uit en als laag de andere

//end loop switch
#define END_START_SWITCH2   PORTCbits.RC1

/** I N C L U D E S *************************************************/
#include <xc.h>
#include "func_adc.h"
#include "func_pwm.h"
#include "func_ioexp.h"
#include "func_audio.h"
#include "func_servo.h"

/** G L O B A L   P U B L I C   V A R I A B L E S *******************/
// when a variable is declared 'extern' it also has to be declared in
// the corresponding .c file without the 'extern' keyword
//motor
extern float DC1Fw_out = 0;
extern float DC2Fw_out = 0;
extern float DC1Bw_out = 0;
extern float DC2Bw_out = 0;
//break down led
extern unsigned char BDLED1_out = 0;
extern unsigned char BDLED2_out = 0;
//starting lights
extern unsigned char LEDGr_out = 0;
extern unsigned char LEDRed_out = 0;
//end loop switches

extern unsigned char ENDLOOP_FinishS = 0;
extern unsigned char ENDLOOP_StartS1 = 0;
extern unsigned char ENDLOOP_StartS2 = 0;

//booleans
extern unsigned char CAR1_BREAKDOWN = FALSE;
extern unsigned char CAR2_BREAKDOWN = FALSE;
extern unsigned char GAME_STARTED = FALSE;
extern unsigned char CARS_BACKINGUP = FALSE;


//input controller
extern unsigned char CONT1_GEAR1 = 0;
extern unsigned char CONT1_GEAR2 = 0;
extern unsigned char CONT1_GEAR3 = 0;
extern unsigned char CONT1_GEAR4 = 0;
extern unsigned char CONT1_GEAR5 = 0;
extern unsigned char CONT1_GEAR6 = 0;
extern unsigned char CONT1_CLUTCH = 0;

extern unsigned char CONT2_GEAR1 = 0;
extern unsigned char CONT2_GEAR2 = 0;
extern unsigned char CONT2_GEAR3 = 0;
extern unsigned char CONT2_GEAR4 = 0;
extern unsigned char CONT2_GEAR5 = 0;
extern unsigned char CONT2_GEAR6 = 0;
extern unsigned char CONT2_CLUTCH = 0;
// this is an example for implementing a stepper motor
// in your game FSM you can store the direction and amount of steps in a variable
// the IO FSM will read out this variable to set the apropriate outputs
// we use an array here, in case you have multiple stepper motors
//extern unsigned char stepper_steps[1];
//extern unsigned char stepper_direction[1];

// this is an example for implementing a PWM on one of the output pins
// you can configure the PWM output in the fsm_io.c file
//extern unsigned char PWM_duty[1];

/** P U B L I C   P R O T O T Y P E S *******************************/

/** F S M   R O U T I N E S *****************************************/
void fsm_io_init(void);
void fsm_io(void);

void fsm_game_init(void);
void fsm_game(void);

#endif
//EOF----------------------------------------------------------------

