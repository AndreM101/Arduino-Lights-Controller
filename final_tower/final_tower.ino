

  

#include <FastLED.h>

#define START_LED_PIN     30
#define NUM_LEDS    300
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_STRIPS 3
#define UPDATES_PER_SECOND 50
#define SENSOR_PIN 50
#define MAX_PATTERN_LENGTH 10
#define SENSOR_DELAY 5000 //milliseconds
#define IDLE_COLOUR_RISE 1
//#define aSize 13
//#define bSize 11
//#define cSize 10
#define IDLE_BLOCK_NUMBER 5
#define PATTERN_H 10
#define IDLE_WAIT 3000
#define PATTERN_DIFFERENT_COLOURS 3


CRGB Green  = CHSV( HUE_GREEN, 255, 255);
CRGB stripArray[NUM_STRIPS][NUM_LEDS];

int tower[3][15][2] = {
  {
    {226, 239},   //changed for
    {210, 225},   //testing purposes
    {170, 209},
    {162, 169},
    {150, 161},
    {116, 140},
    {141, 149},
    {95, 108},
    {109, 115},
    {50, 72},
    {73, 94},
    {40, 49},
    {8, 39},
    {0, 7},
    {-1, -1}
    
  },
  
  {
    {247, 258},
    {239, 246},
    {224, 230},
    {213, 223},
    {202, 212},
    {184, 201},
    {121, 166},
    {167, 183},
    {98, 120},
    {32, 69},
    {70, 97},
    {0, 18},
    {19, 31},
    {-1, -1},
    {-1, -1}
  },
  
  {
    {224, 247},
    {213, 223},
    {174, 212},
    {128, 147},
    {148, 173},
    {105, 127},
    {39, 67},
    {68, 104},
    {0, 23},
    {24, 38},
    {-1, -1},
    {-1, -1},
    {-1, -1},
    {-1, -1}
  }
  
};
//GOD FUCKING DAMNIT. (YOU KNOW WHAT.)
double connections[3][15][8] = {  //A=0, B=1, C=2
     {
         { 11.0, 0, 13.1, 0, 0, 0, 13.0, 0 },
         { 0, 11.0, 14.0, 0, 0, 0, 0, 12.0 },
         { 0, 10.0, 13.0, 0, 0, 0, 8.2, 0 },
         { 8.0, 7.1, 10.1, 13.1, 0, 13.0, 10.0, 0 },
         { 9.0, 8.0, 11.0, 0, 13.0, 12.0, 0, 6.2 },   //10
         { 6.0, 0, 8.0, 0, 10.0, 8.2, 6.2, 0 },
         { 7.0, 0, 7.1, 11.0, 0, 9, 0, 6.0 },
         { 5.0, 5.1, 0, 7.1, 8.0, 0, 6.0, 0 },
         { 4.0, 5.0, 7.0, 8.0, 9.0, 0, 6.2, 4.2 },    //6
         { 0, 3.1, 5.1, 7.1, 7.0, 6.0, 4.0, 3.0 },
         { 0, 3.0, 5.0, 0, 6.0, 6.2, 4.2, 2.2 },
         { 2.0, 2.1, 3.1, 5.1, 5.0, 4.0, 2.2, 0 },                  //3
         { 0, 0, 1.1, 2.1, 2.0, 2.2, 1.2, 0 }  
     },
     {
         { 10.1, 0, 12.1, 0, 0, 0, 14.0, 11.0 },
         { 0, 11.1, 0, 0, 0, 0, 13.1, 10.1 },
         { 9.1, 7.2, 10.2, 0, 0, 12.1, 0, 10.1 },     //11
         { 7.1, 9.1, 0, 11.1, 0, 13.1, 11.0, 8.0 },
         { 8.1, 5.2, 7.2, 0, 11.1, 10.1, 0, 7.1 },
         { 6.1, 3.2, 0, 0, 8.1, 0, 7.1, 0 },
         { 5.1, 6.1, 8.1, 9.1, 10.1, 8.0, 7.0, 5.0 },                  //7
         { 4.1, 3.2, 0, 5.2, 8.1, 7.1, 0, 5.1 },
         { 3.1, 4.1, 0, 6.1, 7.1, 7.0, 0, 3.0 },
         { 0, 1.2, 0, 3.2, 6.1, 5.1, 0, 0 },          //4
         { 0, 2.1, 0, 4.1, 5.1, 3.0, 2.0, 0 },
         { 1.1, 0, 1.2, 3.2, 0, 3.1, 2.0, 1.0 },
         { 0, 0, 0, 1.2, 0, 2.1, 2.0, 1.0 }
     },
     {
         { 7.2, 0, 0, 9.2, 0, 0, 11.1, 0 },
         { 0, 8.1, 0, 0, 0, 0, 10.2, 0 },
         { 6.2, 10.0, 12.0, 0, 0, 9.2, 0, 7.2 },
         { 5.2, 0, 6.2, 8.2, 10.2, 11.1, 9.1, 0 },              //7
         { 4.1, 6.0, 9.0, 10.0, 8.2, 0, 7.2, 5.2 },
         { 3.2, 4.2, 0, 6.2, 7.2, 9.1, 8.1, 6.1 },
         { 0, 0, 4.0, 9.0, 6.2, 5.2, 0, 3.2 },                //4
         { 1.2, 2.2, 4.0, 0, 4.2, 5.2, 6.1, 4.1 },
         { 0, 1.0, 2.0, 3.0, 0, 3.2, 0, 1.2 },
         { 0, 0, 1.0, 2.0, 3.2, 4.1, 2.1, 1.1 }
     }

  };

TBlendType currentBlending; 





//////////////////////
//
//
//     GLOBALS
//
//
////////////////////
int possibleConnections[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
unsigned long timeSinceTrip;
bool idle;
bool idleActive;
CHSV currentPrimaryColour;
bool maximum = false;
int idleCounter = 0;
double idleBlocks[IDLE_BLOCK_NUMBER];
CHSV idleColours[IDLE_BLOCK_NUMBER];
int secondIdleCount;
double patternActiveCells[MAX_PATTERN_LENGTH];
unsigned long timeSinceIdle = millis();
CRGB patternRCArray[PATTERN_DIFFERENT_COLOURS];
//////////////////////
//
//    MAIN FUNCS
//
///////////////////////

void setup() {
  
  timeSinceTrip = millis();
  
  Serial.begin(9600);
  //LED setup
  
  delay( 3000 );
  
  FastLED.addLeds<LED_TYPE, START_LED_PIN, COLOR_ORDER>(stripArray[0], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds < LED_TYPE, START_LED_PIN + 1, COLOR_ORDER > (stripArray[1], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds < LED_TYPE, START_LED_PIN + 2, COLOR_ORDER > (stripArray[2], NUM_LEDS).setCorrection( TypicalLEDStrip );

  FastLED.setBrightness(  BRIGHTNESS );
  currentBlending = LINEARBLEND;

  //Sensor setup

  pinMode(SENSOR_PIN, INPUT);
  pinMode(SENSOR_PIN + 1, INPUT);
  pinMode(SENSOR_PIN + 2, INPUT);
  idle = true;
}

int triggered;

void loop() {
  
    Serial.print("T: "); Serial.print(triggered); Serial.print("\n");
    triggered = sensorDetection(timeSinceTrip);
    if (triggered > 30) {
      timeSinceTrip = millis();
      idle = false;
      idleActive = false;
      patternMaster(triggered);
    } 
        
    else if (triggered == 0){
      idle = true;
      Serial.print("triggered = 0\n");
    }
    
    if (idle && millis() - timeSinceTrip > 10000){
      if(!idleActive){
        setIdle();
        idleActive = true;
        Serial.print("setIdle()\n");
        for (int i = 0; i < IDLE_BLOCK_NUMBER; i++) {
          idleBlocks[i] = randomBlock();
          idleColours[i] = pickIdleColour();
        } 
      }
      
      if (idleCounter == IDLE_BLOCK_NUMBER) {
        Serial.print("FIRST\n");
        secondIdleCount++;
        if (maximum == false) {
          for (int i = 0; i < IDLE_BLOCK_NUMBER; i++){
             idleColours[i] = idleIncrement(idleBlocks[i], idleColours[i]);
             if (idleColours[i].s == 255) {maximum = true; }
          }
        }
      }
      
      else if (idleCounter == IDLE_BLOCK_NUMBER && maximum == true) {
        Serial.print("SECOND\n");
        secondIdleCount++;
           for (int i = IDLE_BLOCK_NUMBER-1; i >=0 ; i--){
             idleColours[i] = idleDecrement(idleBlocks[i], idleColours[i]);
             if (idleColours[i].s == 25) {maximum = false;}
             
          }
      }
      
      else {
        Serial.print("THIRD\n");
        timeSinceIdle = millis();
          idleCounter++;
          secondIdleCount++;
          for (int i = 0; i < idleCounter; i++){
          
           idleColours[i] = idleIncrement(idleBlocks[i], idleColours[i]);
          }

      }
      
      
      if (secondIdleCount == IDLE_BLOCK_NUMBER) {
        Serial.print("FORTH\n");
        secondIdleCount = 0;
        idleActive = false;
      }
      Serial.print("Idle and activated\n");
    }
    Serial.print("*********************TST: "); Serial.print(timeSinceTrip); Serial.print('\n');
    if (triggered == -1){
      delay(500);
      patternContinue();
    }

  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}


//////////////////////////////
//                          //
//   COLOUR FUNCTIONS       //
//                          //
//////////////////////////////



void fillRange(int ledStrip, int ledStart, int ledEnd, CRGB colour) {
  for (int i = ledStart; i <= ledEnd; i++) {
    stripArray[ledStrip][i] = colour;
  }
}

void changeth(int ledStrip, int led, CRGB colour) {
  stripArray[ledStrip][led] = colour;
}



void setBlockColour(double B, CRGB colourToSet) {
  double block = B - 1;
  int strip = extractStrip(block);
  int startLED = tower[strip][(int) block][0];
  int endLED = tower[strip][(int) block][1];
  fillRange(strip, startLED, endLED, colourToSet);
}






            //////////////////////////////////

            //        PATTERN FUNCTIONS

            //////////////////////////////////
void shuffle(int x[]) {
  for (size_t i = 0; i < 8; i++) {
  size_t j = random(i, 8);

  // swap x[i] and x[j]
  auto t = x[i];
  x[i] = x[j];
  x[j] = t;
}
}

CHSV pickIdleColour(){
  int r = random(0, 256);
  CHSV col = CHSV(r, 10, 255);
  return col;
}

double pickAConnection(double b, double prevBlock) {
  double block = b-1;
  int strip = extractStrip(block);

  int randomConnection = 0;
  int sixteen = 16;
  double check = 0;
  int counter = 0;
  shuffle(possibleConnections);
  while (check == 0) {
    counter++;
    if (counter > 16) {
      block = randomBlock();
      strip = extractStrip(block);
      Serial.print("Inf. Loop, randomising block");
    }
    check = connections[strip][(int) block][possibleConnections[randomConnection % 7]];
randomConnection++;

  }
  return check; 
}


void patternMaster(int triggerLocation){
  for (int i = 0; i < MAX_PATTERN_LENGTH; i++) {
    patternActiveCells[i] = 0;
  }
  
  CHSV chosenColour = pickAColour();
//  double start = (double) floor(random(0,3));
  
  setPatternBackground(CHSV((chosenColour.h+100) % 255, chosenColour.s, chosenColour.v));
  double start = 1 + ((triggerLocation % SENSOR_PIN)+1) / 10;
      //A = SENSOR_PIN
      //B = +1
      //C = +2
      patternActiveCells[0] = start;
  patternFunction(chosenColour, start, 0.0, 0);
}




void patternFunction(CHSV pattern, double currentBlock, double previousBlock, int patternLength) {
  //random colour from pattern
  int newLength = patternLength;
  newLength++;
  CHSV chosen = pickASimilarColour(pattern);
    
  setBlockColour(currentBlock, chosen);
 double next = pickAConnection(currentBlock, previousBlock); //maybe more param's for "leaning" connections?
  if ( floor(next) == 0 || newLength > MAX_PATTERN_LENGTH) { //if it's too high, physically, on the tower or if the pattern is too long
    return;
  }
  Serial.print("next block: ");Serial.print(next);Serial.print("\n");
  patternFunction(pattern, next, currentBlock, newLength);
  
}





CHSV pickASimilarColour(CHSV colour) {
  long r = random(colour.h-15, colour.h+16);
  return CHSV((int) r, colour.s, colour.v);
}


CHSV pickAColour() {
  long r = random(35, 220);
  return CHSV((int) r, 255, 255);
}

int sensorDetection(unsigned long previous) {
  int sensor;
  if ((millis() - previous) > SENSOR_DELAY) {
    for (int i = 0; i < 3; i++) {
      sensor = 0;
      sensor = digitalRead(SENSOR_PIN+i);
      Serial.print("######################    Sensor result: ");Serial.print(sensor);Serial.print("  on trigger:  ");Serial.print(i+SENSOR_PIN);Serial.print("\n");
      return random(50,53);
    }
    return 0;
  }
  return -1;
}





void setIdleGood(int i, int l, int a)
{
  for (int i = i; i < l; i++) {
        double block = i + a/10;
        setBlockColour(block, pickIdleColour());
      }
    }

void setactualidle(){
  setIdleGood(1,14,0);
  setIdleGood(1,12,1);
  setIdleGood(1,11,2);
}


void setIdle() {
  for (double a = 0; a < 3; a++) {
      for (int i = 1; i <14; i++) {
        double block = i + a/10;
        setBlockColour(block, pickIdleColour());
      }
    }
  
}


void setPatternBackground(CHSV colour) {
  for (double a = 0; a < 3; a++) {
      for (int i = 1; i < 14; i++) {
        double block = i + a/10;
        if (tower[(int) a][i][0] == 0 && tower[(int) a][i][1] == 0) {break; }
        setBlockColour(block, colour);
      }
    
  }  
}




CHSV idleDecrement(double cell, CHSV oldColour) {
  CHSV newColour;
  if (oldColour.s < 25) { newColour = CHSV(oldColour.h, 25, oldColour.v); }
  else { newColour = CHSV(oldColour.h, oldColour.s-25, oldColour.v); }

  setBlockColour(cell, newColour);
  return newColour;
}

CHSV idleIncrement(double cell, CHSV oldColour) {
  CHSV newColour;
  if (oldColour.s > 255) { newColour = CHSV(oldColour.h, 255, oldColour.v); }
  else { newColour = CHSV(oldColour.h, oldColour.s+25, oldColour.v); }

  setBlockColour(cell, newColour);
  return newColour;
}

void patternIncrement(double cell, CHSV oldColour) {
  CHSV newColour;
  newColour = CHSV(oldColour.h+PATTERN_H, oldColour.s, oldColour.v);
  setBlockColour(cell, newColour);
}

void patternDecrement(double cell, CHSV oldColour) {
  CHSV newColour;
  newColour = CHSV(oldColour.h-PATTERN_H, oldColour.s, oldColour.v);
  setBlockColour(cell, newColour);
  }


double randomBlock() {
  bool done = false;
  double block;
  int b;
  double a;
  while (!done) {
    b = random(1, 14);
    a = random(0, 3);
    if (tower[(int) a][b][0] != 0 || tower[(int) a][b][1] != 0) {
      block = b + a/10;
      done = true;
    }
  }
  Serial.print("-----------RB RELEASING: "); Serial.print(block); Serial.print("\n");
  return block;
}



void patternContinue() {
  for (double a = 0; a < NUM_STRIPS; a++) {
      for (int i = 1; i <14; i++) {
        double block = i + a/10;
        int x = random(0, 2);
        int L = tower[(int) a][i][0];
        if (x == 1) {
            setBlockColour(block, colourDif(stripArray[(int) a][L], patternRCArray[i+1 % PATTERN_DIFFERENT_COLOURS]));
        }
        else {
            setBlockColour(block, colourDif(stripArray[(int) a][L], patternRCArray[i % PATTERN_DIFFERENT_COLOURS]));
        }
      }
  }  
}

CRGB colourDif(CRGB c1, CRGB c2) {
  CRGB X = CRGB(c2.r-c1.r,  c2.g-c1.g,  c2.b-c1.b);
  return CRGB(c1.r + X.r/2, c1.g + X.g/2, c1.b + X.b/2);
}


void setRandomColourArray(){
  for (int i = 0; i < PATTERN_DIFFERENT_COLOURS; i++){
  int R = random(100, 256);
  int G = random(100, 256);
  int B = random(100, 256);
  CRGB NC = CRGB(R,G,B);
  patternRCArray[i] = NC;
  }        
}




int extractStrip(double block){
  int B = (int) block;    //10.2 -> 10
  double c = block - B;     //10.2 - 10 = 0.2
  c = c*10 + 0.5; //2.4999
  return (int) c;
}
