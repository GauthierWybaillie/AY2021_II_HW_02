/* ========================================
 *
 * 
*/

#include "project.h"
#include "LedDriver.h"
#include "InterruptRoutines.h"
#include "stdio.h"
#include "UART.h"

#define Idle 0
#define Header 1
#define R 2
#define G 3
#define B 4
#define Tail 5
#define LED 6


const Color BLACK = {0, 0, 0};
volatile uint8_t status;
volatile uint8_t packet;

char message [20] = {'\0'};

uint8_t start;    
volatile Color color;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    UART_Start();                               //Start UART periferal
    
    RGBLed_Start();
   
    ISR_UART_StartEx(Custom_UART_RX_ISR);       // Initialization ISR
    
    status = 0;                                 // Initialization of varial header to 0
  
    //Configuration Time for timeout -> just one time
    
    for(;;)
    {
        switch (status){
            case Idle: 
            RGBLed_WriteColor(BLACK); //Idle condition: LED OFF
            start = 0;
            break;
            
           case Header:
            if(packet == 160){
               start = 1;
               UART_PutString("Transmission started!!\r\n");
            }else{
                UART_PutString("Transmission NOT started!!\r\n");
                status = Idle;
            }
            break;
            
            case R:
            if (start == 1){
                color.red = packet;
                sprintf(message, "\nRed value: %c\r\n", color.red);
                UART_PutString(message);
            }
            break;
            
            case G:
            if (start == 1){
                color.green = packet;
                sprintf(message, "\nGreen value: %c\r\n", color.green); 
                UART_PutString(message);
            }
            break;
            
            case B:
            if (start == 1){
                color.blue = packet;
                sprintf(message, "\nBlue value: %c\r\n", color.blue); 
                UART_PutString(message);
            }
            break;
            
            case Tail:
            if (packet == 192){
                status = LED;
            }else{
                UART_PutString("Invalid packet!!\r\n");
                status = Tail;
            }
            break;
            
            case LED:
            RGBLed_WriteColor(color);
            CyDelay(1000);
            UART_PutString("LED ACCESO!!\r\n");
            status = Idle;
            break;   
        }
    }
}

/* [] END OF FILE */
