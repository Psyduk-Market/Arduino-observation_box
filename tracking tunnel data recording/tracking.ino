//======================================================
//
//		Author: Thyme N
//		Course: ENEL200
//		Project: Tracking Tunnel data recording
//
//======================================================
//			Initialize pins and gobal variables
//======================================================
const int analogPin1 = A0;     // Right sensor
const int analogPin2 = A1;	   // Left sensor

const int TypeSwitch = 2;	   // Switch for select rat or stoat
const int displayButton = 11;  // display button
const int resetPin = 12;       // Reset pin input
const int outPin = 13;         // Output

// Initialising pins for two 7 segments
const int v1_0 = 3;
const int v1_1 = 4;
const int v1_2 = 5;
const int v1_3 = 6;

const int v2_0 = 7;
const int v2_1 = 8;
const int v2_2 = 9;
const int v2_3 = 10;

int count = 0;

int onesR = 0;
int tensR = 0;

int onesS = 0;
int tensS = 0;

// Array for tens and ones
byte v1[4] = {};
byte v2[4] = {};

// check if the animal stop at the sensor
bool atSensor = 0;

//======================================================
//			Setup pins
//======================================================

void setup() {
  Serial.begin(9600);     // Bit per second
  pinMode(analogPin1, INPUT);
  pinMode(analogPin2, INPUT);
  pinMode(typeSwitch, INPUT_PULLUP);
  pinMode(resetPin, INPUT_PULLUP);
  pinMode(outPin, OUTPUT);
  pinMode(displayButton, INPUT_PULLUP);

  pinMode(v1_0, OUTPUT);
  pinMode(v1_1, OUTPUT);
  pinMode(v1_2, OUTPUT);
  pinMode(v1_3, OUTPUT);

  pinMode(v2_0, OUTPUT);
  pinMode(v2_1, OUTPUT);
  pinMode(v2_2, OUTPUT);
  pinMode(v2_3, OUTPUT);
}

//======================================================
//			Function to handle convertion and display
//======================================================

void displayValue() {
	// Convert every 10 counter to tens
	if (onesR > 9) 
	{
		tensR++;
		onesR = 0;
	}

	if (onesS > 9)
	{
	tensS++;
	onesS = 0;
	}

	if (!digitalRead(displayButton) && digitalRead(typeSwitch))
	{
		int j = 0;
		for (byte i=0; i<4; i++)
		{
		v1[j] = bitRead(onesR, i);
		v2[j] = bitRead(tensR, i);
		j++;
		}
	} else if (!ditalRead(displayButton) && digitalRead(typeSwitch))
	{
		int j = 0;
		for (byte i=0; i<4; i++)
		{
		v1[j] = bitRead(onesS, i);
		v2[j] = bitRead(tensS, i);
		j++
		}
	}
 	
	digitalWrite(v1_0, v1[0]);
  	digitalWrite(v1_1, v1[1]);
  	digitalWrite(v1_2, v1[2]);
  	digitalWrite(v1_3, v1[3]);

  	digitalWrite(v2_0, v2[0]);
  	digitalWrite(v2_1, v2[1]);
  	digitalWrite(v2_2, v2[2]);
  	digitalWrite(v2_3, v2[3]);

}

//======================================================
//			Function to count and define type of species
//======================================================


void sensorsControl(flat distanceS1, float distanceS2) {
	
	float spaceS1 = 15 - distanceS1;
	float spaceS2 = 15 - distanceS2;

	float totalSpace = spaceS1 + spaceS2;

	if ((distanceS1 < 20) && (distanceS1 > 0) && (atSensor == 0))
	{
		count++;
		atSensor = 1;

		if ((totalSpace < 5) && (count == 2)) {
			onesR++;
			count = 0;
		} else if ((totalSpace >= 5) && (count == 2)) {
			onesS++;
			count = 0;
		}

		delay(50);
		Serial.println("detected");
	} else if (distanceS1 >= 20) {
		atSensor = 0;
	}

}

//======================================================
//			Main function to read and calling functions
//======================================================

void loop() {
  
  float adcCode1 = analogRead(analogPin1);          // Reading value from the right sensor into digital
  float voltage1 = (adcCode / (pow(2,10)-1) * 5);   // convert to Voltage (Vin)
  float distanceS1 = 27/voltage;                    // 27 is gradient

  float adcCode2 = analogRead(analogPin2);          // Reading value from the left sensor into digital
  float voltage2 = (adcCode / (pow(2,10)-1) * 5);   // convert to Voltage (Vin)
  float distanceS2 = 27/voltage;                    // 27 is gradient

  sensorsControl(distanceS1, distanceS2);

  if (digitalRead(displayButton))
  {
  	displayValue();
  }

  if (digitalRead(resetPin))
  {
    ones = 0;
    tens = 0;
  }

//  delay(1000); // remove when running demo
}
