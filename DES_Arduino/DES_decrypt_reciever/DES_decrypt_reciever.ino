#include <DES.h>
#include <SoftwareSerial.h>
#include <math.h>

SoftwareSerial mySerial(10, 11); //RX, TX
DES des;

uint8_t key[8] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF1};

uint8_t cipherText[8];
uint8_t decryptedText[8];
uint8_t finaldata[sizeof("HELLO its a test!") - 1];
uint8_t i = 0;
const uint8_t dataPack = (uint8_t) ceil((double)(sizeof("HELLO ITS A TEST!")-1)/8);

void readCipherText(uint8_t *cipherText);
void decryptAndStoreText(uint8_t *decryptedText, uint8_t *cipherText, uint8_t *finaldata, uint8_t stage);
void printFinalData(uint8_t *finaldata);

void setup() {
    Serial.begin(9600);
    mySerial.begin(9600);
    Serial.println();

}

void loop() {
    if (mySerial.available()){
        readCipherText(cipherText);

        if (i < dataPack){
            decryptAndStoreText(decryptedText, cipherText, finaldata, i);
            i++;
        }

        if (i == dataPack){
            printFinalData(finaldata);
        }
    }
}

void readCipherText(uint8_t *cipherText){
    for (int i = 0; i < 8; i++){
        uint8_t hexData = mySerial.read();
        if (hexData != 0xff){
            cipherText[i] = hexData;
            delay(50);
        }
    }
}

void decryptAndStoreText(uint8_t *decryptedText, uint8_t *cipherText, uint8_t *finaldata, uint8_t stage){
    des.decrypt(decryptedText, cipherText, key);
    Serial.print("Decrypted text in stage ");
    Serial.print(stage + 1);
    Serial.print(":  ");
    for (uint8_t i = 0; i < 8; i++) {
        Serial.print((char)decryptedText[i]);
        finaldata[stage*8 + i] = decryptedText[i];
    }
    Serial.println();
}

void printFinalData(uint8_t *finaldata){
    Serial.println("Final decrypted text: ");
    for(uint8_t i = 0; i < sizeof(finaldata); i++){
        Serial.print((char)finaldata[i]);
    }
    Serial.println();
}
