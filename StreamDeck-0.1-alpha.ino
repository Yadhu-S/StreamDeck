#define DEBOUNCE 10  // how many ms to debounce, 5+ ms is usually plenty
 
//define the buttons that we'll use.
byte buttons[] = {2, 3, 4, 5, 12}; 
 
//determine how big the array up above is, by checking the size
#define NUMBUTTONS sizeof(buttons)
 
//track if a button is just pressed, just released, or 'currently pressed' 
byte pressed[NUMBUTTONS], justpressed[NUMBUTTONS], justreleased[NUMBUTTONS];
byte previous_keystate[NUMBUTTONS], current_keystate[NUMBUTTONS];

#include <SoftwareSerial.h>
SoftwareSerial Bluetooth(10, 9); // RX, TX

void setup() {
  byte i;
  Bluetooth.begin(9600);
  Serial.begin(9600); //set up serial port
  Serial.print("Button checker with ");
  Serial.print(NUMBUTTONS, DEC);
  Serial.println(" buttons");
  // Make input & enable pull-up resistors on switch pins
  for (i=0; i< NUMBUTTONS; i++) {
    pinMode(buttons[i], INPUT);
    //digitalWrite(buttons[i], HIGH);
  }
}
 
void loop() {
  
  int x=(analogRead(A0));
  int y = (analogRead(A1));
  byte thisSwitch=thisSwitch_justPressed();
  if (x > 520 || x<500 || y > 520 || y<500){
    Serial.println(x);
    Serial.println(y);
    Bluetooth.print("_1|x:");
    Bluetooth.print(x);
    Bluetooth.print("|y:");
    Bluetooth.println(y);
  }
  switch(thisSwitch)
  {  
  case 0: 
    Serial.println("switch 1 just pressed");
    Bluetooth.println("1");
    break;
  case 1: 
    Serial.println("switch 2 just pressed");
    Bluetooth.println("2");
    break;
  case 2: 
    Serial.println("switch 3 just pressed");
    Bluetooth.println("3");
    break;
  case 3: 
    Serial.println("switch 4 just pressed");
    Bluetooth.println("4");
    break;
  case 4: 
    Serial.println("switch 5 just pressed");
    Bluetooth.println("5");
    break;
  case 5: 
    Serial.println("switch 6 just pressed");
    Bluetooth.println("Z");
    break;   
  }
}
 
void check_switches()
{
  static byte previousstate[NUMBUTTONS];
  static byte currentstate[NUMBUTTONS];
  static long lasttime;
  byte index;
  if (millis() < lasttime) {
    // we wrapped around, lets just try again
    lasttime = millis();
  }
  if ((lasttime + DEBOUNCE) > millis()) {
    // not enough time has passed to debounce
    return; 
  }
  // ok we have waited DEBOUNCE milliseconds, lets reset the timer
  lasttime = millis();
  for (index = 0; index < NUMBUTTONS; index++) {
    justpressed[index] = 0;       //when we start, we clear out the "just" indicators
    justreleased[index] = 0;
    currentstate[index] = digitalRead(buttons[index]);   //read the button
    if (currentstate[index] == previousstate[index]) {
      if ((pressed[index] == LOW) && (currentstate[index] == LOW)) {
        // just pressed
        justpressed[index] = 1;
      }
      else if ((pressed[index] == HIGH) && (currentstate[index] == HIGH)) {
        justreleased[index] = 1; // just released
      }
      pressed[index] = !currentstate[index];  //remember, digital HIGH means NOT pressed
    }
    previousstate[index] = currentstate[index]; //keep a running tally of the buttons
  }
}
 
byte thisSwitch_justPressed() {
  byte thisSwitch = 255;
  check_switches();  //check the switches &amp; get the current state
  for (byte i = 0; i < NUMBUTTONS; i++) {
    current_keystate[i]=justpressed[i];
    if (current_keystate[i] != previous_keystate[i]) {
      if (current_keystate[i]) thisSwitch=i;
    }
    previous_keystate[i]=current_keystate[i];
  }  
  return thisSwitch;
}
