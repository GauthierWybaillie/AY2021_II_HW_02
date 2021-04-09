/* ========================================
 * INTERRUPTROUTINES.h 
 * Group_10: Antonella Buquicchio and Gauthier Wybaillie
 * 09/04/2021
 * ========================================
*/

// Declaration of ISR
#ifndef __INTERRUPT_ROUTINES_H__
   
    #define __INTERRUPT_ROUTINES_H__
    
    #include "project.h"
    #include "LedDriver.h"
    
    #define clock_freq 1000
    
    // Status 
    #define Idle 0
    #define Header 1
    #define R 2
    #define G 3
    #define B 4
    #define Tail 5
    #define Time_Config 6
    #define End_Time_Config 7
    
    // Interrupts and Funtion Timer_Reset declaration
    CY_ISR_PROTO(Custom_UART_RX_ISR);
    
    void Timer_Reset(int16 timeout);
    
    CY_ISR_PROTO(Custom_TIMER_ISR);
#endif

/* [] END OF FILE */
