/* ========================================
 *
 * 
 */

#include "project.h"
#include "InterruptRoutines.h"
#include "stdio.h"
#include "UART.h"
#include "Timer.h"

extern volatile uint8_t status;
extern volatile uint8_t packet;
extern volatile uint8_t flag;
extern volatile uint8_t period;
extern volatile uint8_t flag_GUI;

#define Idle 0
#define Time_Config 6


CY_ISR(Custom_UART_RX_ISR){ 
    // Interrupt start if there is FIFO register not empty
    if (UART_ReadRxStatus() == UART_RX_STS_FIFO_NOTEMPTY && flag_GUI == 0){
        /*
        We have some data in the FIFO: Read the RX data
        */ 
        packet = UART_ReadRxData();
        if (packet == 161 && status == Idle){
            Timer_Stop();
            status = Time_Config;
        }else{
            flag = 1;
            Timer_Reset(period*clock_freq);
        }    
    }
   if (UART_ReadRxStatus() == UART_RX_STS_FIFO_NOTEMPTY && flag_GUI == 1){
       packet = UART_ReadRxData();
       flag = 1;
   }
}

void Timer_Reset(int16 timeout){
    Timer_Stop();
    Timer_WriteCounter(timeout);
    Timer_Start();
    
}

CY_ISR(Custom_TIMER_ISR){

    Timer_ReadStatusRegister();
    UART_PutString("\n OH NO\n\r");
    // when interrupt is triggered, we go back to the Idle state
    status = Idle; 
    
}

/* [] END OF FILE */