/********************************************************************************************
*Filename      :flow_led.c
*Description   : make a flowing led 
*Company: SunRobotics Technologies
*Website: www.sunrobotics.co.in
*E-mail: support@sunrobotics.co.in(For Any Query) ********************************************************************************************/

#include <wiringPi.h>
#include <stdio.h>

//turn on LED --- custom function
void turn_on(int channel){
    digitalWrite(channel,LOW);
}

//turn off LED --- custom function
void turn_off(int channel){
    digitalWrite(channel,HIGH);
}

//set  led port as output
void setup(){
    int i;
    for(i=0;i<8;i++){
        pinMode(i,OUTPUT);
        digitalWrite(i,HIGH);
    }
}

int main(){
    int i;
    if(wiringPiSetup()==-1){
        printf("setup wiringPi failed!\n");
        printf("please check your setup\n");
        return -1;
    }
    
    setup();
    printf("\n");
    printf("\n");
    printf("********************************|\n");
    printf("|         Flow LED              |\n");
    printf("|   ------------------------    |\n");
    printf("|                               |\n");
    printf("|   LED_R1 connect to GPIO0     |\n");
    printf("|   LED_R2 connect to GPIO1     |\n");
    printf("|   LED_G1 connect to GPIO2     |\n");
    printf("|   LED_G2 connect to GPIO3     |\n");
    printf("|   LED_Y1 connect to GPIO4     |\n");
    printf("|   LED_Y2 connect to GPIO5     |\n");
    printf("|   LED_W1 connect to GPIO6     |\n");
    printf("|   LED_W2 connect to GPIO7     |\n");
    printf("|                               |\n");
    printf("|                               |\n");
    printf("|                               |\n");
    printf("********************************|\n");

    while(1){
        //turn led from left to right
        printf("|****************************|\n");
        printf("|         ----->>            |\n");
        printf("|    From Left To Right!     |\n");
        printf("|                            |\n");
        printf("|****************************|\n");
        for(i=0;i<8;i++){
            turn_on(i);
            delay(150);
            turn_off(i);
        } 
        //turn on from righ to left
        printf("|****************************|\n");
        printf("|         <<-----            |\n");
        printf("|     From Right To Left!    |\n");
        printf("|                            |\n");
        printf("|****************************|\n");
        for(i=7;i>=0;i--){
            turn_on(i);
            delay(150);
            turn_off(i);
        }
    }
    return 0;
}
