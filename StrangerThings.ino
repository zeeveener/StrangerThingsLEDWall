#include <StandardCplusplus.h>
#include <vector>

using namespace std;

#include "FastLED.h"

#define NUM_LEDS        50
#define DATA_PIN        3
#define LED_TYPE        WS2811
#define COLOR_ORDER     RGB

#define FPS             30
#define MAX_BRIGHTNESS  255
#define FADE            10

#define A 46 
#define B 45 
#define C 43
#define D 42 
#define E 40 
#define F 39
#define G 38 
#define H 36
#define I 22
#define J 23 
#define K 25 
#define L 26
#define M 27 
#define N 29 
#define O 30
#define P 31 
#define Q 32 
#define R 18
#define S 16 
#define T 15 
#define U 14
#define V 12 
#define W 11 
#define X 10
#define Y 9 
#define Z 7

// Define the array of leds
CRGB leds[NUM_LEDS];
struct FadeStatus {
  CHSV color;
  int index;
  int brightness;
  int fade;
  bool fading;
};
struct FadeStatus fades[NUM_LEDS];

vector<vector<int>> words = {
//  {A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z}
    {H,E,L,L,O},  
    {G,O,O,D,N,E,S,S}, 
    {R,U,N}, 
    {B,E,N,E,V,I,T,Y},
    {H,I,M,O,M},
    {T,E,A,M,E,L,E,V,E,N},
    {M,I,N,I,G,O,L,F}
  };


CHSV COLORS[5] = { CHSV(0, 255, 255),   // Red
                   CHSV(160, 255, 255), // Blue
                   CHSV(96, 255, 255),  // Green
                   CHSV(32, 255, 255),  // Orange
                   CHSV(64, 255, 255)   // Yellow
                 };  
int curWordIndex = 0;
int curLetterIndex = 0;
vector<int> curWord = words.at(0);
unsigned long startNextWordAt = 0;


void setup() {
  delay(1000);
  
  Serial.begin(9600);

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(MAX_BRIGHTNESS);

  for (int i = 0; i < NUM_LEDS; i++) {
    fades[i].color = COLORS[i % 5];
    fades[i].index = i;
    fades[i].brightness = 0;
    fades[i].fade = FADE;
    fades[i].fading = false;
  }

  srand(micros());
}

void startFade(int ledIndex) {
  fades[ledIndex].fading = true;
  fades[ledIndex].fade = FADE;
  fades[ledIndex].brightness = 0;
}

void stopFade(int ledIndex) {
  fades[ledIndex].fading = false;
  fades[ledIndex].fade = FADE;
  fades[ledIndex].brightness = 0;
}

void continueFade(int ledIndex) {
  struct FadeStatus* fadeStatus = &fades[ledIndex];

  if (!fadeStatus->fading) {
    return;
  }

  CRGB* led = &leds[ledIndex];
  CHSV color = fadeStatus->color;

  if ((fadeStatus->brightness + fadeStatus->fade) >= MAX_BRIGHTNESS) {
    fadeStatus->fade *= -1;
  } else if ((fadeStatus->brightness + fadeStatus->fade) <= 0) {
    stopFade(ledIndex);
    led->setHSV(color.hue, color.sat, fadeStatus->brightness);
    return;
  }
  
  fadeStatus->brightness += fadeStatus->fade;
  led->setHSV(color.hue, color.sat, fadeStatus->brightness);
}

void loop() {
  if(curLetterIndex < curWord.size()){
    if(millis() >= startNextWordAt){
      if(!fades[curWord.at(curLetterIndex)].fading){
        startFade(curWord.at(curLetterIndex));
      }else{
        continueFade(curWord.at(curLetterIndex));
        if(!fades[curWord.at(curLetterIndex)].fading){
          curLetterIndex++;
        }
      }
    }else{
      // Wait
    }
  }else{
    curWordIndex++;
    curWordIndex %= words.size();

    curWord = words.at(curWordIndex);
    curLetterIndex = 0;

    startNextWordAt = millis() + 2500;
  }

  FastLED.show();
  delay(1000 / FPS);
}
