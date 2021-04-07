/* ========================================
 *
 * 
*/

#include "project.h"
#include "InterruptRoutines.h"
#include "stdio.h"
#include "UART.h"
#include "Timer.h"


extern volatile uint8_t packet;
extern volatile uint8_t flag;

CY_ISR(Custom_UART_RX_ISR){ 
    // Interrupt start if there is FIFO register not empty
    if (UART_ReadRxStatus() == UART_RX_STS_FIFO_NOTEMPTY){
        
        /*
        We have some data in the FIFO: Read the RX data
        */ 
        flag = 1;
        packet = UART_ReadRxData();
        Timer_Start();
        }    
    }

/* [] END OF FILE */