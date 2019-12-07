/* Code derived from:
 * https://www.arduino.cc/en/Tutorial/AnalogInput
 * https://www.arduino.cc/en/tutorial/button
*/

const int pot_pin = A0; // potentiometer pin
const int plus_pin = 3; // plus button pin
const int minus_pin = 4; // minus button pin
const int setpoint_pin = 5; // output voltage to control setpoint pin
const int led0_pin = 10; // LED pins
const int led1_pin = 11;
const int led2_pin = 12;
const int led3_pin = 13;

int pot_val = 0; // value from potentiometer
int setpoint = 100; // setpoint value, duty cycle between 0 (0V) to 255 (5V)
int step = 0; // step to modify setpoint, controlled by potentiometer
const int max_step = 20;
const int min_step = 1;

void setup() {
    pinMode(plus_pin, INPUT);
    pinMode(minus_pin, INPUT);
    pinMode(setpoint_pin, OUTPUT);
    pinMode(led0_pin, OUTPUT);
    pinMode(led1_pin, OUTPUT);
    pinMode(led2_pin, OUTPUT);
    pinMode(led3_pin, OUTPUT);
    digitalWrite(led0_pin, HIGH);
}

void loop() {
  pot_val = analogRead(pot_pin); // read the value from the potentiometer
  update_leds();
  step = map(pot_val, 0, 1023, min_step, max_step); // set step value according to potentiometer value
  if ( pressed(plus_pin) ) {
    setpoint += step;
  }
  if ( pressed(minus_pin) ) {
    setpoint -= step;
  }
  if (setpoint > 255) {
    setpoint = 255;
  }
  if (setpoint < 0) {
    setpoint = 0;
  }
  analogWrite(setpoint_pin, setpoint);
}

void update_leds() {
    int range = int(max_step/4);
    digitalWrite(led1_pin, light(range));
    digitalWrite(led2_pin, light(range*2));
    digitalWrite(led3_pin, light(range*3));
}

bool light(int threshold) {
    if (setpoint > threshold){
        return true;
    } else {
        return false;
    }
}

bool pressed(const int button_pin) {

  int button_state = 0;
  // read the state of the pushbutton value:
  button_state = digitalRead(button_pin);
  // check if the pushbutton is pressed. If it is, the button_state is HIGH:
  if (button_state == HIGH) {
    return true;
  } else {
    return false;
  }

}