/**********************************************************************
*Filename      : buzzer.c
*Description   : make a buzzer beep. 
*Company: SunRobotics Technologies
*Website: www.sunrobotics.co.in
*E-mail: support@sunrobotics.co.in(For Any Query)

**********************************************************************/
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

#define BuzzerPin 1

int main(void){
    int i;
    if(wiringPiSetup() == -1){ //when initialize wiring failed, print messageto screen
        printf("setup wiringPi failed !");
        exit(1); 
    }
    
    pinMode(BuzzerPin,OUTPUT);   //set GPIO1 output with PWM

    printf("\n");
    printf("|**************************************|\n");
    printf("|                 Beep                 |\n");
    printf("|    ------------------------------    |\n");
    printf("|        Buzzer connect to GPIO1       |\n");
    printf("|                                      |\n");
    printf("|            Make Buzzer beep          |\n");
    printf("|                                      |\n");
    printf("|                                      |\n");
    printf("|**************************************|\n");
    printf("\n");
    
    printf("Program is running...\n");
    printf("\n");
    printf("Please press Ctrl+C to end the program...\n");
    for(;;){
        for(i=0;i<70;i++){
            //beep on
            digitalWrite(BuzzerPin, LOW);
            delay(3);
            //beep off
            digitalWrite(BuzzerPin,HIGH);
            delay(3);
        }
        delay(100);
            for(i=0;i<100;i++){
            //beep off
            digitalWrite(BuzzerPin, HIGH);
            delay(1);
            //beep on
            digitalWrite(BuzzerPin,LOW);
            delay(1);
        }
        //delay(100);
    }

    return 0;
}
