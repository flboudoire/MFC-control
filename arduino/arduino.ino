/*
  This code is in the public domain.
  https://github.com/flboudoire/MFC-control/
*/


/* GLOBAL VARIABLES AND FUNCTIONS */


/* buttons, + and -, used to change setpoint values
 setpoint increment defined by a potentiometer*/

const int plus_pin = 12; // plus button pin
bool plus_state = false; // plus button state used in pressed()

const int minus_pin = 11; // minus button pin
bool minus_state = false; // minus button state used in pressed()

bool is_pressed(const int button_pin, bool &button_state) {

  /* check if button goes from LOW to HIGH and update current button state */

  int button_last_state = button_state; // store last state

  // read the state of the pushbutton value:
  button_state = digitalRead(button_pin);

  // check if pushbutton went from LOW to HIGH meaning it is pressed
  return button_state && !button_last_state ? true:false;

}


/* potentiometer used to set the buttons increment */

const int pot_pin = A7; // potentiometer pin
int pot_val = 0; // value read from potentiometer
int increment = 5; // setpoint increment, controlled by potentiometer
const int max_increment = 20; // maximum increment size
const int min_increment = 1; // minimum increment size


/* mass flow controller PWM control */

const int setpoint_pin = 3; // setpoint+ pin
int setpoint = 100; // setpoint value, duty cycle between 0 (0V) to 255 (5V)
const int max_setpoint = 255; // maximum setpoint = 5V
const int min_setpoint = 0; // minimum setpoint = 0V


/* LEDs, used to give feedback on setpoint value
 used to make sure the device functions properly at a glance */

const int n_leds = 4; // number of leds
const int led_pins[n_leds] = {7, 8, 9, 10}; // led pins

void hello_leds() {

  /* loop through all leds to check they function
   (and also for the cool factor) */

  int t_delay = 300; // time between leds lighting

  // light up leds one after the other
  for (int pin = led_pins[0]; pin <= led_pins[n_leds - 1]; pin++) {
    digitalWrite(pin, HIGH);
    delay(t_delay);
  }

  // shut down leds one after the other
  for (int pin = led_pins[n_leds - 1]; pin >= led_pins[0]; pin--) {
    digitalWrite(pin, LOW);
    delay(t_delay);
  }

}

void update_leds() {

  /* loop through all leds, check whether they should be on or off,
   depending on the setpoint value, and update their state */

  // define interval for one led
  int interval = int(max_setpoint/n_leds);

  // loop through all leds
  for (int i = 0; i <= n_leds - 1; i++) {

    // define setpoint threshold where led i should light up
    int threshold = interval*i;

    // update state according to setpoint value 
    bool state = setpoint > threshold ? true:false;
    digitalWrite(led_pins[i], state);

  }

}


/* SETUP AND LOOP */


void setup() {

  // inputs
  pinMode(plus_pin, INPUT);
  pinMode(minus_pin, INPUT);

  // outputs
  pinMode(setpoint_pin, OUTPUT);
  for (int pin = led_pins[0]; pin <= led_pins[n_leds - 1]; pin++) {
    pinMode(pin, OUTPUT);
  }

  // check leds
  hello_leds();
}


void loop() {

  // read the value from the potentiometer, set increment accordingly
  pot_val = analogRead(pot_pin);
  increment = map(pot_val, 0, 1023, min_increment, max_increment);

  // if buttons are pressed, increment setpoint accordingly
  if (is_pressed(plus_pin, plus_state)) {setpoint += increment;}
  if (is_pressed(minus_pin, minus_state)) {setpoint -= increment;}

  // limit setpoint value between its max and min values
  if (setpoint > max_setpoint) { setpoint = max_setpoint;}
  if (setpoint < min_setpoint) { setpoint = min_setpoint;}

  // update leds state according to setpoint
  update_leds();

  // output setpoint to mass flow controller
  analogWrite(setpoint_pin, setpoint);
}
