/* ========================================
 * MAIN.c 
 * Group_10: Antonella Buquicchio and Gauthier Wybaillie
 * 09/04/2021
 * 
 * The code is organised in multiple states.
 * First there is an initalisation state Idle. This state is followed by gathering of 
   the brightness of the different LEDs. The Tail state ends assembles the colours. 
 * Besides these states, there are also two states for changing the timeout value and the 
   Timer interrupt period. 
 * The communication between the main.c and InterruptRoutines.c is managed by the volatile flag.
 * =========================================
*/

#include "project.h"
#include "InterruptRoutines.h"
#include "LedDriver.h"
#include "Timer.h"

const Color BLACK = {0, 0, 0};

extern volatile uint8_t status;
extern volatile uint8_t packet;
extern volatile uint8_t flag;
extern volatile int16 period;

Color color;
uint8_t timeout;

int main(void)
{
    CyGlobalIntEnable;        /* Enable global interrupts. */
    
    // Start UART periferal
    UART_Start();                               
    
    // Start Led RGB
    RGBLed_Start();
    
    // Start Timer periferal
    Timer_Start();
    
    // Initialization ISR due to UART
    ISR_UART_StartEx(Custom_UART_RX_ISR);       

    isr_timer_StartEx(Custom_TIMER_ISR);
    
    // Initialization of the status to 0
    status = Idle;                             
    
    // Initialization of the timeout value to 5 seconds
    timeout = 5;
    
    // RGB LED start in OFF condition 
    RGBLed_WriteColor(BLACK);

    for(;;)
    {
        switch (status){
            
            // Idle -> It waits the arrival of an Interrupt to switch status.

            /* 
            R -> If the packet header is detected and a data is arrived from UART,
            the value is saved in the RED component of the struct color. Then, it puts the flag
            of the Interrupt to 0 and goes to the next status.
            */
            case R:
            if (flag == 1){
                flag = 0;
                color.red = packet;
                status = G;

            }
            break;
            
            /* 
            G -> If the packet header is detected and a data is arrived from UART,
            the value is saved in the GREEN component of the struct color. Then, it puts the flag
            of the Interrupt to 0 and goes to the next status.
            */
            case G:
            if (flag == 1){
                flag = 0;
                color.green = packet;
                status = B;
            }
            break;
            
            /* 
            B -> If the packet header is detected and a data is arrived from UART,
            the value is saved in the BLUE component of the struct color. Then, it puts the flag
            of the Interrupt to 0 and goes to the next status.
            */
            case B:
            if (flag == 1){
                flag = 0;
                color.blue = packet;
                status = Tail;
            }
            break;
            
            /*
            Tail -> it waits the arrival of an interrupt and then controls if the 
            data corresponds to the packet tail. If the transmission is completed, the colour 
            of the LED is adjusted and the code goes back to the Idle state.
            */
            case Tail:
            if (flag == 1){
                flag = 0;
                if (packet == 192){
                    RGBLed_WriteColor(color);
                    status = Idle;
                }else{
                    status = Tail;
                    
                }
            }
            break;
            
            /*
            Time_Config -> If in the Idle state, the time configuration header (0xA1) is received, the state
            is switched to Time_Config. The Timer is stopped in order for the user to have enough
            time to change the period. If the packet contains an acceptable value, it is stored 
            as the new period. The state is changed to End_Time_Config. Otherwise, it waits for the 
            new value in the Time_Config state. 
            */
            
            case Time_Config:
            if (flag == 1){
                flag = 0;
                Timer_Stop();
                if (packet >= 1 && packet <= 20){
                    timeout = packet;
                    period = timeout*clock_freq;
                    status = End_Time_Config;
                }else{
                    status = Time_Config;
                }
            }
            break;
            
            /*
            End_Time_Config -> Once the period has been set, the code waits for the
            tail byte (0xC0). When the packet is right, the period is updated. The code
            goes back to the Idle state. 
            */
                
            case End_Time_Config:
                if (flag == 1){
                    Timer_Stop();
                    flag = 0;
                    if (packet == 192){
                        Timer_WriteCounter(period - 1);
                        Timer_WritePeriod(period - 1);
                        Timer_Start();
                        status = Idle;
                    }else{
                        status = End_Time_Config;
                    }
                }
            }
        }
    }

/* [] END OF FILE */
