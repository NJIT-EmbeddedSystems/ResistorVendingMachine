#include <Keypad.h>

const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'u'},
  {'4', '5', '6', 'd'},
  {'7', '8', '9', 'b'},
  {'*', '0', 'E', 'e'}
};

byte rowPins[ROWS] = {22,24,26,28}; 
byte colPins[COLS] = {23,25,27,29}; 

bool color_mode = false;

String key_log, exp_log;

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(9600);
}

void key_event(String &key_log, String &exp_log, char key){
    switch (key){
      case 'u':
        break;
      case 'd':
        break;
      case 'b':
        if(key_log.length() > 0){
          key_log.remove(key_log.length()-1);
        }
        break;
      case 'e':
        // turn on led and clear vars
        key_log ="";
        exp_log =NULL;
        break;
      case 'E':
        if(key_log.length() == 0){
          color_mode != color_mode;
        }else{
          if(exp_log == "Ω"){
            exp_log = "kΩ";
          }else if (exp_log == "kΩ"){
            exp_log = "MΩ";
          }else if (exp_log == "MΩ"){
            exp_log = 'Ω';
          }else{
            exp_log = 'Ω';
          }
        }
        break;
      default:
        key_log += key;
        break;
    }
}

void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    key_event(key_log,exp_log,customKey);
    Serial.println(key_log +" "+ exp_log);
  }
}
