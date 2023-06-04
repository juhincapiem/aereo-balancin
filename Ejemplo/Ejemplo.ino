// Añadir librerías
#include <OneWire.h> // (!!!! DIGITAL SENSOR !!!!)
#include <DallasTemperature.h> // (!!!! DIGITAL SENSOR !!!!)

// Definir variables
#define SensorInput_GPIO 8 
#define OutputPWM_GPIO 9  
#define pwmRes 12   
#define pwmMax 4095 

// Variables for Conversion
#define Uunits 50 // Set units for Control Output (u) [MAX value of (u) @ MAX pwmDutyCycle] (e.g. MAX Transistor Collector Current [mA])
//#define scaleVadc 5.0 // Set analog read voltage value 3.3 [V] for ESP32 ADC (!!!! ANALOG SENSOR !!!!)

// Execution Time Control
unsigned long pTime = 0;
unsigned long dTime = 0;
long previousMillis = 0;  // For main loop function
long Ts = 1000; // Sample time in ms
long previousMillis2 = 0; // For auxiliary functions (squarewaves)
bool up = true;
int i = 0;

// Measurement Variables
OneWire oneWire(SensorInput_GPIO); // (!!!! DIGITAL SENSOR !!!!)
DallasTemperature TempSensor(&oneWire); // (!!!! DIGITAL SENSOR !!!!)
//int tempAverages = 20; // # of samples for filtering (!!!! ANALOG SENSOR !!!!)
float tempF = 0.0; 

// Control System Variables
float Ref = 35.0; // System Reference - Temperature [°C]
float U_op = 50.0; // Direct Control Output - FOR OPENLOOP or FEEDFORWARD - Transistor Collector Current [mA]
float U_co =0.0;
float U_to = 0.0; // Control Output
float U_id= 0.0;
float kp = 1;
float ki = 0.5;
float E = 0;
float Ep = 0;
float Up = 0;
unsigned int pwmDuty = 0; // Control Output (Converted to PWM Duty Cycle)

// Advanced Serial Input Variables
const byte numChars = 32;
char receivedChars[numChars];
boolean newData = false;

void calibracion(void){
  // Measurement, Control, Output Command Signal, Serial Data Communication
  unsigned long currentMillis = millis(); // Update current time from the beginning
  if (currentMillis - previousMillis >= Ts) {
    previousMillis = currentMillis;
    TempSensor.requestTemperatures();  tempF = TempSensor.getTempCByIndex(0); 
    float U_to = U_op;   
    float U_tl = min(max(U_to, 0), Uunits); // Saturated Control Output
    pwmDuty = int((U_tl/Uunits)*pwmMax);
    //analogWriteADJ(OutputPWM_GPIO, pwmDuty);
    
  
    Serial.print("U:");
    Serial.print(U_tl);
    Serial.print(",");

    Serial.print("tempF:");
    Serial.println(tempF);     
  }
}
void ident(void){
  // Measurement, Control, Output Command Signal, Serial Data Communication

  unsigned long currentMillis = millis(); // Update current time from the beginning

  if (currentMillis - previousMillis >= Ts ) {
    previousMillis = currentMillis;
    TempSensor.requestTemperatures();  tempF = TempSensor.getTempCByIndex(0); 
    float U_to =  U_op + U_id;   
    float U_tl = min(max(U_to, 0), Uunits); // Saturated Control Output
    pwmDuty = int((U_tl/Uunits)*pwmMax);
    analogWriteADJ(OutputPWM_GPIO, pwmDuty); 

    if (currentMillis >= 60000 && currentMillis-previousMillis2 >= 30000) {
    i++;
    previousMillis2 = currentMillis; // refresh the last time you RUN
    if (up){
      U_id = 5.0;
      up = false;
    } else {
       U_id = -5.0;
      up = true;
    }
  }
  if (i >= 5){ 
    while (true){
      analogWriteADJ(OutputPWM_GPIO, 0);
    }
  }

    //Serial.print("curr:");
    //Serial.print(currentMillis/1000);
    //Serial.print(",");
    Serial.print("U:");
    Serial.println(U_tl);
    Serial.print(",");
    Serial.print("tempF:");
    Serial.println(tempF);  

  }

  // Advanced Serial Input Functions
  recvWithStartEndMarkers();  
  if (newData == true) {
    parseData();
    newData = false;
  }
  
}
void controlPI(void){
  // Measurement, Control, Output Command Signal, Serial Data Communication

  unsigned long currentMillis = millis(); // Update current time from the beginning

  if (currentMillis - previousMillis >= Ts ) {
    previousMillis = currentMillis;
    TempSensor.requestTemperatures();  tempF = TempSensor.getTempCByIndex(0); 
    
    float U_to =  U_op + U_co;   
    float U_tl = min(max(U_to, 0), Uunits); // Saturated Control Output
    pwmDuty = int((U_tl/Uunits)*pwmMax);
    analogWriteADJ(OutputPWM_GPIO, pwmDuty); 

    if (currentMillis >= 30000) {
    E = Ref - tempF;
    U_co=//ley de control
    Ep = E;
    Up = U_co;
  }
  
   
    Serial.print("Error:");
    Serial.print(E);
    Serial.print(",");
    Serial.print("U:");
    Serial.print(U_to);
    Serial.print(",");
    Serial.print("Ref:");
    Serial.print(Ref);
    Serial.print(",");
    Serial.print("tempF:");
    Serial.println(tempF);  

  }

   // Advanced Serial Input Functions
  recvWithStartEndMarkers();  
  if (newData == true) {
    parseData();
    newData = false;
  }
  
}

// the setup function runs once when you press reset or power the board
void setup() {
  
  Serial.begin(115200); // Use MAX possible to reduce serial.print() execution time (9600 19200 38400 57600 74880 115200 230400 250000 500000 1000000 2000000)
  
  // Set up inputs (!!!! ANALOG SENSOR !!!!)
  //  pinMode(SensorInput_GPIO, INPUT);

  // Set up inputs (!!!! DIGITAL SENSOR !!!!)
  TempSensor.begin(); // Sensor start command
  TempSensor.setResolution(12); // Sets the maximum supported resolution 12bit. (DS18B20 Accuracy 0.5°C)

  // Set up output
  setupPWMadj();
  analogWriteADJ(OutputPWM_GPIO, pwmDuty);
  //  pinMode(OutputPWM_GPIO, OUTPUT);
  //  analogWrite(OutputPWM_GPIO, pwmDuty);

  delay(5000); // Wait 5 [s] to open Serial Monitor/Plotter
}

// the loop function runs over and over again forever
void loop() {
  controlPI();
}

/* Configure digital pins 9 and 10 as 12-bit PWM outputs (3905 Hz). */
void setupPWMadj() {
  DDRB |= _BV(PB1) | _BV(PB2);        /* set pins as outputs */
  TCCR1A = _BV(COM1A1) | _BV(COM1B1)  /* non-inverting PWM */
      | _BV(WGM11);                   /* mode 14: fast PWM, TOP=ICR1 */
  TCCR1B = _BV(WGM13) | _BV(WGM12)
      | _BV(CS10);                    /* no prescaling */
  ICR1 = 0x0fff;                      /* TOP counter value - SETS RESOLUTION/FREQUENCY */
}

/* 12-bit version of analogWrite(). Works only on pins 9 and 10. (MAX VAL=4095) */
void analogWriteADJ(uint8_t pin, uint16_t val){
  switch (pin) {
    case  9: OCR1A = val; break;
    case 10: OCR1B = val; break;
    }
}

//============ Advanced Serial Input Functions

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<'; // Serial input must start with this character
    char endMarker = '>'; // Serial input must end with this character
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//============

void parseData() {      // split the data into its parts

    Ref = atof(receivedChars);     // convert serial input to a float and update System Reference value with that value

}
