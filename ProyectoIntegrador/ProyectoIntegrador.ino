const int segA = 22;
const int segB = 23;
const int segC = 24;
const int segD = 25;
const int segE = 26;
const int segF = 27;
const int segG = 28;
const int segDP = 29;

const int dig1 = 30;
const int dig2 = 31;
const int dig3 = 32;
const int dig4 = 33;

const int ledIzq = 2;
const int ledDer = 3;
const int ledArr = 4;
const int ledAbj = 5;
const int pinTemperatura = A0;
const int pinJoyX = A1;
const int pinJoyY = A2;

const int trigPin = 8;
const int echoPin = 9;
const int buzzerPin = 10;

bool activarTemp = false;
bool activarUltra = false;
float distanciaActual = 0;

void setup() {
  for (int i = 22; i <= 29; i++) pinMode(i, OUTPUT);
  for (int i = 30; i <= 33; i++) pinMode(i, OUTPUT);

  pinMode(ledIzq, OUTPUT);
  pinMode(ledDer, OUTPUT);
  pinMode(ledArr, OUTPUT);
  pinMode(ledAbj, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // Conexión con Python
  if (Serial.available()) {
    String comando = Serial.readStringUntil('\n');
    comando.trim();
    if (comando == "T1") activarTemp = true;
    else if (comando == "T0") activarTemp = false;
    else if (comando == "U1") activarUltra = true;
    else if (comando == "U0") activarUltra = false;
    else if (comando == "B0") digitalWrite(buzzerPin, LOW);
  }

  // Sensor de temperatura
  if (activarTemp) {
    int lectura = analogRead(pinTemperatura);
    float voltaje = lectura * 5.0 / 1023.0;
    float temperatura = (voltaje - 0.5) * 100.0;
    Serial.println("TEMP:" + String(temperatura));
    digitalWrite(buzzerPin, HIGH);
  }

  // Sensor ultrasónico
  if (activarUltra) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duracion = pulseIn(echoPin, HIGH);
    float distancia = duracion * 0.034 / 2;
    distanciaActual = distancia;
    Serial.println("DIST:" + String(distancia));

    if (distancia < 10.0) {
      digitalWrite(buzzerPin, HIGH);
    }
  }

  // Encender LEDs
  int ejeX = analogRead(pinJoyX);
  int ejeY = analogRead(pinJoyY);

  
  int umbralAlto = 700;
  int umbralBajo = 300;

  digitalWrite(ledIzq, ejeX < umbralBajo);
  digitalWrite(ledDer, ejeX > umbralAlto);
  digitalWrite(ledArr, ejeY < umbralBajo);
  digitalWrite(ledAbj, ejeY > umbralAlto);

  delay(300);
}