//Variables 
#define PWM1 9
#define PWM2 10
#define sensorInput A0

int pwmDuty1 = 50;
int pwmDuty2 = 0;
int IntputSensor;
int actualAng;
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
  actualAng = map(IntputSensor, 0, 1023, 0, 360);
  if(actualAng >= 83){
  actualAng = actualAng - 83;
  } else{
  actualAng = actualAng + 277;}
  /*Serial.print("ValorBits:");
  Serial.print(IntputSensor);*/
  Serial.print("ValorMapeado:");
  Serial.print(actualAng);
  Serial.println("°");
  delay(1000);
}

void Calibrazao(){

  analogWrite(PWM1, pwmDuty1);

}
