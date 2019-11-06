/********************************************************************************************
*Filename      : IRtest.c
*Description   : IR remote control other device
*Company       : SunRobotics Technologies
*Website       : www.sunrobotics.co.in
*E-mail        : support@sunrobotics.co.in(For Any Query)
********************************************************************************************/
#include <wiringPi.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <lirc/lirc_client.h>
#include <time.h>
#include <stdint.h>
#include <wiringPiSPI.h>
#include <wiringPiI2C.h>

void display_on(void);
void display_off(void);
void LightSensor();
void flipBeep(int beep);

#define ON 1
#define OFF 0
 
//The WiringPi pin numbers used by our device
#define DHT11  15
#define BEEP   4

#define MAXTIMINGS  85
#define DHTPIN      15  //DHT connect to TxD
int dht11_dat[5] ={0,0,0,0,0};//store DHT11 data

#define CHAN_CONFIG_SINGLE  8
#define SPICHANNEL          0
#define ANALOGCHANNEL       0
static int myFd ;

int LCDAddr = 0x3F;//IIc LCD address
int BLEN = 1;//1--open backlight.0--close backlight
int fd;//linux file descriptor

void setup()
{
     pinMode (DHT11, INPUT);
     pinMode (BEEP, OUTPUT);
     digitalWrite(BEEP,HIGH);
}

void read_dht11_dat()
{
    uint8_t laststate = HIGH;
    uint8_t counter = 0;
    uint8_t j = 0,i;
    float f;//fahrenheit

    dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

    //pull pin down to send start signal
    pinMode( DHTPIN, OUTPUT );
    digitalWrite( DHTPIN, LOW );
    delay( 18 );
    //pull pin up and wait for sensor response
    digitalWrite( DHTPIN, HIGH );
    delayMicroseconds( 40 );
    //prepare to read the pin
    pinMode( DHTPIN, INPUT );

    //detect change and read data
    for ( i = 0; i < MAXTIMINGS; i++ )
    {
        counter = 0;
        while ( digitalRead( DHTPIN ) == laststate )
        {
            counter++;
            delayMicroseconds( 1 );
            if ( counter == 255 )
            {
                break;
            }
        }
        laststate = digitalRead( DHTPIN );

        if ( counter == 255 )
            break;
        //ignore first 3 transitions
        if ( (i >= 4) && (i % 2 == 0) )
        {
            //shove each bit into the storage bytes
            dht11_dat[j / 8] <<= 1;
            if ( counter > 16 )
                dht11_dat[j / 8] |= 1;
            j++;
        }
    }
    //check we read 40 bits(8bit x 5) +verify checksum in the last byte
    //print it out if data is good
    if ( (j >= 40) &&
         (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) )
    {
        f = dht11_dat[2] * 9. / 5. + 32;
        printf( "Humidity = %d.%d %% Temperature = %d.%d C (%.1f F)\n",
            dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3], f );
    }
    else
    {
        printf( "Data not good, skip\n" );
    }
}

//writ a word(16 bits) to LCD
void write_word(int data){
    int temp = data;
    if ( BLEN == 1 )
        temp |= 0x08;
    else
        temp &= 0xF7;
    wiringPiI2CWrite(fd, temp);
}

//send command to lcd
void send_command(int comm){
    int buf;
    // Send bit7-4 firstly
    buf = comm & 0xF0;
    buf |= 0x04;            // RS = 0, RW = 0, EN = 1
    write_word(buf);
    delay(2);
    buf &= 0xFB;            // Make EN = 0
    write_word(buf);

    // Send bit3-0 secondly
    buf = (comm & 0x0F) << 4;
    buf |= 0x04;            // RS = 0, RW = 0, EN = 1
    write_word(buf);
    delay(2);
    buf &= 0xFB;            // Make EN = 0
    write_word(buf);
}

//send data to lcd
void send_data(int data){
    int buf;
    // Send bit7-4 firstly
    buf = data & 0xF0;
    buf |= 0x05;            // RS = 1, RW = 0, EN = 1
    write_word(buf);
    delay(2);
    buf &= 0xFB;            // Make EN = 0
    write_word(buf);

    // Send bit3-0 secondly
    buf = (data & 0x0F) << 4;
    buf |= 0x05;            // RS = 1, RW = 0, EN = 1
    write_word(buf);
    delay(2);
    buf &= 0xFB;            // Make EN = 0
    write_word(buf);
}

//initialize the lcd
void init(){
    send_command(0x33); // Must initialize to 8-line mode at first
    delay(5);
    send_command(0x32); // Then initialize to 4-line mode
    delay(5);
    send_command(0x28); // 2 Lines & 5*7 dots
    delay(5);
    send_command(0x0C); // Enable display without cursor
    delay(5);
    send_command(0x01); // Clear Screen
    wiringPiI2CWrite(fd, 0x08);
}

//clear screen
void clear(){
    send_command(0x01); //clear Screen
}

//Print the message on the lcd
void i2c_write(int x, int y, char data[]){
    int addr, i;
    int tmp;
    if (x < 0)  x = 0;
    if (x > 15) x = 15;
    if (y < 0)  y = 0;
    if (y > 1)  y = 1;

    // Move cursor
    addr = 0x80 + 0x40 * y + x;
    send_command(addr);
    
    tmp = strlen(data);
    for (i = 0; i < tmp; i++){
        send_data(data[i]);
    }
}

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

 
int main(int argc, char *argv[])
{
        struct lirc_config *config;
         //Timer for our buttons
        int buttonTimer = millis();
        char *code;
 
        //Initiate WiringPi and set WiringPi pins 4, 5 & 6 (GPIO 23, 24 & 25) to output. These are the pins the LEDs are connected to.
        if (wiringPiSetup () == -1)
        {
        	fprintf(stderr,"Can't init wiringPi: %s\n",strerror(errno));
        	exit(EXIT_FAILURE);
        }
        setup();
	fd = wiringPiI2CSetup(LCDAddr);
	init(); //init 1602
  clear(); //clear screen
        spiSetup(SPICHANNEL);//select SPI0

        //Initiate LIRC. Exit on failure
        if(lirc_init("lirc",1)==-1)
                exit(EXIT_FAILURE);
 
        //Read the default LIRC config at /etc/lirc/lircd.conf  This is the config for your remote.
        if(lirc_readconfig(NULL,&config,NULL)==0)
        {
                //Do stuff while LIRC socket is open  0=open  -1=closed.
                while(lirc_nextcode(&code)==0)
                {
                        //If code = NULL, meaning nothing was returned from LIRC socket,
                        //then skip lines below and start while loop again.
                        if(code==NULL) continue;{
                                //Make sure there is a 400ms gap before detecting button presses.
                                if (millis() - buttonTimer  > 400){
                                        //Check to see if the string "KEY_1" appears anywhere within the string 'code'.
                                        if(strstr (code,"KEY_1")){
                                                printf("open 1602\n");
                                                display_on();
                                                buttonTimer = millis();
                                        }
                                        else if(strstr (code,"KEY_2")){
                                                printf("close 1602\n");
                                                display_off();
                                                buttonTimer = millis();
                                        }
                                        else if(strstr (code,"KEY_3")){
                                                printf("get light value\n");
                                                LightSensor();
                                                buttonTimer = millis();
                                        }
                                        else if(strstr (code,"KEY_4")){
                                                printf("control beep\n");
                                                flipBeep(BEEP);
                                                buttonTimer = millis();
                                        }
                                        else if(strstr (code,"KEY_5")){
                                                printf("get temperature and humidity\n");
                                                read_dht11_dat();
                                                buttonTimer = millis();
                                        }
                                }
                        }
                        //Need to free up code before the next loop
                        free(code);
                }
                //Frees the data structures associated with config.
                lirc_freeconfig(config);
        }
	close(myFd);
        //lirc_deinit() closes the connection to lircd and does some internal clean-up stuff.
        lirc_deinit();
        exit(EXIT_SUCCESS);
}

//open/close beep
void flipBeep(int beep)
{
		 //If beep is on, turn it off. Otherwise it is off, so thefore we need to turn it on.
        if(digitalRead(beep)==ON)
                digitalWrite(beep, OFF);
        else
                digitalWrite(beep, ON);
}

// display info on 1602
void display_on()
{
	i2c_write(0,0,"This is Lesson20");
  i2c_write(0,1,"IIC LCD Test");	
}
//close 1602
void display_off()
{
	clear();	
}
//read light sensor
void LightSensor()
{
	int adc;
	adc = myAnalogRead(SPICHANNEL,CHAN_CONFIG_SINGLE,ANALOGCHANNEL);	
	printf("ADC = %d\n",adc);
}
