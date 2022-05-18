# #244 RTOS for Arduino Uno (and more)
A real-time operating system for the humble UNO (and Mega2560 etc)

![Thumbnail (Phone)](https://user-images.githubusercontent.com/20911308/169022328-e084c6d7-bc4f-426a-96e9-ac823039eb91.jpg)  
See video #244  

[![JLCPCB-Purple](https://user-images.githubusercontent.com/20911308/159024530-3e083ca1-fea4-4ba9-97d3-a3af3fb979d2.png)](https://www.jlcpcb.com/cem)  

So can we run an operating system (task scheduler) on an Arduino Uno in a meaningful way? By "meaningful way" I maean more than just flashing a couple of LEDs. Stay tuned, you may be pleasantly surprised.

The video demonstrates a simple, two-LED example, before doing something a bit more "meaningful" (something I'm actually doing in a project right now).

► You can now support me by buying me a coffee!
https://buymeacoffee.com/ralphbacon  

### The Arduino loop()

FreeRTOS can optionally include the standard Arduino loop as part of the RTOS **idle task**. This task only runs when no other task is waiting for processor time, which might mean the loop will never run.

Ideally, include the **void loop()** function, but leave it empty. 

To emulate a loop function of your own, create a separate task to run something called myLoop which looks just like the standard loop but over which you have total control.
```
void myLoop() {
    for (;;) {
        // Put all your code for the loop here - runs repeatedly
    }
}
```
Schedule the loop in the usual way (see the video for more details) in the setup function:
```
xTaskCreate(
      myLoop, // the function to run
      "Loop", // Friendly name
      256,    // Stack size
      NULL,   // no parameters for loop task
      1,      // Priority
      NULL ); // handle
```
Then you have the same setup as an ESP32. I tend to use the loop() function (*myLoop()* here) as a sort of master control.

### Resources

CoolTerm serial monitor (freeware):  
https://freeware.the-meiers.org/  

Arduino RTOS library by Phillip Stevens:  
https://github.com/feilipu/Arduino_FreeRTOS_Library  

Essential reading about Arduino RTOS by Phillip Stevens:  
https://feilipu.me/2015/11/24/arduino_freertos/  

### Running RTOS on an Arduino Nano Every (ATMega4809)  
https://tomalmy.com/freertos-on-arduino-nano-every/  

► List of all my videos
(Special thanks to Michael Kurt Vogel for compiling this)  
http://bit.ly/YouTubeVideoList-RalphBacon

► If you like this video please give it a thumbs up, share it and if you're not already subscribed please consider doing so and joining me on my Arduinite (and other μControllers) journey

My channel, GitHub and blog are here:  
\------------------------------------------------------------------  
• https://www.youtube.com/RalphBacon  
• https://ralphbacon.blog  
• https://github.com/RalphBacon  
• https://buymeacoffee.com/ralphbacon  
\------------------------------------------------------------------



