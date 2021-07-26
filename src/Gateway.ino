#include <SPI.h>
#include <RH_RF69.h>

#define RFM69_CS  10
#define RFM69_INT 2
#define FREQUENCY 915.0
#define TRANSMIT_POWER 14 //(ranges from 14-20dBi)
#define KEY "encryptionkey123" //(must be 16 chars and match all Nodes)

//initiate driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);

void setup()
{
  Serial.begin(9600);
  // NOTE: remove if not tethered to computer:
  while (!Serial)
  {
    delay(1); //wait until console opens
  }

  
  if (!rf69.init())
    Serial.println("init failed");
  if (!rf69.setFrequency(FREQUENCY))
    Serial.println("setFrequency failed");
  rf69.setTxPower(TRANSMIT_POWER);
  //must match server's key:
  //rf69.setEncryptionKey(KEY);
}

void loop() {
  Serial.println("Listening...");
  if (rf69.available())                      
  {
     // Should be a message for us now   
    uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf69.recv(buf, &len))
    {
      Serial.print("got request: ");
      Serial.print((char*)buf);
    }
    else
    {
      Serial.println("recv failed");
    }
  }
}
