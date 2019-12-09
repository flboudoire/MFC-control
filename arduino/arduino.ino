/* Code derived from:
 * https://www.arduino.cc/en/Tutorial/AnalogInput
 * https://www.arduino.cc/en/tutorial/button
*/

const int pot_pin = A7; // potentiometer pin
const int plus_pin = 12; // plus button pin
bool plus_state = false;
const int minus_pin = 11; // minus button pin
bool minus_state = false;
const int setpoint_pin = 3; // output voltage to control setpoint pin

// LED pins, 4 LEDs are used to give feedback on step value
const int led0_pin = 7;
const int led1_pin = 8;
const int led2_pin = 9;
const int led3_pin = 10;

int pot_val = 0; // value from potentiometer
int setpoint = 100; // setpoint value, duty cycle between 0 (0V) to 255 (5V)
const int max_setpoint = 255; // maximum setpoint
const int min_setpoint = 0; // minimum setpoint
int step = 5; // step to modify setpoint, controlled by potentiometer
const int max_step = 20; // maximum step size
const int min_step = 1; // minimum step size

void setup() {
    pinMode(plus_pin, INPUT);
    pinMode(minus_pin, INPUT);
    pinMode(setpoint_pin, OUTPUT);
    pinMode(led0_pin, OUTPUT);
    pinMode(led1_pin, OUTPUT);
    pinMode(led2_pin, OUTPUT);
    pinMode(led3_pin, OUTPUT);
    hello();
    digitalWrite(led0_pin, HIGH);
    Serial.begin(9600);
}

void loop() {
  pot_val = analogRead(pot_pin); // read the value from the potentiometer
  update_leds();
  step = map(pot_val, 0, 1023, min_step, max_step); // set step value according to potentiometer value
  if ( pressed(plus_pin, plus_state) ) {
    setpoint += step;
  }
  if ( pressed(minus_pin, minus_state) ) {
    setpoint -= step;
  }
  if (setpoint > max_setpoint) {
    setpoint = max_setpoint;
  }
  if (setpoint < min_setpoint) {
    setpoint = min_setpoint;
  }
  analogWrite(setpoint_pin, setpoint);
  Serial.println(setpoint);
}

void update_leds() {
    int range = int(max_setpoint/4);
    digitalWrite(led1_pin, light(range));
    digitalWrite(led2_pin, light(range*2));
    digitalWrite(led3_pin, light(range*3));
}

void hello() {
  int t_delay = 300;
  for (int pin = led0_pin; pin <= led3_pin; pin++) {
    digitalWrite(pin, HIGH);
    delay(t_delay);
  }
  for (int pin = led3_pin; pin >= led0_pin; pin--) {
    digitalWrite(pin, LOW);
    delay(t_delay);
  }
}

bool light(int threshold) {
    if (setpoint > threshold){
        return true;
    } else {
        return false;
    }
}

bool pressed(const int button_pin, bool &button_last_state) {
  // read the state of the pushbutton value:
  int button_state = digitalRead(button_pin);
  button_last_state = button_state;
  // check if the pushbutton is pressed and released. If it is, the button_state is HIGH:
  if (button_state && !button_last_state)
  {
    return true;
  }
  return false;
}