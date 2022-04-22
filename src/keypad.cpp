#include <Keypad.h>

const byte ROWS, COLS;
char hexaKeys[ROWS][COLS];

Keypad customKeypad;

void init(){
    hexaKeys= {
        {'1', '2', '3', 'u'},
        {'4', '5', '6', 'd'},
        {'7', '8', '9', 'b'},
        {'.', '0', 'E', 'e'}
    };

    customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

}
char getkey(){
    char key = customKeypad.getKey();
    if(key){
        return key;
    }
    return;
}
