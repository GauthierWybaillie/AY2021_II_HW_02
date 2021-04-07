/* ========================================
 *
 * INTERRUPT_TIMER.c
 *
 * ========================================
*/
#include "project.h"
#include "InterruptTimer.h"

extern volatile uint8_t status;

#define Idle 0

void Timer_Reset(int16 timeout){
    Timer_Stop();
    Timer_WriteCounter(timeout);
    Timer_Start();
    
}


CY_ISR(Custom_TIMER_ISR){
    
    // when interrupt is triggered, we go back to the Idle state
    status = Idle; 
    
}

/* [] END OF FILE */
