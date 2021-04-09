/* ========================================
 * LEDDRIVER.h
 * Group 10 Antonella Buquicchio and Gauthier Wybaillie
 * 09/04/2021 
 * ========================================
*/
#ifndef __LED_DRIVER_H__
    #define __LED_DRIVER_H__
    
    #include "project.h" 
    
    /*
    Struct holding color data:
    We can control the three differen channel to create all the colors
    */
    typedef struct{ 
        uint8_t red;     //RED value (0-255)
        uint8_t green;   //GREEN value (0-255)
        uint8_t blue;    //BLUE value (0-255)
    } Color;
    
    // Start the PWM components driver
    void RGBLed_Start(void);
    
    // Stop the PWM components driver
    void RGBLed_Stop(void);
    
    //Write a new color to the RGB Led
    // c is the new color to be written and it is a variable of type Color 
    void RGBLed_WriteColor(Color c);
   
#endif


/* [] END OF FILE */
