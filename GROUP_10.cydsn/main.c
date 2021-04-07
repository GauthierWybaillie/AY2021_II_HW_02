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
#define LED 6


const Color BLACK = {0, 0, 0};

volatile uint8_t packet;
volatile uint8_t flag;
volatile Color color;
volatile uint8_t status;    

uint8_t start;


int main(void)
{
    CyGlobalIntEnable;        /* Enable global interrupts. */
    
    //Start UART periferal
    UART_Start();                               
    
    // Start Led RGB
    RGBLed_Start();
    
    // Initialization ISR due to UART
    ISR_UART_StartEx(Custom_UART_RX_ISR);       
    
    // Initialization of the status to 0
    status = Idle;                             
    // Initialization of the variable start: it turns into one after the detection of the packet header
    start = 0;                                
  
    RGBLed_WriteColor(BLACK); // RGB LED start in OFF condition 
    
    
    for(;;)
    {
        switch (status){
            // Idle status --> it waits the arrival of an Interrupt to switch status
            case Idle: 
            start = 0;
            if (flag == 1){
                status = Header;
            }
            break;
            
            /*
            Header status -> Depending on the packet that is arrived from UART it can:
              - Print the string required for the GUI;
              - Switch to R (red) status if the Header packet is detected;
              - Go back to Idle state and wait for the right packet. 
            */
            case Header: 
            if (packet == 'v'){                          //Requirement for GUI
                UART_PutString("RGB LED Program $$$");
                status = Idle;
                flag = 0 ;
            }else if(packet == 160){                    // Control for Packet Header
               start = 1;
               status = R; 
               flag = 0 ;
            }else{ 
                flag = 0 ;                              
                status = Idle;
            }
            break;
            
            /* 
            R status -> If the packet header is detected and a data is arrived from UART,
            the value is saved in the RED component of the struct color. Then it puts the flag
            of the Interrupt to 0 and goes to the next status.
            */
            case R:
            if (start == 1 && flag == 1){
                color.red = packet;
                status = G;
                flag = 0;
            }
            break;
            
            /* 
            G status -> If the packet header is detected and a data is arrived from UART,
            the value is saved in the GREEN component of the struct color. Then it puts the flag
            of the Interrupt to 0 and goes to the next status.
            */
            case G:
            if (start == 1 && flag == 1){
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
            if (start == 1 && flag == 1){
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
                if (packet == 192){
                    status = LED;
                    flag = 0;
                }else{
                    status = Tail;
                    flag = 0;
                }
            }
            break;
            
            // LED status -> It turns on the LED and goes back to the IDLE status
            case LED:
            RGBLed_WriteColor(color);
            status = Idle;
            break;   
        }
    }
}

/* [] END OF FILE */
