 /*This example illustrates two user funcions of LedP10 library
  *First function-showmsg_single_scroll is for scroling of single message on P10 panel with 4 arguments which are message, no. of times of message scrolling, speed and font.
  *for message, pointer to a character string can be passed. For continuously running the message, INF can be passed in second argument, spped can be in range of 0-30, 
   and font can be 0 or 1 as two type of fonts are currently supported for full screen messages. 
   
  * For second function-showmsg_double_scroll, there would be 7 arguments, 2 for messages, two for no. of times of message scrolling, two for speed, and last one for font. 
  * Specification being same as for single scrolling function, except only ont font id provided for double scrolling and it's value should be 0.
  * For connection diagram visit https://goo.gl/HiCJC1
  */
 
#include <TimerOne.h>
#include"SPI.h"
#include <ledP10.h>

LedP10 myled;

void setup() 
{
    myled.init(3,4,8,9 ,1);
//    delay(6000);
//    myled.showmsg_single_scroll("this is single led test",2,3,0);
//        myled.showmsg_single_scroll("BIT IOT LAB",INF,2,0);
//    delay(6000);
    myled.showmsg_double_scroll("this is double led test1","this is double led test2",10,INF,3,1,0); 
}

void loop() {
  
}
