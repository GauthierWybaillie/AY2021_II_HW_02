/* ========================================
 * LEDDRIVER.c
 * Group_10: Antonella Buquicchio and Gauthier Wybaillie
 * 09/04/2021
 * 
 * Basic functions for control of the LED are defined in this file.
 * ========================================
*/

#include "project.h"
#include "LedDriver.h" //Header file

extern volatile Color color;

//Accessible only for this source file 
static void RGBLed_WriteRed(uint8_t red);
static void RGBLed_WriteGreen(uint8_t green);
static void RGBLed_WriteBlue(uint8_t blue);

//Implementation RGBLed_Start function: include the two PWM
void RGBLed_Start(){
    
    PWM_RG_Start();
    PWM_B_Start();
}

//Implementation RGBLed_Stop function: include the two PWM
void RGBLed_Stop(){
    
    PWM_RG_Stop();
    PWM_B_Stop();
}

//Switch on the red channel with the corresponding value
static void RGBLed_WriteRed (uint8_t red){
    PWM_RG_WriteCompare1(red);
}

//Switch on the green channel with the corresponding value
static void RGBLed_WriteGreen (uint8_t green){
    PWM_RG_WriteCompare2(green);
}

//Switch on the blue channel with the corresponding value
static void RGBLed_WriteBlue (uint8_t blue){
    PWM_B_WriteCompare(blue);
}

void RGBLed_WriteColor(Color c){
    RGBLed_WriteRed(c.red);
    RGBLed_WriteGreen(c.green);
    RGBLed_WriteBlue(c.blue);
}

/* [] END OF FILE */
