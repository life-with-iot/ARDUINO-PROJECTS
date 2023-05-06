#Python 3.5.3 (default, Sep 27 2018, 17:25:39) 
#[GCC 6.3.0 20170516] on linux
#Type "copyright", "credits" or "license()" for more information.
import time
import RPi.GPIO as GPIO       ## Import GPIO library
GPIO.setmode(GPIO.BOARD)      ## Use board pin numbering
GPIO.setup(11, GPIO.OUT)      ## Setup GPIO Pin 11 to OUT
while True:
	GPIO.output(11,True)  ## Turn on Led
	time.sleep(1)         ## Wait for one second
	GPIO.output(11,False) ## Turn off Led
	time.sleep(1)         ## Wait for one second
