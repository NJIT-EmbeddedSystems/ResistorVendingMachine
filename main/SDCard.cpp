#include <SPI.h>
#include <SD.h>

File myFile;

void sdCard_init() {
    //initalize SD card
    SD.begin();  
    
    //Creates txt file if not found
    if (!SD.exists("resistorInventory.txt")) {
        SD.open("resistorInventory.txt");
        SD.close("resistorInventory.txt");
    }
}







