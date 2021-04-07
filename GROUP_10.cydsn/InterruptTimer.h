/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#ifndef __INTERRUPT_TIMER_H__
   
    #define __INTERRUPT_TIMER_H__
    
    
    #include "project.h"
    
    void Timer_Reset(int16 timeout);
    
    
    CY_ISR_PROTO(Custom_TIMER_ISR);
    
    
#endif



/* [] END OF FILE */
