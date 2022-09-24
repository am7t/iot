#if !defined(ESP8266)
#error This code is designed to run on ESP8266 and ESP8266-based boards! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "ESP8266TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG 0
#define _TIMERINTERRUPT_LOGLEVEL_ 0

// Select a Timer Clock
#define USING_TIM_DIV1 true     // for shortest and most accurate timer
#define USING_TIM_DIV16 false   // for medium time and medium accurate timer
#define USING_TIM_DIV256 false  // for longest timer but least accurate. Default

#include "ESP8266TimerInterrupt.h"

//#define PIN_D1 5  // Pin D1 mapped to pin GPIO5 of ESP8266

#define SWPin D3

#define TIMER_INTERVAL_MS 20
#define DEBOUNCING_INTERVAL_MS 100
#define LONG_PRESS_INTERVAL_MS 5000

//#define LOCAL_DEBUG               1

// Init ESP8266 timer 1
ESP8266Timer ITimer;

volatile bool SWPressed = false;
volatile bool SWLongPressed = false;

void IRAM_ATTR TimerHandler() {
  static unsigned int debounceCountSWPressed = 0;
  static unsigned int debounceCountSWReleased = 0;

#if (TIMER_INTERRUPT_DEBUG > 0)
  static unsigned long SWPressedTime;
  static unsigned long SWReleasedTime;

  static unsigned long currentMillis;
#endif

  //currentMillis = millis();

  if ((!digitalRead(SWPin))) {
    // Start debouncing counting debounceCountSWPressed and clear debounceCountSWReleased
    debounceCountSWReleased = 0;

    if (++debounceCountSWPressed >= DEBOUNCING_INTERVAL_MS / TIMER_INTERVAL_MS) {
      // Call and flag SWPressed
      if (!SWPressed) {
#if (TIMER_INTERRUPT_DEBUG > 0)
        SWPressedTime = currentMillis;

        Serial.print("SW Press, from millis() = ");
        Serial.println(SWPressedTime - DEBOUNCING_INTERVAL_MS);
#endif

        SWPressed = true;
        // Do something for SWPressed here in ISR
        // But it's better to use outside software timer to do your job instead of inside ISR
        //Your_Response_To_Press();
        digitalWrite(ledPin, !digitalRead(ledPin));
        lightStateChanged = true;
        publishLightState();
      }

      if (debounceCountSWPressed >= LONG_PRESS_INTERVAL_MS / TIMER_INTERVAL_MS) {
        // Call and flag SWLongPressed
        if (!SWLongPressed) {
#if (TIMER_INTERRUPT_DEBUG > 0)
          Serial.print("SW Long Pressed, total time ms = ");
          Serial.print(currentMillis);
          Serial.print(" - ");
          Serial.print(SWPressedTime - DEBOUNCING_INTERVAL_MS);
          Serial.print(" = ");
          Serial.println(currentMillis - SWPressedTime + DEBOUNCING_INTERVAL_MS);
#endif

          SWLongPressed = true;
          // Do something for SWLongPressed here in ISR
          // But it's better to use outside software timer to do your job instead of inside ISR
          //Your_Response_To_Long_Press();
        }
      }
    }
  } else {
    // Start debouncing counting debounceCountSWReleased and clear debounceCountSWPressed
    if (SWPressed && (++debounceCountSWReleased >= DEBOUNCING_INTERVAL_MS / TIMER_INTERVAL_MS)) {
      // Call and flag SWPressed
#if (TIMER_INTERRUPT_DEBUGEBUG > 0)
      SWReleasedTime = millis();

      // Call and flag SWPressed
      Serial.print("SW Released, from millis() = ");
      Serial.println(millis());
#endif

      SWPressed = false;
      SWLongPressed = false;

      // Do something for !SWPressed here in ISR
      // But it's better to use outside software timer to do your job instead of inside ISR
      //Your_Response_To_Release();

      // Call and flag SWPressed
#if (TIMER_INTERRUPT_DEBUG > 0)
      Serial.print("SW Pressed total time ms = ");
      Serial.println(millis() - SWPressedTime);
#endif

      debounceCountSWPressed = 0;
    }
  }
}

void initSwitchBounceSettings() {
  pinMode(SWPin, INPUT_PULLUP);
  Serial.print(F("\nStarting SwitchDebounce on "));
  Serial.println(ARDUINO_BOARD);
  Serial.println(ESP8266_TIMER_INTERRUPT_VERSION);
  Serial.print(F("CPU Frequency = "));
  Serial.print(F_CPU / 1000000);
  Serial.println(F(" MHz"));

  // Interval in microsecs
  if (ITimer.attachInterruptInterval(TIMER_INTERVAL_MS * 1000, TimerHandler)) {
    Serial.print(F("Starting ITimer OK, millis() = "));
    Serial.println(millis());
  } else
    Serial.println(F("Can't set ITimer. Select another freq. or timer"));
}