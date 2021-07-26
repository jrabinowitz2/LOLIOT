#include <SPI.h>
#include <RH_RF69.h>
#include <Wire.h>
#include <SparkFunTMP102.h>

#define RFM69_CS	10
#define RFM69_INT	2
#define FREQUENCY 915.0
#define TRANSMIT_POWER 14 //(ranges from 14-20dBi)
#define KEY "encryptionkey123" // (must be 16 chars)
#define AREA "Kitchen" // (set a unique AREA for each node!)

//initiate driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);
TMP102 sensor0(0x48);

void setup()
{
	Serial.begin(9600);
	// NOTE: remove if not tethered to computer:
	/*while (!Serial)
	{
		delay(1); //wait until console opens
	}*/
	sensor0.begin();

	if (!rf69.init())
		Serial.println("init failed");
	if (!rf69.setFrequency(FREQUENCY))
		Serial.println("setFrequency failed");
	rf69.setTxPower(TRANSMIT_POWER, true);
	//must match server's key:
	//rf69.setEncryptionKey(KEY);
}

void loop()
{
  //read temp
  sensor0.wakeup();
  float temperature = sensor0.readTempF();
  sensor0.sleep();

  //convert to data
  String tempStr = String(temperature);
  String mid = ", ";
  String fullStr = AREA+mid+tempStr;
  uint8_t data[sizeof(fullStr)];
  fullStr.getBytes(data, sizeof(data));

  //send to gateway
  if (rf69.send(data, sizeof(data)))
    Serial.println("[+] sending temp...");
  else
    Serial.println("[!] send failed!");
  delay(1000);
}
