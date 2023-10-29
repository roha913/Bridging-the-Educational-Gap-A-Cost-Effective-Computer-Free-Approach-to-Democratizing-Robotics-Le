float threshold_avg = 0.0;


//#include "IRLremote.h"
//#include <IRremote.h>
#include <IRremoteTank.h> 
#include <EEPROM.h>
#define m1dir 13
#define m1pwm 11
#define m2dir 12 
#define m2pwm 3
#define leftLight A1
#define rightLight A2

IRrecv irrecv(A0); 
decode_results results; 

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
String commands = "";
void setup() {
  Serial.begin(9600);
  pinMode(m1dir,OUTPUT);
  pinMode(m1pwm,OUTPUT);
  pinMode(m2dir,OUTPUT);
  pinMode(m2pwm,OUTPUT);
  irrecv.enableIRIn(); // Start the receiver

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
   display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  delay(2000);
  display.clearDisplay();
  delay(2000);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.display();

  pinMode(leftLight, INPUT);
  pinMode(rightLight, INPUT);
  int strlen;
  if(EEPROM.read(1) == 255){
    strlen = 0;
  }else{
    strlen = EEPROM.read(0);
  }
  for(int jj = 1; jj < strlen + 1; jj++){
    display.print(EEPROM.read(jj));
    commands += EEPROM.read(jj);
  }
  display.display();
}

int velo = 127;
float T = 0.75*(0.84*1000);//time to turn 90 degrees
float Q = 625;//time to move step
void Ztop(){
  analogWrite(m1pwm, 0);
  analogWrite(m2pwm, 0);
}
void forward(int blockations){
  digitalWrite(m1dir,HIGH);
  digitalWrite(m2dir,HIGH);
  for(int block = 0; block < blockations; block++){
    analogWrite(m1pwm, velo);
    analogWrite(m2pwm, velo);
    delay(Q);
    analogWrite(m1pwm, 0);
    analogWrite(m2pwm, 0);
    delay(500);
  }
}
void backward(int blockations){
  digitalWrite(m1dir,LOW);
  digitalWrite(m2dir,LOW);
  for(int block = 0; block < blockations; block++){
    analogWrite(m1pwm, velo);
    analogWrite(m2pwm, velo);
    delay(Q);
    analogWrite(m1pwm, 0);
    analogWrite(m2pwm, 0);
    delay(500);
  }
}
void turn_left(int timeations){
  digitalWrite(m1dir,LOW);
  digitalWrite(m2dir,HIGH);
  for(int block = 0; block < timeations; block++){
    analogWrite(m1pwm, velo);
    analogWrite(m2pwm, velo);
    delay(T);
    analogWrite(m1pwm, 0);
    analogWrite(m2pwm, 0);
    delay(500);
  }
}
void turn_right(int timeations){
  digitalWrite(m1dir,HIGH);
  digitalWrite(m2dir,LOW);
  for(int block = 0; block < timeations; block++){
    analogWrite(m1pwm, velo);
    analogWrite(m2pwm, velo);
    delay(T);
    analogWrite(m1pwm, 0);
    analogWrite(m2pwm, 0);
    delay(500);
  }
}

void forwards_n_times(String things, int i){// i is position in things
  String num = ""; //used for numbers for for-loops
  if(!isDigit(things[i + 1]) || i == things.length() - 1){//if at end of commands or next character is not a number
                    forward(1);
                  }else{//if next character is a number
                    int j = i + 1;
                    while(j < things.length() && isDigit(things[j])){//while character is a number
                      num += things[j];
                      j++;
                    }
                    forward(num.toInt());//goes forwards that many times
                    i = j;//moving i to next command
                  }
                  
  
  
}
void backwards_n_times(String things, int i){
  String num = ""; //used for numbers for for-loops
  if(!isDigit(things[i + 1]) || i == things.length() - 1){
                    backward(1);
                  }else{
                    
                    int j = i + 1;
                    while(j < things.length() && isDigit(things[j])){
                      num += things[j];
                      j++;
                    }
                    //Serial.println("num:" + num);
                    backward(num.toInt());
                    i = j;
                  }
  
}
void left_n_times(String things, int i){
  String num = ""; //used for numbers for for-loops
   if(!isDigit(things[i + 1]) || i == things.length() - 1){
                    turn_left(1);
                  }else{
          
                   int j = i + 1;
                    while(j < things.length() && isDigit(things[j])){
                      num += things[j];
                      j++;
                    }
                    //Serial.println("num:" + num);
                    turn_left(num.toInt());
                    i = j;
                  }
  
}
void right_n_times(String things, int i){
  
   if(!isDigit(things[i + 1]) || i == things.length() - 1){
                    turn_right(1);
                  }else{
                    String num = ""; //used for numbers for for-loops
                    int j = i + 1;
                    while(j < things.length() && isDigit(things[j])){
                      num += things[j];
                      j++;
                    }
                    turn_right(num.toInt());
                    i = j;
                  }
  
}
void cont_forward(){
  digitalWrite(m1dir,LOW);
  digitalWrite(m2dir,HIGH);
  analogWrite(m1pwm, velo);
  analogWrite(m2pwm, velo);
  //delay(Q*blockations);
  //Serial.println("FORWARDS");
  //Ztop();
}
void cont_turn_left(){
  digitalWrite(m1dir,HIGH);
  digitalWrite(m2dir,HIGH);
  analogWrite(m1pwm, velo);
  analogWrite(m2pwm, velo);
  //Serial.println("LEFT TURN");

}
void cont_turn_right(){
  digitalWrite(m1dir,LOW);
  digitalWrite(m2dir,LOW);
  analogWrite(m1pwm, velo);
  analogWrite(m2pwm, velo);
  //Serial.println("RIGHT TURN");
}

boolean if_statement_flag = true;
int threshold;
void run_program(String things){
  threshold = (int)(threshold_avg/2);
  Serial.println(Q);
  Serial.println(things);
  for(int i = 0; i < things.length(); i++){//looping through commands/things
              char characterAt = things.charAt(i);
              Serial.println(characterAt);
              switch(characterAt){
                case 'F':
                  if(if_statement_flag){
                    forwards_n_times(things,i);
                  }else{
                    if_statement_flag = true;
                  }
                  break;
                case 'B':
                  if(if_statement_flag){
                    backwards_n_times(things,i);
                  }else{
                    if_statement_flag = true;
                  }
                  break;
                case 'L':
                 if(if_statement_flag){
                    left_n_times(things,i);
                  }else{
                    if_statement_flag = true;
                  }
                  break;
                case 'R':
                  if(if_statement_flag){
                    right_n_times(things,i);
                  }else{
                    if_statement_flag = true;
                  }
                  break;
                case 'I'://I0->F3
                  Serial.println(threshold);
                  if(i < things.length()){
                    if(things.charAt(i + 1) == '0' && !(analogRead(leftLight) < threshold && analogRead(rightLight) < threshold)){
                       i += 4;
                       if_statement_flag = false;
                    }
                    if(things.charAt(i + 1) == '1' && !(analogRead(leftLight) > threshold && analogRead(rightLight) > threshold)){
                       i += 4;
                       if_statement_flag = false;
                    }
                    if(things.charAt(i + 1) == '0' && !(analogRead(leftLight) < threshold && analogRead(rightLight) < threshold)){
                       i += 3;
                    }
                    if(things.charAt(i + 1) == '1' && (analogRead(leftLight) > threshold && analogRead(rightLight) > threshold)){
                       i += 3;
                    }
                  }
                                
              }
              delay(250);
      }
            Serial.println(things);
}
//String commands = "";//stores programs
int num_of_presses = 0;
byte r;
boolean I_pressed = false;
boolean bang_bang_mode = false;
void loop() {
    if(!bang_bang_mode){
      if(irrecv.decode(&results)){
            r = results.value;
            irrecv.resume();
            Serial.println(r); 
            num_of_presses ++;
            threshold_avg = ((num_of_presses - 1)*threshold_avg + 0.5*(analogRead(leftLight) + analogRead(rightLight)))/(num_of_presses);
              switch(r){
                if(!(r == 189)){ //'I' must be pressed twice in a row to enter bang-bang mode. This is if the one after the first 'I' is not
                                 // 'I'
                  I_pressed = false;
                }
                case 157://forwards
                  commands += "F";
                 // isLetter = true;
                 display.print("F");
                display.display();
                  Serial.println("forwards");  
                  break;
                case 87://backwards
                  commands += "B";
                  //isLetter = true;
                  display.print("B");
                display.display();
                  Serial.println("backwards");
                  break;
                case 221://turn left
                  commands += "L";
                  //isLetter = true;
                  display.print("L");
                display.display();
                  Serial.println("left");
                  break;
                case 61://turn right
                  commands += "R";
                  //isLetter = true;
                  display.print("R");
                display.display();
                  Serial.println("right");
                  break;
                case 173://delete a character
                  if(commands.charAt(commands.length() - 1) == '>'){
                    commands = commands.substring(0,commands.length() - 3);//I0->
                  }else{
                    commands = commands.substring(0,commands.length() - 1); 
                  }
                 // isLetter = !isDigit(commands[commands.length() - 1]);
                  display.clearDisplay();
                  display.setCursor(0, 10);
                  display.print(commands);
                  display.display();
                  Serial.println("deleted");
                  break;
                case 189://I, *
                  if(!I_pressed){
                    commands += "I";
                    display.print("I");
                    display.display();
                    Serial.println("I");
                    I_pressed = true;
                  }else{
                    Serial.println("Bang Bang Mode");
                    bang_bang_mode = true;
                    commands = "";//user's program gets erased after entering bang-bang mode
                  }
                  break;
                case 253://run
                  Serial.println("RUN");
                  if(commands.charAt(commands.length() - 1) == '>'){
                    display.clearDisplay();
                    display.print("\"->\" needs something after it.");
                    display.setCursor(0, 10);
                    display.display();
                  }else if(isDigit(commands.charAt(0))){
                    display.clearDisplay();
                    display.print("A program cannot start with a number");
                    display.setCursor(0, 10);
                    display.display();
                  }else{
                    boolean if_error_flag = false;
                    for(int iter = 0; iter < commands.length(); iter ++){
                      if(commands.charAt(iter) == 'I' && !(commands.charAt(iter + 1) == '0' || commands.charAt(iter + 1) == '1')){
                        display.clearDisplay();
                        display.print(" \"I\" needs either 0 or 1 after it");
                        display.setCursor(0, 10);
                        display.display();
                        if_error_flag = true;
                        break;
                      }
                    }
                    if(!if_error_flag){
                      EEPROM.write(0, commands.length());
                      for(int jj = 1; jj < commands.length() + 1; jj ++){
                        EEPROM.write(jj, commands.charAt(jj - 1));
                      }
                      run_program(commands);
                    }
                  }
                  
                  break;
                case 181:
                  commands += '0';
                  if(commands.charAt(commands.length() - 2) == 'I'){
                    display.print("0->");
                    Serial.print("0->");
                    commands += "->";
                  }else{
                    display.print("0");
                    Serial.print("0");
                  }
                  display.display();
                  break;
                case 151:
                  commands += '1';
                  if(commands.charAt(commands.length() - 2) == 'I'){
                    display.print("1->");
                    Serial.print("1->");
                    commands += "->";
                  }else{
                    display.print("1");
                    Serial.print("1");
                  }
                 display.display();
                  break;      
                case 103:
                  commands += '2';
                   //isLetter = false;
                   Serial.print("2");
                   display.print("2");
                display.display();
                  break; 
                case 79:
                  commands += '3';
                   //isLetter = false;
                   Serial.print("3");
                   display.print("3");
                display.display();
                  break;        
                case 207:
                  commands += '4';
                  // isLetter = false;
                   Serial.print("4");
                   display.print("4");
                display.display();
                  break;
                case 231:
                  commands += '5';
                   //isLetter = false;
                   Serial.print("5");
                   display.print("5");
                display.display();
                  break; 
                case 133:
                  commands += '6';
                   //isLetter = false;
                   Serial.print("6");
                   display.print("6");
                display.display();
                  break;        
                case 239:
                  commands += '7';
                   //isLetter = false;
                   Serial.print("7");
                   display.print("7");
                display.display();
                  break; 
                case 199:
                  commands += '8';
                   //isLetter = false;
                   Serial.print("8");
                   display.print("8");
                display.display();
                  break; 
                case 165:
                  commands += '9';
                   Serial.print("9");
                   display.print("9");
                display.display();
                  break;
              
                
                  
              }
      }
    }else{
      if(digitalRead(leftLight) && !digitalRead(rightLight)){
        cont_turn_right();
      }else if(!digitalRead(leftLight) && digitalRead(rightLight)){
        cont_turn_left();
      }else{
        cont_forward();
      }
    }
}
        
             
  
  
