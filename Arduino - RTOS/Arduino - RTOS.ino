#include <LibPrintf.h>

#define configUSE_IDLE_HOOK 0
#include <Arduino_FreeRTOS.h>

constexpr uint8_t redLED = 4;
constexpr uint8_t grnLED = 5;

// Two blink tas
void TaskBlinkRed( void *pvParameters );
void TaskBlinkGrn( void *pvParameters );

// Handles to the tasks (if we need them - optional)
TaskHandle_t redTask;
TaskHandle_t grnTask;

void setup() {
  Serial.begin(115200);
  Serial.println("Setup started");

  pinMode(redLED, OUTPUT);
  pinMode(grnLED, OUTPUT);

    // Now set up two tasks to run independently.
  xTaskCreate(
      TaskBlink,
      "BlinkGrn",   // A name just for humans
      256,  // This stack size can be checked & adjusted by reading the Stack Highwater
      (void *) &grnLED,
      1,  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
      &redTask);

  xTaskCreate(
      TaskBlink,
      "BlinkRed",
      256,  // Stack size
      (void *) &redLED,
      1,  // Priority
      &grnTask); // Handle

  xTaskCreate(
      myLoop,
      "Loop",
      256,  // Stack size
      NULL, // no parameters for loop task
      1,  // Priority
      NULL ); // handle

  printf("Setup completed\n");
}

void TaskBlink(void * parameters) {
  printf("This is task: %s\n", pcTaskGetName(NULL));

  uint8_t ledPin = *((uint8_t *)parameters);
  printf("Using LED pin %d\n", ledPin);

  for (;;) {
    //printf("Blink %d\n", ledPin);
    digitalWrite(ledPin, !digitalRead(ledPin));
    delay(ledPin == 4 ? 200 : 500);
  }

}

// Not used as part of the idle task
void loop() {
}

// Replacement loop under my control
void myLoop() {

  static unsigned long myCount = 0;
  //for (;;) {
    Serial.println(myCount++);
    delay(1000);
  //}
}

