int sensorValue = 0;
int prevValue = -1;
bool powerState = false;
const int POWER_SOURCE_PIN = 13;
const int TOLERANCE = 1;

/************************************* Main Body **************************************************************/
void setup() {
  //Begin sending output at 9600 baud
  Serial.begin(9600);

  sendCSVHeader();

  // On startup, turn sensor off
  turnOffLightSensor();
}

void loop() {
  // Code here will check for for the start/stop collecting-data event and
  // set the power to the LED transistor appropriately (for now, the default is always on)
  turnOnLightSensor();

  //Read output from LED Transistor
  sensorValue = analogRead(A0);

  // If the value differs from the previous value by the amount TOLERANCE, print the new value
  if ((abs(sensorValue - prevValue)) > TOLERANCE) {
    printSensorValue();
  }
  prevValue = sensorValue;
  delay(10);
}
/************************************* End Main Body **************************************************************/


void printSensorValue() {
  Serial.print(sensorValue);
  Serial.print(",");
  Serial.println(micros());
}

void turnOnLightSensor() {
  // Set digital pin POWER_SOURCE_PIN to output voltage
  if (!isPowerOn()) {
    pinMode(POWER_SOURCE_PIN, OUTPUT);
    digitalWrite(POWER_SOURCE_PIN, HIGH);
    powerState = true;
  }
}

void turnOffLightSensor() {
  if (isPowerOn()) {
    // Set digital pin POWER_SOURCE_PIN to zero volts
    digitalWrite(POWER_SOURCE_PIN, LOW);
    powerState = false;
  }
}

bool isPowerOn() {
  return powerState;
}

void sendCSVHeader() {
  Serial.println("Voltage,Time Of Measurement");
}

