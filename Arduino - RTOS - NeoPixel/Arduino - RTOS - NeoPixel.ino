// Libraries
#include <LibPrintf.h>
#include <Arduino_FreeRTOS.h>
#include <Adafruit_NeoPixel.h>

// Forward declaration of functions
void stripLedTask(void *parameter);
void circleTask (void *parameter);
void TaskLoop( void *pvParameters );

// GPIOs the NeoPixel LEDs are connected to
uint8_t CIRCLE = 2 ;
uint8_t STRIP = 12;

// The number of LEDs per NeoPixel array
#define CIRCLE_LEDS 12
#define STRIP_LEDS 16

// NeoPixel brightness, 0 (min) to 255 (max) - keep low for low current
#define BRIGHTNESS 20 

// Declare TWO independent objects to control the NeoPixels
Adafruit_NeoPixel circle(CIRCLE_LEDS, CIRCLE, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip(STRIP_LEDS, STRIP, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  Serial.println("Setup started");

  // Now set up two tasks to run independently.
  xTaskCreate(
      stripLedTask,
      "Strip ",    // A name just for humans
      250,            // This stack size can be checked & adjusted by reading the Stack Highwater
      &STRIP,         // no parameters for task
      1,              // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
      NULL);          // handle

  xTaskCreate(
      circleTask, // Code to execute
      "Circle",   // Friendly name
      200,        // Stack size
      &CIRCLE,    // no parameters for task
      1,          // Priority
      NULL );     // handle

  // Ensure all LED arrays are clear when we start
  circle.begin();
  circle.clear();
  circle.setBrightness(BRIGHTNESS);

  strip.begin();
  strip.clear();
  strip.setBrightness(BRIGHTNESS);

  printf("Setup completed\n");
}

// Not used - this code does not run (part of idle task)
void loop() {
  Serial.println("In loop");
  delay(1000);
}

// This is the code that causes the Darth Vader/Knight Rider effect
void stripLedTask(void *parameter) {
    static uint8_t red;
    static uint8_t green;
    static uint8_t blue;
    static uint8_t cycle = 0;
    uint8_t currPin = *((uint8_t*) parameter);
    printf("Strip task running\n");
    checkStack(currPin);

    // Do forever (the equivalent of a loop())
    for (;;) {
        switch (cycle) {
        case 0:
            red   = 255;
            green = 0;
            blue  = 0;
            cycle = 1;
            break;

        case 1:
            red   = 0;
            green = 255;
            blue  = 0;
            cycle = 2;
            break;

        case 2:
            red   = 0;
            green = 0;
            blue  = 255;
            cycle = 0;
            break;

        default:
            break;
        }

        // Forwards
        for (int i = 0; i < STRIP_LEDS; i++) {
            strip.setPixelColor(i, red, green, blue);
            strip.show();
            delay(40);
            // Clear the lit LED after a small delay
            strip.setPixelColor(i, 0, 0, 0);
            strip.show();
        }

        // Backwards
        for (int i = STRIP_LEDS - 1; i >= 0; i--) {
            strip.setPixelColor(i, red, green, blue);
            strip.show();
            delay(40);
            strip.setPixelColor(i, 0, 0, 0);
            strip.show();
        }
    }
}

void circleTask (void *parameter) {
    uint8_t red   = 255;
    uint8_t green = 0;
    uint8_t blue  = 0;
    uint8_t currPin = *((uint8_t*) parameter);
    printf("Circle Task running\n");
    checkStack(currPin);

    for(;;) {
      // Make the colours change as we go round the circle
      for (int i = 0; i < CIRCLE_LEDS; i++) {
          circle.setPixelColor(i, red, green, blue);
          circle.show();
          delay(40);
          blue = blue + 12;
          red  = red - 12;
      }

      // Change colours and go round the circle the other way
      red   = 0;
      green = 255;
      blue  = 0;
      circle.clear();

      for (int i = CIRCLE_LEDS - 1; i >= 0; i--) {
          circle.setPixelColor(i, red, green, blue);
          circle.show();
          delay(50);
      }

      // And again
      green = 0;
      blue  = 255;
      circle.clear();

      for (int i = 0; i < CIRCLE_LEDS; i++) {
          circle.setPixelColor(i, red, green, blue);
          circle.show();
          delay(30);
          green += 8;
      }

      // Clear down the circle LEDs and start again
      circle.clear();
      circle.show();
    }
}

// Check stack space
void checkStack(uint8_t binNo) {
   unsigned long remainingStack = uxTaskGetStackHighWaterMark(NULL);
   printf("Task %d Free stack:%lu\n", binNo, remainingStack);

}

