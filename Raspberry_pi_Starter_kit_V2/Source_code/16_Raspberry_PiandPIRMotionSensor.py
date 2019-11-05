#!/usr/bin/python
###########################################################################
#Filename      : pirsensor.c
#Description   : alarm system
#Company: SunRobotics Technologies
#Website: www.sunrobotics.co.in
#E-mail: support@sunrobotics.co.in(For Any Query)
############################################################################
import RPi.GPIO as GPIO
import time

# set BCM_GPIO 17(GPIO 0) as PIR pin
PIRPin = 17
# set BCM_GPIO 18(GPIO 1) as buzzer pin
BuzzerPin = 18

#print message at the begining ---custom function
def print_message():
    print ('==================================')
    print ('|              Alarm             |')
    print ('|     -----------------------    |')
    print ('|     PIR connect to GPIO0       |')
    print ('|                                |')
    print ('|     Buzzer connect to GPIO1    |')
    print ('|     ------------------------   |')
    print ('|                                |')
    print ('|                                |')
    print ('==================================\n')
    print ('Program is running...')
    print ('Please press Ctrl+C to end the program...')

#setup function for some setup---custom function
def setup():
    GPIO.setwarnings(False)
    #set the gpio modes to BCM numbering
    GPIO.setmode(GPIO.BCM)
    #set BuzzerPin's mode to output,and initial level to HIGH(3.3V)
    GPIO.setup(BuzzerPin,GPIO.OUT,initial=GPIO.HIGH)
    GPIO.setup(PIRPin,GPIO.IN)

#main function
def main():
    #print info
    print_message()
    while True:
        #read Sw520dPin's level
        if(GPIO.input(PIRPin)!=0):
            GPIO.output(BuzzerPin,GPIO.LOW)
            #time.sleep(0.5)
            print ('********************')
            print ('*     alarm!     *')
            print ('********************')
            print ('\n')
	    time.sleep(1)
        else:
            GPIO.output(BuzzerPin,GPIO.HIGH)
            print ('====================')
            print ('=     Not alarm...  =')
            print ('====================')
            print ('\n')
	    time.sleep(1)
            

#define a destroy function for clean up everything after the script finished
def destroy():
    #turn off buzzer
    GPIO.output(BuzzerPin,GPIO.HIGH)
    #release resource
    GPIO.cleanup()
#
# if run this script directly ,do:
if __name__ == '__main__':
    setup()
    try:
            main()
    #when 'Ctrl+C' is pressed,child program destroy() will be executed.
    except KeyboardInterrupt:
        destroy()
        pass
