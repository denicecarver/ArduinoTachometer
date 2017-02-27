int previousSensorValue = -1;
bool dataCollectionState = false;
bool endDataState = false;
unsigned long dataStartTime = 0;
const int ANALOG_HIGH = 1023;
const int DIGITAL_LIGHT_SENSOR_PIN = 0;
const int ANALOG_BUTTON_OUT_PIN = 1;
const int DIGITAL_INDICATOR_PIN = 2;
const int TOLERANCE = 1;

/************************************* Main Body **************************************************************/
void setup() {
  initializePins();

  //Begin sending output at 9600 baud
  Serial.begin(9600);

  sendCSVHeader();

  // On startup, turn sensor off
  //setLightSensor(dataCollectionState);
}

void loop() {
  // Code here will check for for the start/stop collecting-data event and
  // set the power to the LED transistor appropriately (for now, the default is always on)
  if (analogRead(ANALOG_BUTTON_OUT_PIN) == ANALOG_HIGH) {
    setIndicatorLight(true);
  } else {
    setIndicatorLight(false);
  }
  delay(5);
  /*
    // If we haven't started collecting
    if (!dataCollectionState) {
      // Check for the button press
      Serial.println("dataCollectionState = FALSE");
      if (digitalRead(POWER_BUTTON_OUT_PIN) == HIGH) {
        // Start light sensor
        setLightSensor(true);
        //start timer
        startDataCollectionTimer();
        // set data collection flag
        dataCollectionState = true;
      }
    } else if (!endDataState) {
      // check timer
      checkTime();
      //Read output from Light Sensor and write to serial port
      previousSensorValue = writeSensorValue(analogRead(A0));
    }
  */
}
/************************************* End Main Body **************************************************************/

void initializePins() {
  pinMode(DIGITAL_LIGHT_SENSOR_PIN, OUTPUT);
  //pinMode(POWER_BUTTON_OUT_PIN, INPUT);
  pinMode(DIGITAL_INDICATOR_PIN, OUTPUT);
}
void sendCSVHeader() {
  Serial.println("Voltage,Time Of Measurement");
}

void startDataCollectionTimer() {
  dataStartTime = millis();
}

bool checkTime() {
  if (millis() > dataStartTime + 60000) {
    setIndicatorLight(false);
    endDataState = true;
    return endDataState;
  }
  return false;
}

void printSensorValue(int sensorValue) {
  Serial.print(sensorValue);
  Serial.print(",");
  Serial.println(micros());
}

void setLightSensor(bool powerState) {
  if (powerState) {
    if (!endDataState) {
      digitalWrite(DIGITAL_LIGHT_SENSOR_PIN, HIGH);
      dataCollectionState = true;
      setIndicatorLight(powerState);
    }
  } else {
    digitalWrite(DIGITAL_LIGHT_SENSOR_PIN, LOW);
    dataCollectionState = false;
  }
}

void setIndicatorLight(bool powerState) {
  if (powerState) {
    digitalWrite(DIGITAL_INDICATOR_PIN, HIGH);
  } else {
    digitalWrite(DIGITAL_INDICATOR_PIN, LOW);
  }
}

int writeSensorValue(int sensorValue) {
  // If the value differs from the previous value by the amount TOLERANCE, print the new value
  if ((abs(sensorValue - previousSensorValue)) > TOLERANCE) {
    printSensorValue(sensorValue);
  } else {
    delay(1);
  }
  return sensorValue;
}


