//Variables 
#define PWM1 9 //Puerto para controlar el PWM1
#define PWM2 10 //Puerto para controlar el PWM2
#define sensorInput A0 //Puerto de adquicion de datos del sensor de forma analoga

int pwmDuty1 = 0; //DutyCycle [%] del PWM1
float pwmDuty1_op = 0; //DutyCycle [%] del PWM1 de cambio 
float pwmDuty1_to = 0; //DutyCycle [%] del PWM1 total
int pwmDuty1bite ; //DutyCycle [Num] del PWM1
int pwmDuty2 = 0; //DutyCycle [%] del PWM2
int pwmDuty2bite ; //DutyCycle [Num] del PWM2
int IntputSensor; //Dato en bytes de entra del Sensor 
int actualAng; //Angulo actual sensado [°]
int refAng = 90; //Angulo de referencia [°]
long Ts = 10; // Tiempo de muestreo 
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
  CalibracionSensor();
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
   if (currentMillis >= 1000 && currentMillis-previousMillis2 >= 2000) {
    i++;
    previousMillis2 = currentMillis; // refresh the last time you RUN
    if (up){
      pwmDuty1_op = 5.0;
      up = false;
    } else {
       pwmDuty1_op = -5.0;
      up = true;
    }
  }
  if (i >= 5){ 
    while (true){
      analogWrite(PWM1, 0);
    }
  }
      Serial.print("tiempo:");
      Serial.print(",");
      Serial.print(currentMillis/1000);
      Serial.print(",");
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