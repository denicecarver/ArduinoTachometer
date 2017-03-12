int previousSensorValue = -1;
bool dataCollectionState = false;
bool endDataState = false;
unsigned long dataStartTime = 0;
const unsigned long dataCollectionTime = 60000; // 60000 milliseconds == 60 seconds
const int ANALOG_HIGH = 1023;
const int ANALOG_LIGHT_SENSOR_OUTPUT_PIN = 0;
const int ANALOG_BUTTON_OUT_PIN = 1;
const int TOLERANCE = 1;

/************************************* Main Body **************************************************************/
void setup() {
  initializePins();

  //Begin sending output at 9600 baud
  Serial.begin(9600);

}

void loop() {
  // Code here will check for for the start/stop collecting-data event and
  // set the power to the LED transistor appropriately (for now, the default is always on)
  if ((analogRead(ANALOG_BUTTON_OUT_PIN) == ANALOG_HIGH) && !dataCollectionState) {
    startDataCollectionTimer();
    sendCSVHeader();
    dataCollectionState = true;
  }

  // If we have started collecting data and haven't finished
  if ((dataCollectionState) && (!endDataState)) {
    //Read output from Light Sensor and write to serial port
    previousSensorValue = printSensorValueWithinTolerance(analogRead(ANALOG_LIGHT_SENSOR_OUTPUT_PIN));
    // check timer
    if (isTestComplete()) {
      setEndState();
    }
  }
  delay(10);
}
/************************************* End Main Body **************************************************************/

// Sets the Arduino pins for input
void initializePins() {
  pinMode(ANALOG_BUTTON_OUT_PIN, INPUT);
  pinMode(ANALOG_LIGHT_SENSOR_OUTPUT_PIN, INPUT);
}

//Prints header for CSV file
void sendCSVHeader() {
  Serial.println("Voltage,Time Of Measurement");
}

// Records start time for data collection
void startDataCollectionTimer() {
  dataStartTime = millis();
}

//Checks is current time is beyond the start time by the amount dataCollectionTime
bool isTestComplete() {
  return (millis() > dataStartTime + dataCollectionTime);
}

// Marks the end of the file and sets the end-of-data-collection state flag to true
bool setEndState() {
  endDataState = true;
  Serial.println("EOF");
  return endDataState;
}

// Prints the current sensor value along with a timestamp in milliseconds
void printSensorValue(int sensorValue) {
  Serial.print(sensorValue);
  Serial.print(",");
  Serial.println(millis());
}

// switches data collection state between off and on
void setLightSensor(bool powerState) {
  if (powerState) {
    if (!endDataState) {
      dataCollectionState = true;
    }
  } else {
    dataCollectionState = false;
  }
}

// Prints the sensor value if it is outside the window of tolerance 
int printSensorValueWithinTolerance(int sensorValue) {
  // If the value differs from the previous value by the amount TOLERANCE, print the new value
  if ((abs(sensorValue - previousSensorValue)) > TOLERANCE) {
    printSensorValue(sensorValue);
  } else {
    delay(1);
  }
  return sensorValue;
}


