#include<FastLED.h>
#define NUM_LEDS 8
#define DATA_PIN 7
#define CLOCK_PIN 5
int pir = 0;
int ldr = 0;
int white_bright = 255;       // default brightness for white (0-255)
int buzz_disable = 1;         // change to 1 to switch off initial buzzer

int ledPin = 13;
int pirPin = 12;

// jingle bell variables
int buzzerPin = 2;
int tempo = 200;
char notes[] = "eeeeeeegcde fffffeeeeddedg";
int duration[] = {1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2};


// todo on clean host
// install CH340G serial chip driver, from wemos
// Tools > Board > Arduino Nano
// Tools > Processor > ATmega328P Old Bootloader (if running a recent version of the IDE)
// watch windows ransomware protection and allow arduino to install libraries if next step fails.
// install FastLED library - sketch > include library > manage libraries

CRGBArray<NUM_LEDS> leds;

void setup() { 
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR, DATA_RATE_MHZ(12)>(leds, NUM_LEDS);
  FastLED.setBrightness(white_bright); // Global Brightness setting max 255 

  pinMode(pirPin, INPUT);         // PIR motion sensor read port 
  pinMode(buzzerPin, OUTPUT);         // buzzer port
  pinMode(ledPin, OUTPUT);        // LED port
  if (buzz_disable == 0) {
     digitalWrite(buzzerPin, HIGH);   // buzzer ON, if not disabled        
  }
  digitalWrite(ledPin, HIGH);     // PIR LED ON
  delay(1000);
  digitalWrite(buzzerPin, LOW);       // buzzer OFF
  digitalWrite(ledPin, LOW);      // PIR LED OFF
}

void loop() {
      FastLED.clear();
      for(int i=0; i<NUM_LEDS; i++){           // iterate through the RGB LEDs
         leds[i].setRGB(0, 0, 0);              // set default colour or off
      }
      pir = digitalRead(pirPin);                   // check PIR
      ldr = analogRead(7);                     // read light sensor (0 = very dark, 1023 = very bright)
                                               //      50-150 is usually a suitable threshold
      if (pir == 1){                           // If motion detected
        digitalWrite(ledPin, HIGH);                // PIR LED ON
        delay(100);                            // Delay so when dark the LED will flash on for 0.1s
      }
      else{
        digitalWrite(ledPin, LOW);                 // PIR LED OFF
      }
      //if (pir == 1 && ldr <= 150 ) {         // if someone there AND dark(ish) no case
      if (pir == 1 && ldr <= 50 ) {            // if someone there AND dark(ish) with case
      //if (pir == 1){                           // if someone there
        digitalWrite(ledPin, LOW);                 // Don't need PIR LED on now
    
        //for(int i=0; i<NUM_LEDS; i++){
     //       leds[i].setRGB(white_bright, white_bright, white_bright);
     //   }
        
      //  FastLED.show();
      //  delay(18000);                          // lights on for about 18 seconds

        playTheTune();
        
    //    for(int j=white_bright; j>-1; j--){ 
    //       for(int i=0; i<NUM_LEDS; i++){
    //          leds[i].setRGB(j, j, j);         // fading out the white over ~2s
    //       }
    //      FastLED.show();
    //      delay(10);                           // wait 10ms
    //    }

        FastLED.clear();                       // reset LEDs to 0,0,0
        FastLED.show();                        // lights off
      }
      else {
        FastLED.show();                        // display current LED settings
        delay(500);                            // otherwise poll PIR sensor at 2 Hertz
      }
}

void setLed0() {  
     leds[0].setRGB(255, 0, 0);
     leds[1].setRGB(0, 255, 0);
     leds[2].setRGB(0, 0, 255);
     leds[3].setRGB(255, 0, 0);
     leds[4].setRGB(0, 255, 0);
     leds[5].setRGB(0, 0, 255);
     leds[6].setRGB(255, 0, 0);
     leds[7].setRGB(0, 255, 0);
        
     FastLED.show();
}

void setLed1() {  
     leds[0].setRGB(0, 255, 0);
     leds[1].setRGB(0, 0, 255);
     leds[2].setRGB(255, 0, 0);
     leds[3].setRGB(0, 255, 0);
     leds[4].setRGB(0, 0, 255);
     leds[5].setRGB(255, 0, 0);
     leds[6].setRGB(0, 255, 0);
     leds[7].setRGB(0, 0, 255);
        
     FastLED.show();
}

// https://giacomocerquone.com/blog/jingle-bell-with-an-arduino-and-a-buzzer

void playTheTune() {
  bool ledStep = true;
  // Scan each char from "notes"
  for (int i = 0; i < sizeof(notes)-1; i++) {
    if (notes[i] == ' ') {
      // If find a space it rests
      delay(duration[i] * tempo);
    } else {
      playANote(notes[i], duration[i] * tempo);
      if (ledStep) {
        ledStep = false;
        setLed0();
      }
      else {
        ledStep = true;
        setLed1();
      }
    }

    // Pauses between notes
    delay((tempo*2)*duration[i]);
  }
}

void playANote(char note, int duration) {
  char notesName[] = { 'c', 'd', 'e', 'f', 'g' };
  int tones[] = { 261, 293, 329, 349, 392 };

  for (int i = 0; i < sizeof(tones); i++) {
    // Bind the note took from the char array to the array notesName
    if (note == notesName[i]) {
      // Bind the notesName to tones
      tone(buzzerPin, tones[i], duration);
    }
  }
}
