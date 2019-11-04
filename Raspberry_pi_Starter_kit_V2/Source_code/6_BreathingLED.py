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

//pin 1(BCM GPIO 18) is PWM  port
#define  LEDPIN   1


int main(){
    int bright;
    if(wiringPiSetup()==-1){
        printf("setup wiringPi failed!\n");
        printf("please check your setup\n");
        exit(1);
    }
    
    pinMode(LEDPIN,PWM_OUTPUT);

    printf("\n");
    printf("\n");
    printf("********************************|\n");
    printf("|         Breath LED            |\n");
    printf("|   ------------------------    |\n");
    printf("|                               |\n");
    printf("|      LED connect to GPIO1     |\n");
    printf("|                               |\n");
    printf("|                               |\n");
    printf("|                               |\n");
    printf("********************************|\n");
    for(;;){
         printf("|*****************|\n");
         printf("|   breath off    |\n"); 
         printf("|*****************|\n");
         for(bright=0;bright<1024;++bright){
            pwmWrite(LEDPIN,bright);
            delay(2);
            }
         printf("|*****************|\n");
         printf("|    breath on    |\n");
         printf("|*****************|\n");
         for(bright=1023;bright>=0;--bright){
            pwmWrite(LEDPIN,bright);
            delay(2);
            }
        }
    return 0;
 }
