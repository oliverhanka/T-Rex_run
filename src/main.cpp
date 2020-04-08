#include <Arduino.h>
#include <ESP32Servo.h>

// DEV KIT
// #define CHANNEL 34        
// #define THRESHOLD 2970

// #define SERVO_REST_POSITION 90
// #define SERVO_PUSH_POSITION 60
// #define SERVO_GPIO 13

// #define ANIMATION_TICK_INTERVALL (1*1000) // timer is set to 10ms (base is in microseconds)
// #define SERVO_PUSH_CNT  200
// #define SERVO_REST_CNT  200
// #define JUMP_DELAY      100

// #define DEBUG_CNT       500

// UNICORN
#define CHANNEL 36        
#define THRESHOLD 3020

#define SERVO_REST_POSITION 90
#define SERVO_PUSH_POSITION 60
#define SERVO_GPIO 26

#define ANIMATION_TICK_INTERVALL (1*1000) // timer is set to 10ms (base is in microseconds)
#define SERVO_PUSH_CNT  200
#define SERVO_REST_CNT  200
#define JUMP_DELAY      90

#define DEBUG_CNT       500


Servo servo1;
int adcValue;
int servoPushCnt;
int servoRestCnt;
int jumpDelay;

int debugCnt;

/*
 * TIMER SETUP
 */
volatile int eventLoopTimerInterruptCounter;

hw_timer_t * eventLoopTimer = NULL;
portMUX_TYPE eventLoopTimerMux = portMUX_INITIALIZER_UNLOCKED;
 
void IRAM_ATTR onEventLoopTimer() {
  portENTER_CRITICAL_ISR(&eventLoopTimerMux);
  eventLoopTimerInterruptCounter++;         // record that an interrupt was fired
  portEXIT_CRITICAL_ISR(&eventLoopTimerMux);
}

void setup() {
  Serial.begin(115200);
  Serial.println("start.");

  // initialize the servo
  servo1.attach(SERVO_GPIO);
  servo1.write(SERVO_REST_POSITION);

  // setup and start event loop timer
  eventLoopTimer = timerBegin(0, 80, true);
  timerAttachInterrupt(eventLoopTimer, &onEventLoopTimer, true);
  timerAlarmWrite(eventLoopTimer, ANIMATION_TICK_INTERVALL, true);
  timerAlarmEnable(eventLoopTimer);

  // reset all counter variables
  servoPushCnt = -1;
  servoRestCnt = -1;
  jumpDelay = -1;
  debugCnt = DEBUG_CNT;
}



void loop() {

    if (eventLoopTimerInterruptCounter > 0) {                // did we get a timer interrupt?

      portENTER_CRITICAL(&eventLoopTimerMux);                // decrease the timer interrupt counter (synchronized via mutex)
      eventLoopTimerInterruptCounter--;
      portEXIT_CRITICAL(&eventLoopTimerMux);
            
      adcValue = analogRead(CHANNEL);
      if (adcValue < THRESHOLD) {               // check for an obstacle (i.e., there was something other then white on the screen)
        jumpDelay = JUMP_DELAY;                 // start the delay between detecting the obstacle and initiating the jump
        Serial.print(".");
      }

      /*
       * Check the counter values
       */

      if (servoPushCnt > 0) {
        servoPushCnt--;
        if (servoPushCnt == 0) {
          servoPushCnt--;                       // set servo push cnt to -1 to disable the counter
          servoRestCnt = SERVO_REST_CNT;        // start the servo rest counter
          servo1.write(SERVO_REST_POSITION);    // start the servo movement (should reach its final position after the servo rest counter is zero)
        }
      }

      if (servoRestCnt > 0) {
        servoRestCnt--;
        if (servoRestCnt == 0) {
          servoRestCnt--;                       // set servo rest cnt to -1 to disable the counter
          // nothing to do here. hopefully t-rex jumpt over the obstacle...
        }
      }

      if (jumpDelay > 0) {
        jumpDelay--;
        if (jumpDelay == 0) {
          jumpDelay--;                          // set jump delay to -1 to disable the counter
          servoPushCnt = SERVO_PUSH_CNT;   // start the servo push counter
          servo1.write(SERVO_PUSH_POSITION);    // start the servo movement (should reach its final position after the servo push counter is zero)
        }
      }

      if (debugCnt > 0) {
        debugCnt--;
        if (debugCnt == 0) {
          debugCnt = DEBUG_CNT;
          Serial.print("DEBUG - jmpDelayCnt: ");
          Serial.print(jumpDelay);
          Serial.print(" srvPushCnt: ");
          Serial.print(servoPushCnt);
          Serial.print(" srvRestCnt: ");
          Serial.print(servoRestCnt);
          Serial.print(" ### advValue: ");
          Serial.println(adcValue);
        }
      }
    } 
}