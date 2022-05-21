/*
    Quick sketch (Arduino-speak C++) to show FreeRTOS tasks running and loop()

    Sketch uses 13244 bytes (41%) of program storage space. Maximum is 32256 bytes.
    Global variables use 563 bytes (27%) of dynamic memory, leaving 1485 bytes for
    local variables. Maximum is 2048 bytes.
*/

#include <Arduino_FreeRTOS.h>
#include <LibPrintf.h>

// The GPIO pins for the LEDs
constexpr uint8_t redLED = 4;
constexpr uint8_t grnLED = 13;

// Two blink tasks (forward declarations)
void TaskBlinkRed( void *pvParameters );
void TaskBlinkGrn( void *pvParameters );

// Handles to the tasks (if we need them - optional)
TaskHandle_t redTask;
TaskHandle_t grnTask;

// SETUP
void setup() {
  Serial.begin(9600);
  printf("Setup started with portTICK_PERIOD_MS:%d\n", portTICK_PERIOD_MS);

  // GPIO pins for LEDs
  pinMode(redLED, OUTPUT);
  pinMode(grnLED, OUTPUT);

  // Now set up two tasks to run independently.
  xTaskCreate(
    TaskBlink,
    "BlinkGrn",   // A name just for humans
    256,  // This stack size can be checked & adjusted by reading the Stack Highwater
    (void *) &grnLED,
    2,  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    &grnTask);

  xTaskCreate(
    TaskBlink,
    "BlinkRed",
    200,  // Stack size
    (void *) &redLED,
    1,  // Priority
    &redTask); // Handle

  xTaskCreate(
    myLoop,
    "Loop",
    256,  // Stack size
    NULL, // no parameters for loop task
    2,  // Priority
    NULL ); // handle

  printf("Setup completed\n");
}

// Common blink task
void TaskBlink(void * parameters) {
  printf("This is task: %s\n", pcTaskGetName(NULL));

  uint8_t ledPin = *((uint8_t *)parameters);
  printf("Using LED pin %d\n", ledPin);

  // Must never end. Variable delay depending on LED colour
  for (;;) {
    digitalWrite(ledPin, !digitalRead(ledPin));
    vTaskDelay((ledPin == 4 ? 200 : 500) / portTICK_PERIOD_MS);
  }
}

// Now used as part of the idle task - don't use! Whenever I tried
// to use the loop it just crashed the whole sketch (heap memory, maybe)
// if I introduced any kind of delay() or vTaskDelay().
// Whatever the cause, I recommend avoid using the loop;
void loop() {
  static unsigned long lastMillis = 0;
  static unsigned long myCount = 0;

  if (millis() - lastMillis > 750) {
    printf("std Loop: %lu\n", myCount++);
    lastMillis = millis();
  }
}

// Replacement loop under my control
void myLoop() {
  static unsigned long myCount = 0;
  for (;;) {
    printf("myLoop: %lu\n", myCount++);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}
