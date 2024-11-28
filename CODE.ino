#include <Adafruit_NeoPixel.h>

// Multiplexer control pins for LEDs
const int S0 = 2;  // Multiplexer select pin 1
const int S1 = 3;  // Multiplexer select pin 2
const int S2 = 4;  // Multiplexer select pin 3
const int S3 = 5;  // Multiplexer select pin 4
const int COMMON_PIN = 7;  // Common input pin to control the selected LED ring

// Multiplexer control pins for columns (input multiplexer connected to analog pin)
const int S0_column = 8;  // Multiplexer column select pin 1
const int S1_column = 9;  // Multiplexer column select pin 2
const int S2_column = 10; // Multiplexer column select pin 3
const int S3_column = 11; // Multiplexer column select pin 4

// Multiplexer control pins for rows (output multiplexer connected to HIGH signal)
const int S0_row = 12;    // Multiplexer row select pin 1
const int S1_row = 13;    // Multiplexer row select pin 2
const int S2_row = 14;    // Multiplexer row select pin 3
const int S3_row = 15;    // Multiplexer row select pin 4
const int ROW_COMMON_PIN = 6;  // Common pin to output HIGH to active row

// Pressure sensor and button pins
const int PRESSURE_SENSOR_PIN = A0;  // Analog pin for column multiplexer (reading resistance)
const int BUTTON_PIN = 16;           // Button to change difficulty

// Adafruit NeoPixel setup
#define LED_PIN 7    // LED data pin
#define NUM_LEDS 28  // Each ring has 28 LEDs
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Difficulty levels
const int Easy1[] = {3, 6, 11, 14};
const int Easy2[] = {3, 2, 4, 1, 7, 6, 8, 5, 11, 10, 12, 9, 15, 14, 16, 13};
const int Intermediate1[] = {7, 3, 4, 2, 8, 6, 15, 11, 12, 10, 16, 14};
const int Advanced[] = {8, 5, 3, 2, 7, 6, 4, 1, 6, 7, 16, 13, 11, 10, 15, 14, 12, 9, 15, 14};

// Array of difficulties
const int* difficulties[] = {Easy1, Easy2, Intermediate1, Advanced};
const int difficultiesLengths[] = {4, 16, 12, 20};  // Lengths of each difficulty
int current_difficulty_index = 0;  // Start with Easy1
const int* current_difficulty = difficulties[current_difficulty_index];
int current_difficulty_length = difficultiesLengths[current_difficulty_index];

// Threshold for detecting pressure (adjust based on testing)
const int PRESSURE_THRESHOLD = 512;

// Function to set multiplexer channel using control pins
void setMultiplexerChannel(int channel, int S0_pin, int S1_pin, int S2_pin, int S3_pin) {
    digitalWrite(S0_pin, (channel >> 0) & 1);
    digitalWrite(S1_pin, (channel >> 1) & 1);
    digitalWrite(S2_pin, (channel >> 2) & 1);
    digitalWrite(S3_pin, (channel >> 3) & 1);
}

// Function to light up an LED in a specific color
void Light_LED(int ring, uint8_t red, uint8_t green, uint8_t blue) {
    setMultiplexerChannel(ring - 1, S0, S1, S2, S3);  // Select the LED ring (1-based index)
    digitalWrite(COMMON_PIN, HIGH);                  // Enable the common pin for LEDs
    for (int i = 0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i, red, green, blue);     // Set color
    }
    strip.show();                                    // Update LED state
}

// Function to turn off all LEDs in the current ring
void turnOffAllLEDs() {
    for (int i = 0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i, 0, 0, 0);  // Turn off LEDs
    }
    strip.show();
}

// Function to detect pressure input based on the active LED ring
bool Pressure_input_detected(int ring) {
    // Activate the corresponding row and column for the current LED
    switch (ring) {
        case 13: setMultiplexerChannel(1, S0_column, S1_column, S2_column, S3_column);
                 setMultiplexerChannel(8, S0_row, S1_row, S2_row, S3_row);
                 break;
        case 9:  setMultiplexerChannel(1, S0_column, S1_column, S2_column, S3_column);
                 setMultiplexerChannel(7, S0_row, S1_row, S2_row, S3_row);
                 break;
        case 5:  setMultiplexerChannel(1, S0_column, S1_column, S2_column, S3_column);
                 setMultiplexerChannel(6, S0_row, S1_row, S2_row, S3_row);
                 break;
        case 1:  setMultiplexerChannel(1, S0_column, S1_column, S2_column, S3_column);
                 setMultiplexerChannel(5, S0_row, S1_row, S2_row, S3_row);
                 break;
        case 14: setMultiplexerChannel(2, S0_column, S1_column, S2_column, S3_column);
                 setMultiplexerChannel(8, S0_row, S1_row, S2_row, S3_row);
                 break;
        case 10: setMultiplexerChannel(2, S0_column, S1_column, S2_column, S3_column);
                 setMultiplexerChannel(7, S0_row, S1_row, S2_row, S3_row);
                 break;
        case 6:  setMultiplexerChannel(2, S0_column, S1_column, S2_column, S3_column);
                 setMultiplexerChannel(6, S0_row, S1_row, S2_row, S3_row);
                 break;
        case 2:  setMultiplexerChannel(2, S0_column, S1_column, S2_column, S3_column);
                 setMultiplexerChannel(5, S0_row, S1_row, S2_row, S3_row);
                 break;
        case 15: setMultiplexerChannel(3, S0_column, S1_column, S2_column, S3_column);
                 setMultiplexerChannel(8, S0_row, S1_row, S2_row, S3_row);
                 break;
        case 11: setMultiplexerChannel(3, S0_column, S1_column, S2_column, S3_column);
                 setMultiplexerChannel(7, S0_row, S1_row, S2_row, S3_row);
                 break;
        case 7:  setMultiplexerChannel(3, S0_column, S1_column, S2_column, S3_column);
                 setMultiplexerChannel(6, S0_row, S1_row, S2_row, S3_row);
                 break;
        case 3:  setMultiplexerChannel(3, S0_column, S1_column, S2_column, S3_column);
                 setMultiplexerChannel(5, S0_row, S1_row, S2_row, S3_row);
                 break;
        case 16: setMultiplexerChannel(4, S0_column, S1_column, S2_column, S3_column);
                 setMultiplexerChannel(8, S0_row, S1_row, S2_row, S3_row);
                 break;
        case 12: setMultiplexerChannel(4, S0_column, S1_column, S2_column, S3_column);
                 setMultiplexerChannel(7, S0_row, S1_row, S2_row, S3_row);
                 break;
        case 8:  setMultiplexerChannel(4, S0_column, S1_column, S2_column, S3_column);
                 setMultiplexerChannel(6, S0_row, S1_row, S2_row, S3_row);
                 break;
        case 4:  setMultiplexerChannel(4, S0_column, S1_column, S2_column, S3_column);
                 setMultiplexerChannel(5, S0_row, S1_row, S2_row, S3_row);
                 break;
        default: return false;
    }

    digitalWrite(ROW_COMMON_PIN, HIGH);  // Activate the row
    delay(10);                           // Stabilize multiplexer
    int pressureValue = analogRead(PRESSURE_SENSOR_PIN);
    digitalWrite(ROW_COMMON_PIN, LOW);   // Deactivate the row after reading
    return pressureValue > PRESSURE_THRESHOLD;  // Return true if pressure detected
}

// Function to handle the light-up order
void light_up_order(const int* difficulty, int length) {
    for (int i = 0; i < length; i++) {
        int ring = difficulty[i];
        Light_LED(ring, 255, 255, 255);  // Light up in white
        while (!Pressure_input_detected(ring)) {
            if (digitalRead(BUTTON_PIN) == LOW) {
                turnOffAllLEDs();
                return;
            }
            delay(100);  // Poll every 100ms
        }
        Light_LED(ring, 0, 255, 0);  // Change to green when pressure detected
        delay(500);  // Hold green for 0.5 seconds
        turnOffAllLEDs();
    }
}

// Setup function
void setup() {
    // Initialize multiplexer pins
    pinMode(S0, OUTPUT); pinMode(S1, OUTPUT); pinMode(S2, OUTPUT); pinMode(S3, OUTPUT);
    pinMode(S0_column, OUTPUT); pinMode(S1_column, OUTPUT); pinMode(S2_column, OUTPUT); pinMode(S3_column, OUTPUT);
    pinMode(S0_row, OUTPUT); pinMode(S1_row, OUTPUT); pinMode(S2_row, OUTPUT); pinMode(S3_row, OUTPUT);
    pinMode(COMMON_PIN, OUTPUT);
    pinMode(ROW_COMMON_PIN, OUTPUT);
    pinMode(PRESSURE_SENSOR_PIN, INPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // Initialize NeoPixel
    strip.begin();
    strip.show();
}

// Main loop
void loop() {
    if (digitalRead(BUTTON_PIN) == LOW) {
        current_difficulty_index = (current_difficulty_index + 1) % 4;
        current_difficulty = difficulties[current_difficulty_index];
        current_difficulty_length = difficultiesLengths[current_difficulty_index];
        delay(500);  // Debounce
    }

    light_up_order(current_difficulty, current_difficulty_length);
}
