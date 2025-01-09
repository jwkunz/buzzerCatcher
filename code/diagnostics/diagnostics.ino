#include "pin_map.h"

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_READY, OUTPUT);
  pinMode(PIN_TRIGGER_1, OUTPUT);
  pinMode(PIN_TRIGGER_2, OUTPUT);
  pinMode(PIN_TRIGGER_3, OUTPUT);
  pinMode(PIN_TRIGGER_4, OUTPUT);
  pinMode(PIN_BUTTON, OUTPUT);
  pinMode(PIN_SWITCH, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_IO_0, OUTPUT);
  pinMode(PIN_IO_1, OUTPUT);
  pinMode(PIN_IO_2, OUTPUT);
  pinMode(PIN_IO_3, OUTPUT);
  pinMode(PIN_LED_0, OUTPUT);
  pinMode(PIN_LED_1, OUTPUT);
  pinMode(PIN_LED_2, OUTPUT);
  pinMode(PIN_LED_3, OUTPUT);
  pinMode(PIN_LED_4, OUTPUT);
  pinMode(PIN_LED_5, OUTPUT);
  pinMode(PIN_LED_6, OUTPUT);
  pinMode(PIN_LED_7, OUTPUT);

  Serial.begin(250000);
}

void buzzer() {
  Serial.println("Buzzer Diagnostic:\nListen for a 1 KHz tone\nPress 0 to cancel.\n");
  bool continue_flag = true;
  bool buzzer_state = false;
  const uint16_t frequency_hz = 1000;
  const uint16_t delay_us = 1000000 / frequency_hz;
  while (continue_flag) {
    uint8_t byte = Serial.read();
    switch (byte) {
      case '0':
        {
          return;
          break;
        }
      default:
        {
          break;
        }
    }
    digitalWrite(PIN_BUZZER, buzzer_state);
    buzzer_state = !buzzer_state;
    delayMicroseconds(delay_us);
  }
}

void leds() {
  Serial.println("LED Diagnostic:\nLEDs should all be cycling with 1 second each\nPress 0 to cancel.\n");
  bool continue_flag = true;
  const uint16_t delay_ms = 1000;
  int pin_number = 0;
  const int n_pins = 8;
  const int led_pins[n_pins] = { PIN_LED_0, PIN_LED_1, PIN_LED_2, PIN_LED_3, PIN_LED_4, PIN_LED_5, PIN_LED_6, PIN_LED_7 };
  while (continue_flag) {
    uint8_t byte = Serial.read();
    switch (byte) {
      case '0':
        {
          return;
          break;
        }
      default:
        {
          break;
        }
    }
    digitalWrite(led_pins[pin_number], true);
    delay(delay_ms);
    digitalWrite(led_pins[pin_number], false);
    pin_number = (pin_number + 1) % n_pins;
  }
}

void main_buttons_switch() {
  Serial.println("Main button and swtich diagnostic:\nPress 0 to cancel.\n");
  bool continue_flag = true;
  const uint16_t delay_ms = 1000;
  while (continue_flag) {
    uint8_t byte = Serial.read();
    switch (byte) {
      case '0':
        {
          return;
          break;
        }
      default:
        {
          break;
        }
    }
    bool switch_state = digitalRead(PIN_SWITCH);
    bool button_state = digitalRead(PIN_BUTTON);
    char print_str[32];
    sprintf(print_str, "Button: %u, Switch: %u \n", button_state, switch_state);
    Serial.println(print_str);
    delay(delay_ms);
  }
}

void trigger_test() {
  Serial.println("Trigger Test\nPress 0 to cancel.\n");
  bool continue_flag = true;
  uint8_t trigger_state = 0;
  while (continue_flag) {
    uint8_t byte = Serial.read();
    switch (byte) {
      case '0':
        {
          return;
          break;
        }
      default:
        {
          break;
        }
    }

    switch (trigger_state) {
      case 0:
        {
          digitalWrite(PIN_READY, HIGH);
          trigger_state += 1;
          Serial.println("Placed the ready HIGH.  Waiting for press from user 1");
          break;
        }
      case 1:
        {
          if (digitalRead(PIN_TRIGGER_1) == HIGH) {
            Serial.println("Got Trigger 1 and now waiting for Trigger 2");
            trigger_state += 1;
          }
          break;
        }
      case 2:
        {
          if (digitalRead(PIN_TRIGGER_2) == HIGH) {
            Serial.println("Got Trigger 2 and now waiting for Trigger 3");
            trigger_state += 1;
          }
          break;
        }
      case 3:
        {
          if (digitalRead(PIN_TRIGGER_3) == HIGH) {
            Serial.println("Got Trigger 3 and now waiting for Trigger 4");
            trigger_state += 1;
          }
          break;
        }
      case 4:
        {
          if (digitalRead(PIN_TRIGGER_4) == HIGH) {
            Serial.println("Got Trigger 4");
            trigger_state += 1;
          }
          break;
        }
      default:
        {
          digitalWrite(PIN_READY, LOW);
          continue_flag = false;
          break;
        }
    }
  }
}

void loop() {
  Serial.println("\n\nDiagnostic mode\nEnter number for a command\n0-Cancel/Quit Operation\n1-Buzzer\n2-LEDs\n3-Main Button and Switch\n4-Trigger Test\n");
  bool continue_flag = true;
  while (continue_flag) {
    uint8_t byte = Serial.read();
    switch (byte) {
      case '0':
        {
          continue_flag = false;
          break;
        }
      case '1':
        {
          buzzer();
          continue_flag = false;
          break;
        }
      case '2':
        {
          leds();
          continue_flag = false;
          break;
        }
      case '3':
        {
          main_buttons_switch();
          continue_flag = false;
          break;
        }
      case '4':
        {
          trigger_test();
          continue_flag = false;
          break;
        }
      default:
        {
          break;
        }
    }
  }
}
