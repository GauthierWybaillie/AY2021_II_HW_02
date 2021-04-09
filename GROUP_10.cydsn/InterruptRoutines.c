/* ========================================
 * INTERRUPTROUTINES.c
 * Group 10 Antonella Buquicchio and Gauthier Wybaillie
 * 09/04/2021
 * 
 * This file contains the interrupt of the UART and the Timer. 
 *   The UART ISR is invoked every time new data is received. 
     All of the commands in the Idle state are handled by the interrupt.
         Different packets are discriminated:
         If the packet contains a packet header (0xA0), the configuration of the LED is continued.
         If the packet contains the timeout packet header (0xA1), the timeout period is adjusted.
         If the packet contains the character "v", a string is transmitted to the serial port.
     Otherwise (not in the Idle state), the code is continued and the timer is reset.
 *  The Timer ISR is triggered when the terminal count is reached.
    The status register is cleared in order to allow future interrupts.
    The code returns to the Idle state.
    
 * Timer_Reset(timeout) is a function.
   It is responsible for the start of the timer with a proper period every time new data is received. 
 * ========================================
*/

#include "project.h"
#include "InterruptRoutines.h"
#include "UART.h"
#include "Timer.h"

volatile uint8_t status;
volatile uint8_t packet;
volatile uint8_t flag;
volatile uint8_t period;


CY_ISR(Custom_UART_RX_ISR){ 
    // Interrupt start if there is FIFO register not empty
    if (UART_ReadRxStatus() == UART_RX_STS_FIFO_NOTEMPTY){
        
        // We have some data in the FIFO: Read the RX data
         
        packet = UART_ReadRxData();
        if (status == Idle){
            if (packet == 161){
                Timer_Stop();
                status = Time_Config;
            }else if(packet == 'v'){
                Timer_Stop();
                UART_PutString("RGB LED Program $$$");
                status = Idle;
            }else if (packet == 160){
                Timer_Reset(period*clock_freq);
                status = R;
            }else status = Idle;
        }else{
            Timer_Reset(period*clock_freq);
            flag = 1;
        }    
    }
}


void Timer_Reset(int16 timeout){
    Timer_Stop();
    Timer_WriteCounter(timeout);
    Timer_Start();    
}

CY_ISR(Custom_TIMER_ISR){

    Timer_ReadStatusRegister();
    // when interrupt is triggered, we go back to the Idle state
    status = Idle;    
}

/* [] END OF FILE */