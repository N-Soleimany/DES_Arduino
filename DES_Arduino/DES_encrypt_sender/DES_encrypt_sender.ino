#include <DES.h>
#include <SoftwareSerial.h>
#include <math.h>

SoftwareSerial mySerial(10, 11); //RX, TX

DES des;

//64 bit key
uint8_t key[8] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF1};

uint8_t plainText[8]; //8 byte block
uint8_t cipherText[8];
uint8_t unpacked[] = "HELLO its a test!";
const uint8_t dataPack = (uint8_t) ceil((double)(sizeof(unpacked)-1)/8);
uint8_t i = 0;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println();

  Serial.print("Original text: ");
  for (int i = 0; i < sizeof(unpacked)-1; i++){
    Serial.print((char)unpacked[i]);
  }
  Serial.println();
  Serial.print("The original text is ");
  Serial.print(sizeof(unpacked)-1);
  Serial.println(" Bytes");
  Serial.print(dataPack);
  Serial.println(" times 8 (DES goes by 8 Byte packs)");
}

void loop(){
  if (i < dataPack){
    memcpy(plainText, &unpacked[i*8], 8*sizeof(uint8_t));
    //encrypt
    des.encrypt(cipherText, plainText, key);

    Serial.print("Cipher text stage ");
    Serial.print(i + 1);
    Serial.print(":  ");
    for (int j = 0; j < 8; j++){
      Serial.print(" 0x");
      Serial.print(cipherText[j], HEX);
    }
    Serial.println();

    //send   
    mySerial.write(cipherText, 8);

    i++;   
    delay(3000);    
  } 
}
