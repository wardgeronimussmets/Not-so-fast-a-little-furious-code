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
#include "func_adc.h"
#if ENABLE_ADC == TRUE

/** P U B L I C   V A R I A B L E S *********************************/
// in order for the variable to be used in other file, it also has to
// be declared as 'extern' in the corresponding .h file
unsigned int ADC_value[ADC_CHANNELS];

/** P R I V A T E   V A R I A B L E S *******************************/
// 'static' implies that the variable can only be used in this file
// (cfr. 'private' in Java)
static char current_channel;

/** P R I V A T E   P R O T O T Y P E S *****************************/
// 'static' implies that the function can only be used in this file
// (cfr. 'private' in Java)
static void openADC(unsigned char channels, unsigned char intEnable);
static unsigned int readADC(void);
static void setChanADC(unsigned char channel);
static char busyADC(void);

/********************************************************************/
/** P U B L I C   D E C L A R A T I O N S ***************************/
/********************************************************************
 * Function:        void ADC_init(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        Initializes A/D module and input channels
 ********************************************************************/
void ADC_init(void) {
	openADC(ADC_CHANNELS, TRUE);
    current_channel = 0;
}

/********************************************************************
 * Function:        void ADC_ISR(void)
 * PreCondition:    ADC_init()
 * Input:           None
 * Output:          None
 * Overview:        ADC Interrupt service routine will process all ADC
 *                  data and load it in the ADC_value array
 ********************************************************************/
void ADC_ISR(void) {
    if (PIR1bits.ADIF == 1) {
        if (!busyADC()) {
            ADC_value[current_channel] = readADC();
            current_channel++;
            if (current_channel >= ADC_CHANNELS) {
                current_channel = 0;
                setChanADC(current_channel);
            } else {
                setChanADC(current_channel);
                ADCON0bits.GO = 1;
            }
        }
        PIR1bits.ADIF = 0;
    }
}

/********************************************************************/
/** P R I V A T E   D E C L A R A T I O N S *************************/
/********************************************************************
 * Function:        void openADC(unsigned char channels, 
 *                               unsigned char intEnable)
 * PreCondition:    None
 * Input:           channels: the amount of ADC channels that will be
 *                            activated
 *                  intEnable: enable ADC interrupt
 * Output:          None
 * Overview:        Will initialize the ADC given the parameters
 ********************************************************************/
static void openADC(unsigned char channels, unsigned char intEnable) {
    ADCON0bits.CHS = 0;
    ADCON1bits.NVCFG = 0;
    ADCON1bits.PVCFG = 0;
    ADCON2bits.ACQT = 0b111;
    ADCON2bits.ADCS = 0b101;
    ADCON2bits.ADFM = 1;
    
    if (channels >= 1) ANSELAbits.ANSA0 = 1;
    if (channels >= 2) ANSELAbits.ANSA1 = 1;
    if (channels >= 3) ANSELAbits.ANSA2 = 1;
    if (channels >= 4) ANSELAbits.ANSA3 = 1;
    if (channels >= 5) ANSELAbits.ANSA5 = 1;

    PIR1bits.ADIF = 0;
    IPR1bits.ADIP = 0;
    PIE1bits.ADIE = (intEnable & 0x01u);
    RCONbits.IPEN = 1;
    INTCONbits.GIEH = (intEnable & 0x01u) | INTCONbits.GIEH;
    INTCONbits.GIEL = (intEnable & 0x01u) | INTCONbits.GIEL;
    
    ADCON0bits.ADON = 1;
    ADCON0bits.GO = 1;
}

/********************************************************************
 * Function:        int readADC(void)
 * PreCondition:    startADC()
 * Input:           None
 * Output:          ADC value of last conversion
 * Overview:        Loads the result from the ADC into a variable
 ********************************************************************/
static unsigned int readADC(void) {
  return (((unsigned int)ADRESH)<<8)|(ADRESL);
}

/********************************************************************
 * Function:        void setChanADC(unsigned char channel)
 * PreCondition:    openADC(...)
 * Input:           channel : the requested ADC channel
 * Output:          None
 * Overview:        Sets the active channel for the next conversion
 ********************************************************************/
static void setChanADC(unsigned char channel) {
  ADCON0bits.CHS = channel;
}

/********************************************************************
 * Function:        char busyADC(void)
 * PreCondition:    openADC(...)
 * Input:           None
 * Output:          TRUE in case the conversion is still running
 * Overview:        Reads the status of the ADC module
 ********************************************************************/
static char busyADC(void) {
  return(ADCON0bits.GO);
}

/********************************************************************
 * Function:        void startADC(void)
 * PreCondition:    openADC(...)
 * Input:           None
 * Output:          None
 * Overview:        Starts the AD conversion
 ********************************************************************/
void startADC(void) {
  ADCON0bits.GO = 1;
}

#endif
//EOF-----------------------------------------------------------------
