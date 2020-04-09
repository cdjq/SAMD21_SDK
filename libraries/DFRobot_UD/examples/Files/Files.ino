#include "Arduino.h"
#include "DFRobot_IL0376F_SPI.h"
DFRobot_IL0376F_SPI eink;

#define EINK_CS  D3
#define Font_CS  D6
#define EINK_DC  D8
#define BUSY     D7

void setup(void)
{
    Serial.begin(115200);
    //Select the corresponding pins
    eink.begin(EINK_CS, Font_CS, EINK_DC, BUSY);

    //Clear the screen and display white
    eink.fillScreen(WHITE);
    //Displays a string, red font
    eink.disString(12,12,1,"DFRobot EINK 1234567890,!@#$%^&*()-+=",RED);
    //Refresh screen display
    eink.flush();
 
}

void loop(void)
{
     delay(3000);
    //Displays a string, red font
    eink.disString(12,48,1,"DFRobot三色电子墨水屏测试程序",BLACK);
    //Refresh screen display
    eink.flush();
}
