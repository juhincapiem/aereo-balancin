//Variables 
#define PWM1 9 //Puerto para controlar el PWM1
#define PWM2 10 //Puerto para controlar el PWM2
#define sensorInput A0 //Puerto de adquicion de datos del sensor de forma analoga

int pwmDuty1 = 50; //DutyCycle [%] del PWM1
int pwmDuty2 = 0; //DutyCycle [%] del PWM2
int IntputSensor; //Dato en bytes de entra del Sensor 
int actualAng; //Angulo actual sensado [°]
int refAng = 90; //Angulo de referencia [°]
long Ts = 1000; // Tiempo de muestreo 
long previousMillis = 0;  // Para el ciclo de la función principal

void setup() {
  pinMode(sensorInput, INPUT); //Configuración del puerto de entrada del sensor
  pinMode(PWM1, OUTPUT); //Configuración del puerto de salida del PWM1
  pinMode(PWM2, OUTPUT); //Configuración del puerto de salida del PWM2
  digitalWrite(PWM2, LOW); //Salida del PWM2 0 de forma permanente
  Serial.begin(115200); //Inialización de comunicación serial
  delay(5000); //Espera para correcta comunicación serial
}

void loop() {
  Calibracion();
}

void SensarAngulo(){ //Sensa el valor del angulo con los datos adquiridos de forma analoga
  IntputSensor = analogRead(sensorInput);
  actualAng = map(IntputSensor, 0, 1023, 0, 360); //Transfoama el valor de los datos de bits a un valor númerico
  if(actualAng >= 83){ //Calibración del cero de referencia
  actualAng = actualAng - 83;
  } else{
  actualAng = actualAng + 277;}
  Serial.print("ValorMapeado:");
  Serial.print(actualAng);
  Serial.println("°");
  delay(1000); //Espera para siguente muestra
}

void Calibracion(){

  unsigned long currentMillis = millis(); // Actualiza el tiempo vigente en el arduino
   if (currentMillis - previousMillis >= Ts) {
    
     previousMillis = currentMillis;
    analogWrite(PWM1, pwmDuty1); //Configura el % de PWM en el puerto del PWM1
    SensarAngulo();
  
  }
}
