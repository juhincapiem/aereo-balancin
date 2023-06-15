//Variables 
#define PWM1 9 //Puerto para controlar el PWM1
#define PWM2 10 //Puerto para controlar el PWM2
#define sensorInput A0 //Puerto de adquicion de datos del sensor de forma analoga

int pwmDuty1 = 0; //DutyCycle [%] del PWM1
float pwmDuty1_op = 0; //DutyCycle [%] del PWM1 de cambio 
float pwmDuty1_to = 0; //DutyCycle [%] del PWM1 total
float pwmDuty1bite ; //DutyCycle [Num] del PWM1
float pwmDuty2 = 0; //DutyCycle [%] del PWM2
float pwmDuty2bite ; //DutyCycle [Num] del PWM2
int IntputSensor; //Dato en bytes de entra del Sensor 
float actualAng; //Angulo actual sensado [°]
float refAng = 90.0; //Angulo de referencia [°]
long Ts = 1000; // Tiempo de muestreo 
long previousMillis = 0;  // Para el ciclo de la función principal
long previousMillis2 = 0; // Para funciones auxiliares
bool up = true;
int i = 0; //Iterador 


void setup() {
  pinMode(sensorInput, INPUT); //Configuración del puerto de entrada del sensor
  pinMode(PWM1, OUTPUT); //Configuración del puerto de salida del PWM1
  pinMode(PWM2, OUTPUT); //Configuración del puerto de salida del PWM2
  digitalWrite(PWM2, LOW); //Salida del PWM2 0 de forma permanente
  Serial.begin(115200); //Inialización de comunicación serial
  delay(5000); //Espera para correcta comunicación serial
}

void loop() {
  //CalibracionSensor();
  PuntoDeOperacion();
}

void SensarAngulo(){ //Sensa el valor del angulo con los datos adquiridos de forma analoga
  IntputSensor = analogRead(sensorInput);
  actualAng = map(IntputSensor, 0, 1023, 0, 360); //Transfoama el valor de los datos de bits a un valor númerico
  if(actualAng >= 83){ //Calibración del cero de referencia
  actualAng = actualAng - 83;
  } else{
  actualAng = actualAng + 277;}
}

void CalibracionSensor(){

    unsigned long currentMillis = millis(); // Actualiza el tiempo vigente en el arduino
   if (currentMillis - previousMillis >= Ts) {
    SensarAngulo();
    previousMillis = currentMillis;
    pwmDuty1bite = map(pwmDuty1, 0.0, 100.0, 0, 255);
    analogWrite(PWM1, pwmDuty1bite); //Configura el % de PWM en el puerto del PWM1
    Serial.print("U:");
    Serial.print(",");
    Serial.print(pwmDuty1);
    Serial.print(",");
    Serial.print("%");
    Serial.print(",");
    Serial.print("ValorMapeado:");
    Serial.print(",");
    Serial.print(actualAng);
    Serial.print(",");
    Serial.println("°");  
  }
}

void PuntoDeOperacion(){
  unsigned long currentMillis = millis(); // Actualiza el tiempo vigente en el arduino
   if (currentMillis - previousMillis >= Ts) {
    previousMillis = currentMillis;
    pwmDuty1_to = pwmDuty1 + pwmDuty1_op;
    pwmDuty1bite = map(pwmDuty1_to, 0.0, 100.0, 0, 255);
    analogWrite(PWM1, pwmDuty1bite); //Configura el % de PWM en el puerto del PWM1
    SensarAngulo();
   if (currentMillis >= 1000 && currentMillis-previousMillis2 >= 5000) {
    previousMillis2 = currentMillis; // Actualiza el tiempo
    if (actualAng < 90 && pwmDuty1_op < 32){
      pwmDuty1_op = 0.5 + pwmDuty1_op;
    } 
      Serial.print("tiempo:");
      Serial.print(",");
      Serial.print(currentMillis/1000);
      Serial.print(",");
      Serial.print("U:");
      Serial.print(",");
      Serial.print(pwmDuty1_to);
      Serial.print(",");
      Serial.print("%");
      Serial.print(",");
      Serial.print("Grados:");
      Serial.print(",");
      Serial.print(actualAng);
      Serial.print(",");
      Serial.println("°");
   }
   }
}