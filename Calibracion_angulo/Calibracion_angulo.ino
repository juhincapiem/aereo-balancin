//Variables 
#define PWM1 9
#define PWM2 10
#define sensorInput A0

int pwmDuty1 = 0;
int pwmDuty2 = 0;
int IntputSensor = 0;
int actualAng = 0;
int refAng = 90;

void setup() {
  pinMode(sensorInput, INPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  digitalWrite(PWM2, LOW);
  Serial.begin(115200);
  delay(5000);
}

void loop() {
  Calibrazao();
  measureAng();
}

void measureAng(){
  IntputSensor = analogRead(sensorInput);

  actualAng = (IntputSensor * 360)/ 1023;
  Serial.println(IntputSensor);
  Serial.println(actualAng);
  delay(1000);
}

void Calibrazao(){

  analogWrite(PWM1, pwmDuty1);

}


