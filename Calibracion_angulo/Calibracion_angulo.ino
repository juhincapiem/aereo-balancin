//Variables 
#define PWM1 9
#define PWM2 10
#define sensorInput A0

int pwmDuty1 = 0;
int pwmDuty2 = 0;
long IntputSensor = 0;
long actualAng = 0;
long refAng = 84;
long tiempoAnterior = 0;
long Ts = 10;

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
  unsigned long tiempoReal = millis();

  if (tiempoReal - tiempoAnterior >= Ts){ 
    tiempoAnterior = tiempoReal;
    IntputSensor = analogRead(sensorInput);
    actualAng = (IntputSensor * 360)/1023;
    //Serial.println(IntputSensor);
    //Serial.println(actualAng);
    long angle = actualAng - refAng;
    //Serial.println(angle);
    Serial.println(angle);

  }
}

void Calibrazao(){

  analogWrite(PWM1, pwmDuty1);

}


