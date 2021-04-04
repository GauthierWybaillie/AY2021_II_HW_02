/* ========================================
 *
 * 
*/

#include "project.h"
#include "InterruptRoutines.h"
#include "stdio.h"
#include "UART.h"

extern volatile uint8_t status;
extern volatile uint8_t packet;

CY_ISR(Custom_UART_RX_ISR){ 
    // Interrupt start if there is FIFO register not empty
    if (UART_ReadRxStatus() == UART_RX_STS_FIFO_NOTEMPTY){
        /*
        We have some data in the FIFO: Read the RX data
        */
        packet = UART_ReadRxData();
        status++;
        }    
    }

/* [] END OF FILE */
