/* ========================================
 *
 * 
*/

// Declaration of ISR
#ifndef __INTERRUPT_ROUTINES_H__
   
    #define __INTERRUPT_ROUTINES_H__
    
    #include "project.h"
    
    #define clock_freq 1000
    
    CY_ISR_PROTO(Custom_UART_RX_ISR);
    
    void Timer_Reset(int16 timeout);
    
    CY_ISR_PROTO(Custom_TIMER_ISR);
#endif

/* [] END OF FILE */
