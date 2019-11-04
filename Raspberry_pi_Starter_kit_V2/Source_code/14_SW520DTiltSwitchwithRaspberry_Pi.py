###########################################################################
#Filename      :sw520d.py
#Description   :If the SW520D tilt,red LED flashing to alarm
#Company       :SunRobotics Technologies
#Website       :www.sunrobotics.co.in
#E-mail        :support@sunrobotics.co.in(For Any Query)
############################################################################

import RPi.GPIO as GPIO
import time

# set #5 as LED pin
LedPin = 6
# set #6 as SW520D Pin
Sw520dPin = 5

#print message at the begining ---custom function
def print_message():
    print ('==================================')
    print ('|            LED Alarm           |')
    print ('|        ----------------        |')
    print ('|        If SW520D Tilt          |')
    print ('|                                |')
    print ('|        LED Will Blink          |')
    print ('|        ----------------        |')
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
    #set LedPin's mode to output,and initial level to HIGH(3.3V)
    GPIO.setup(LedPin,GPIO.OUT,initial=GPIO.HIGH)
    GPIO.setup(Sw520dPin,GPIO.IN)


#main function
def main():
    #print info
    print_message()
    while True:
        #read Sw520dPin's level
        if(GPIO.input(Sw520dPin)):
            GPIO.output(LedPin,GPIO.LOW)
            time.sleep(0.5)
            GPIO.output(LedPin,GPIO.HIGH)
            time.sleep(0.5)
            print ('********************')
            print ('*     tilting!     *')
            print ('********************')
            print ('\n')
        else:
            GPIO.output(LedPin,GPIO.HIGH)
            print ('====================')
            print ('=     Not tilt...  =')
            print ('====================')
            print ('\n')
            time.sleep(1)

#define a destroy function for clean up everything after the script finished
def destroy():
    #turn off LED
    GPIO.output(LedPin,GPIO.HIGH)
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

        
        





    
