// For translation dot-dash series into chars
#include "translate_table.hpp"


// Universal Morse Code values in units
#define DOT_DURATION 1
#define DASH_DURATION 3

#define DOT_DASH_SEPARATION 1
#define LETTER_SEPARATION 3
#define WORD_SEPARATION 7

// unit duration based on frequency
const int unit_duration = 300;

// Error coefficient
// For example if error_coefficeint=0.25 and unit_duration=100ms then 85ms < catch_period < 125ms
const double error_coefficient = 0.25;

// Calculate duration of basic signals
const long dot_duration = DOT_DURATION * unit_duration;
const long dash_duration = DASH_DURATION * unit_duration;

const long dot_dash_separation = DOT_DASH_SEPARATION * unit_duration;
const long letter_separation = LETTER_SEPARATION * unit_duration;
const long word_separation = WORD_SEPARATION * unit_duration;


// Pin for reading value from photoresistor
#define PHOTORESISTOR_PIN A0

// Global variables for readSymbol()
bool previous_state;
long previous_time_stamp;


// Decide what level of voltages means HIGH and which LOW
bool readState() {
  if (analogRead(PHOTORESISTOR_PIN) > 700)
    return true;
  return false;
}


// Dot, dash, separator?
char readSymbol() {

  // // Initial values for measuring duration of pulse; put before function execution
  // previous_state = readState();
  // previous_time_stamp = millis();

  // If no change in state do nothing (wait for change)
  // Should be handled by interrupt... but in that case while is just enough
  int foo = 0;
  while (readState() == previous_state) foo++;

  // If change of state is detected calculate duration and translate it to dot/dash/separator
  bool current_state =  readState();
  if ( current_state != previous_state) {

    // Calculate duration of pulse
    long new_time_stamp = millis();
    long singal_duration = new_time_stamp - previous_time_stamp;
    // For debugging purposes
    //    char _print_buffer[40];
    //    sprintf(_print_buffer, "Signal duration: %d State: %d", singal_duration, current_state);
    //    Serial.println(_print_buffer);

    // Prepare for next reading
    previous_state = current_state;
    previous_time_stamp = new_time_stamp;

    // If HIGH pulse return dot or dash

    // WTF?! logically here should be current_state == true; but it works...
    if (current_state == false ) {
      if ( singal_duration > dot_duration * (1 - error_coefficient) && singal_duration < (dot_duration * (1 + error_coefficient))) return '.';
      if ( singal_duration > dash_duration * (1 - error_coefficient) && singal_duration < (dash_duration * (1 + error_coefficient))) return '-';
      return '!';
    }
    else {
      if (singal_duration > dot_dash_separation * (1 - error_coefficient) && singal_duration < dot_dash_separation * (1 + error_coefficient)) return '_';
      if (singal_duration > letter_separation * (1 - error_coefficient) && singal_duration < letter_separation * (1 + error_coefficient)) return ':';
      if (singal_duration > word_separation * (1 - error_coefficient) && singal_duration < word_separation * (1 + error_coefficient)) return ' ';
      return '?';
    }
  }
}
// Translate series of dots, dashes and separators into alphanumeric characters
String readChar() {

  // Variable for storing series of dots and dashes
  String buffer = "";

  while (char current_symbol = readSymbol()) {

    // For debugging purposes
    //    Serial.println("Incoming symbol:");
    //    Serial.println(current_symbol);
    //
    //    Serial.println("buffer:");
    //    Serial.println(buffer);
    //-----------------------

    //
    switch (current_symbol) {
      case '.':
        buffer += '.';
        break;
      case '-':
        buffer += '-';
        break;
      case ':':
        return String(translate(buffer));
        break;
      case ' ':
        return String(translate(buffer))+" ";
        break;
      case '?':
        return "?";
      case '_':
        break;
      default:
        buffer += '+';
        break;
    }
  }
  return "?";   // Error in read
}

void setup() {
  // For debugging purposes
  Serial.begin(9600);

  // Initial values for measuring duration of pulse
  // Variables used in readSymbol()
  previous_state = readState();
  previous_time_stamp = millis();


  Serial.println("Welocme to Arduino Morse Decoder!");
  Serial.print("Current dot duration is set as: ");
  Serial.print(unit_duration);
  Serial.println(" ms");
  Serial.println("Keep in mind that the first and the last character are most exposed to errors!");
  Serial.println("Decoder ready");
}

void loop() {
  // For testing purposes, should return series of dots, dashes and separators
  //  char serial_buf =  readSymbol();
  //  Serial.println(serial_buf);

  // Master function
  String serial_buf =  readChar();
  Serial.print(serial_buf);
  //     Serial.println(1);

}
