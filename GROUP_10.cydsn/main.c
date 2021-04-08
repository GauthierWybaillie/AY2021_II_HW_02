/* ========================================
 *
 * 
*/

#include "project.h"
#include "LedDriver.h"
#include "InterruptRoutines.h"
#include "UART.h"

#define Idle 0
#define Header 1
#define R 2
#define G 3
#define B 4
#define Tail 5
#define Time_Config 6
#define End_Time_Config 7


const Color BLACK = {0, 0, 0};
const Color RED = {255, 0, 0};
const Color GREEN = {0, 255, 0};
const Color BLUE = {0, 0, 255};
const Color PURPLE = {255, 0, 255};
const Color YELLOW = {255, 255, 0};


volatile uint8_t packet;
volatile uint8_t flag;
volatile Color color;
volatile uint8_t status;
volatile uint8_t period;
volatile uint8_t flag_GUI;


int main(void)
{
    CyGlobalIntEnable;        /* Enable global interrupts. */
    
    //Start UART periferal
    UART_Start();                               
    
    // Start Led RGB
    RGBLed_Start();
    
    // Initialization ISR due to UART
    ISR_UART_StartEx(Custom_UART_RX_ISR);       
    
    Timer_Start();
    isr_timer_StartEx(Custom_TIMER_ISR);
    
    // Initialization of the status to 0
    status = Idle;                             
    
    period = 5;
    flag_GUI = 0;
    
    RGBLed_WriteColor(BLACK); // RGB LED start in OFF condition 
    
    for(;;)
    {
        switch (status){
            // Idle status --> it waits the arrival of an Interrupt to switch status
            case Idle:
            //RGBLed_WriteColor(BLACK);
            if (flag == 1){
                status = Header;
                flag = 0 ; 
            }
            break;

            /*
            Header status -> Depending on the packet that is arrived from UART it can:
              - Print the string required for the GUI;
              - Switch to R (red) status if the Header packet is detected;
              - Go back to Idle state and wait for the right packet. 
            */
            case Header: 
            if(packet == 'v'){                          //Requirement for GUI
                Timer_Stop();
                UART_PutString("RGB LED Program $$$");
                if (flag_GUI == 1){
                    flag_GUI = 0;
                }else{
                    flag_GUI = 1;
                    Timer_Start();
                }
                status = Idle;
            }else if(packet == 160){                    // Control for Packet Header
               //RGBLed_WriteColor(PURPLE); 
               status = R; 
            }else{                               
                status = Idle;
            }
            break;
            
            /* 
            R status -> If the packet header is detected and a data is arrived from UART,
            the value is saved in the RED component of the struct color. Then it puts the flag
            of the Interrupt to 0 and goes to the next status.
            */
            case R:
            if (flag == 1){
                flag = 0;
                //RGBLed_WriteColor(RED);
                color.red = packet;
                status = G;

            }
            break;
            
            /* 
            G status -> If the packet header is detected and a data is arrived from UART,
            the value is saved in the GREEN component of the struct color. Then it puts the flag
            of the Interrupt to 0 and goes to the next status.
            */
            case G:
            if (flag == 1){
                //RGBLed_WriteColor(GREEN);
                color.green = packet;
                status = B;
                flag = 0;
            }
            break;
            
            /* 
            B status -> If the packet header is detected and a data is arrived from UART,
            the value is saved in the BLUE component of the struct color. Then it puts the flag
            of the Interrupt to 0 and goes to the next status.
            */
            case B:
            if (flag == 1){
                //RGBLed_WriteColor(BLUE);
                color.blue = packet;
                flag = 0;
                status = Tail;
            }
            break;
            
            /*
            Tail status -> it waits the arrival of an interrupt and then controls if the 
            data corresponds to the packet tail. If the transmission is completed, the status 
            is switched to LED
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
            
            case Time_Config:
            if (flag == 1){
                flag = 0;
                //RGBLed_WriteColor(YELLOW);
                Timer_Stop();
                if (packet >= 1 && packet <= 20){
                    period = packet;
                    status = End_Time_Config;    
                }else{
                    status = Time_Config;
                }
            break;
            
            case End_Time_Config:
                if (flag == 1){
                    flag = 0;
                    if (packet == 192){
                        Timer_WritePeriod(period*clock_freq);
                        status = Idle;
                    }else{
                        
                    status = End_Time_Config;
                    }
                }
            }
            }
        }
    }

/* [] END OF FILE */