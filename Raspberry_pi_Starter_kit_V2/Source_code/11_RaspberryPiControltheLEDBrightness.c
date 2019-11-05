/*********************************************************************************
*Filename      : lightsensor.c
*Description   :use a photoresistor to allow the LED to breathe
*Company       : SunRobotics Technologies
*Website       : www.sunrobotics.co.in
*E-mail        : support@sunrobotics.co.in(For Any Query)
*******************************************************************************/

#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPiSPI.h>

//pin 1(BCM GPIO 18) is PWM  port
#define  LEDPIN   1
 
#define CHAN_CONFIG_SINGLE  8
#define SPICHANNEL          0
#define ANALOGCHANNEL       0
static int myFd ;

void spiSetup (int spiChannel)
{
    if ((myFd = wiringPiSPISetup (spiChannel, 10000)) < 0)
    {
        fprintf (stderr, "Can't open the SPI bus: %s\n", strerror (errno)) ;
        exit (EXIT_FAILURE) ;
    }
}

int myAnalogRead(int spiChannel,int channelConfig,int analogChannel)
{
    if(analogChannel<0 || analogChannel>7)
        return -1;
    unsigned char buffer[3] = {1}; // start bit
    buffer[1] = (channelConfig+analogChannel) << 4;
    wiringPiSPIDataRW(spiChannel, buffer, 3);
    return ( (buffer[1] & 3 ) << 8 ) + buffer[2]; // get last 10 bits
}

void print_info()
{
    printf("\n");
    printf("|************************************|\n");
    printf("|        MCP3008 read lightsensor    |\n");
    printf("|       -------------------------    |\n");
    printf("|       | ADC |           | Pi  |    |\n");
    printf("|       |-----|-----------|-----|    |\n");
    printf("|       | CS  | connect to| CE0 |    |\n");
    printf("|       | Din | connect to| MOSI|    |\n");
    printf("|       | Dout| connect to| MISO|    |\n");
    printf("|       | CLK | connect to| SCLK|    |\n");
    printf("|       | CH0 | connect to| 3.3V|    |\n");
    printf("|       | CH1 | connect to| GND |    |\n");
    printf("|************************************|\n");
    printf("|          LED connect to GPIO1      |\n");
    printf("|                                    |\n");
    printf("|************************************|\n");
    printf("\n");

} 

int main()
{
	int adc;
	if(wiringPiSetup()<0)
		{
			printf("setup wiringPi failed!\n");
            printf("please check your setup\n");
            exit(1);
		} 
	spiSetup(SPICHANNEL);
	pinMode(LEDPIN,PWM_OUTPUT);	
	print_info();
	
	for(;;)
	{
			adc = myAnalogRead(SPICHANNEL,CHAN_CONFIG_SINGLE,ANALOGCHANNEL);
            printf("ADC = %d\n",adc);
			pwmWrite(LEDPIN,1023-adc);
			delay(1000);
	}
	
}
