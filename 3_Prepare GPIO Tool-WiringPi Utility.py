/*****************************************************************
*Filename      : testwiringpi.c
*Description   : this program controls led flashing with wiringPi
*Company       : SunRobotics Technologies
*Website       : www.sunrobotics.co.in
*E-mail        : support@sunrobotics.co.in(For Any Query)
******************************************************************/
#include <wiringPi.h>
#include <stdio.h>

#define LEDPIN  0

int main(){
    //when initialize wiring failed,print message to screen
    if(wiringPiSetup()== -1){
        printf("Setup wiringPi Failed!");
        return -1;
    }

    pinMode(LEDPIN,OUTPUT);
    printf("\n");
    printf("\n");
    printf("********************************|\n");
    printf("|       LED Blink               |\n");
    printf("|  -----------------------      |\n");
    printf("|                               |\n");
    printf("|   LED connect to GPIO 0       |\n");
    printf("|                               |\n");
    printf("|   LED will blink at 500ms     |\n");
    printf("|                               |\n");
    printf("********************************|\n");

    while(1){
        digitalWrite(LEDPIN,LOW);
        printf("...LED OFF\n");
        printf("\n");
        delay(500);
        digitalWrite(LEDPIN,HIGH);
        printf("LED ON...\n");
        printf("\n");
        delay(500);
    }
    return 0;
}
