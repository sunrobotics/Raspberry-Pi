/********************************************************************************************
*Filename      : breathing_led.c
*Description   : make LED breath. 
*Company: SunRobotics Technologies
*Website: www.sunrobotics.co.in
*E-mail: support@sunrobotics.co.in(For Any Query)
********************************************************************************************/

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

//setup GPIO0(BCM_GPIO17) as button pin
#define  ButtonPin   0
//setup GPIO1(BCM_GPIO18) as led pin
#define  LedPin      1

int main(void){
    // When initialize wiring failed, print messageto screen
    if(wiringPiSetup() == -1){
        printf("setup wiringPi failed!");
        exit(1); 
    }
    
    pinMode(LedPin, OUTPUT); 
    pinMode(ButtonPin, INPUT);

    // Pull up to 3.3V,make GPIO1 a stable level
    pullUpDnControl(ButtonPin, PUD_UP);

    printf("\n");
    printf("|**************************************|\n");
    printf("|          Button control LED          |\n");
    printf("|    ------------------------------    |\n");
    printf("|         LED connect to GPIO1         |\n");
    printf("|        Button connect to GPIO0       |\n");
    printf("|                                      |\n");
    printf("|     Press button to turn on LED.     |\n");
    printf("|                                      |\n");
    printf("|                                      |\n");
    printf("|**************************************|\n");
    printf("\n");

    digitalWrite(LedPin, HIGH);
    printf("|****************|\n");
    printf("|   LED off...   |\n");
    printf("|****************|\n");

    for(;;){
        // Indicate that button has pressed down
        if(digitalRead(ButtonPin) == 0){
            // Led on
            digitalWrite(LedPin, LOW);
            printf("|****************|\n");
            printf("|   ...LED on    |\n");
            printf("|****************|\n");
            delay(400);
        }
        else{
            // Led off
            digitalWrite(LedPin, HIGH);
            printf("|****************|\n");
            printf("|   LED off...   |\n");
            printf("|****************|\n");
            delay(400);
        }
    }
    return 0;
}
