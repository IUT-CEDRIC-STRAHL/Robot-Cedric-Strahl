#include <xc.h>
#include "timer.h"
#include "IO.h"
#include "main.h"
#include "PWM.h"
#include "QEI.h"

volatile unsigned long timestamp = 0; 

//Initialisation d?un timer 32 bits
void InitTimer23(void) 
{
    T3CONbits.TON = 0; // Stop any 16-bit Timer3 operation
    T2CONbits.TON = 0; // Stop any 16/32-bit Timer3 operation
    T2CONbits.T32 = 1; // Enable 32-bit Timer mode
    T2CONbits.TCS = 0; // Select internal instruction cycle clock
    T2CONbits.TCKPS = 0b00; // Select 1:1 Prescaler
    TMR3 = 0x00; // Clear 32-bit Timer (msw)
    TMR2 = 0x00; // Clear 32-bit Timer (lsw)
    PR3 = 0x0262; // Load 32-bit period value (msw)
    PR2 = 0x5A00; // Load 32-bit period value (lsw)
    IPC2bits.T3IP = 0x01; // Set Timer3 Interrupt Priority Level
    IFS0bits.T3IF = 0; // Clear Timer3 Interrupt Flag
    IEC0bits.T3IE = 1; // Enable Timer3 interrupt
    T2CONbits.TON = 1; // Start 32-bit Timer
    /* Example code for Timer3 ISR */
}

//Interruption du timer 32 bits sur 2-3
void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void)
{
    IFS0bits.T3IF = 0; // Clear Timer3 Interrupt Flag
}

void SetFreqTimer1(float freq)
{
    T1CONbits.TCKPS = 0b00; //00 = 1:1 prescaler value
    if(FCY /freq > 65535)
    {
        T1CONbits.TCKPS = 0b01; //01 = 1:8 prescaler value
        if(FCY /freq / 8 > 65535)
        {
            T1CONbits.TCKPS = 0b10; //10 = 1:64 prescaler value
                if(FCY /freq / 64 > 65535)
                {
                    T1CONbits.TCKPS = 0b11; //11 = 1:256 prescaler value
                    PR1 = (int)(FCY / freq / 256);
                }
            else
            PR1 = (int)(FCY / freq / 64);
        }
        else
        PR1 = (int)(FCY / freq / 8);
    }
    else
    PR1 = (int)(FCY / freq);
}

//Initialisation d?un timer 16 bits
void InitTimer1(void)
{
    //Timer1 pour horodater les mesures (1ms)
    T1CONbits.TON = 0; // Disable Timer
    T1CONbits.TCKPS = 0b10; //Prescaler
    //11 = 1:256 prescale value
    //10 = 1:64 prescale value
    //01 = 1:8 prescale value
    //00 = 1:1 prescale value
    T1CONbits.TCS = 0; //clock source = internal clock
    //PR1 =6250;

    SetFreqTimer1(FREQ_ECH_QEI); //set timer frequency at QUEI sampling frequency

    IFS0bits.T1IF = 0; // Clear Timer Interrupt Flag
    IEC0bits.T1IE = 1; // Enable Timer interrupt
    T1CONbits.TON = 1; // Enable Timer
}

void SetFreqTimer4(float freq) 
{ 
    T4CONbits.TCKPS = 0b00; //00 = 1:1 prescaler value 
    if (FCY / freq > 65535) 
    { 
        T4CONbits.TCKPS = 0b01; //01 = 1:8 prescaler value 
        if (FCY / freq / 8 > 65535) 
        { 
            T4CONbits.TCKPS = 0b10; //10 = 1:64 prescaler value 
            if (FCY / freq / 64 > 65535) 
            { 
                T4CONbits.TCKPS = 0b11; //11 = 1:256 prescaler value 
                PR1 = (int) (FCY / freq / 256); 
            } else 
                PR1 = (int) (FCY / freq / 64); 
        } else 
            PR1 = (int) (FCY / freq / 8); 
    } else 
        PR1 = (int) (FCY / freq); 
}


void InitTimer4(void) 
{ 
    //Timer4  
    T4CONbits.TON = 0; // Disable Timer 
    //T1CONbits.TCKPS = 0b01; //Prescaler 
    //T1CONbits.TCKPS = 0b00; 
    //11 = 1:256 prescale value 
    //10 = 1:64 prescale value 
    //01 = 1:8 prescale value 
    //00 = 1:1 prescale value 
    T4CONbits.TCS = 0; //clock source = internal clock 
    //PR1 = 0xC350; 
    //PR1 = 0x9C40; 
     
    SetFreqTimer4(1000); 
     
    IFS1bits.T4IF = 0; // Clear Timer Interrupt Flag 
    IEC1bits.T4IE = 1; // Enable Timer interrupt 
    T4CONbits.TON = 1; // Enable Timer 
} 
