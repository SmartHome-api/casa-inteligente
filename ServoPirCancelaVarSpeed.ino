/*
 * Projeto que aciona, abre e fecha uma cancela automaticamente através de dois sensores de presença
 * PIR 501.
 * Componentes:
 * . 1 Servo motor com cancela acoplada.
 * . 2 Sensor de presença PIR 501.
 * . 1 Placa Arduino Nano.
 * . 2 LEDS.
 * . 2 Resistores de 180 ohm.
 * . Fios.
 * . Fonte externa (ligar servo e sensor PIR).
 */

#include <VarSpeedServo.h>

VarSpeedServo myservo;        // create servo object to control a servo

//int pos = 0;          // variable to store the servo position

int pinLedRed = 5;    //Pino ligado ao led vermelho
int pinLedGreen = 6;  //Pino ligado ao led azul

int pinPirOpen = 3;   //Pino ligado ao sensor PIR 1
int pinPirClose = 4;  //Pino ligado ao sensor PIR 2

int acionamentoOpen;  //Variavel para guardar valor do sensor 1
int acionamentoClose; //Variavel para guardar valor do sensor 2

void setup() {
  Serial.begin(9600);
  Serial.println("Code ServoPirCancelaVarSpeed.ino");
  
  // servo
  myservo.attach(9);   // attaches the servo on pin 9 (PWM) to the servo object
  myservo.write(0);    // initial position of servo
  delay(3000);
  
  // leds
  pinMode(pinLedRed, OUTPUT);   //Define pino como saida
  pinMode(pinLedGreen, OUTPUT); //Define pino como saida

  // pir's
  pinMode(pinPirOpen, INPUT);   //Define pino sensor como entrada
  pinMode(pinPirClose, INPUT);  //Define pino sensor como entrada
}

void loop() {
  // pir 1
  acionamentoOpen = digitalRead(pinPirOpen);  // ler sinal do sensor PIR 1
  
  if (acionamentoOpen == LOW)       //Sem movimento, mantem led verde ligado
  {
    digitalWrite(pinLedRed, LOW);
    digitalWrite(pinLedGreen, HIGH);
  }
  else  //Caso seja detectado um movimento, aciona o led vermelho
  {
    // leds
    digitalWrite(pinLedRed, HIGH);
    digitalWrite(pinLedGreen, LOW);

    // servo - sobe cancela
    myservo.slowmove(90, 10);   //posição, velocidade    
    delay(5000);               // tempo de espera
    
    // ler sinal do sensor PIR 2
    acionamentoClose = digitalRead(pinPirClose);
    while (acionamentoClose == LOW) {
      acionamentoClose = digitalRead(pinPirClose);
    }

    // servo - desce cancela
    myservo.slowmove(0, 10);   //posição, velocidade    
    delay(5000);
    
    acionamentoClose == LOW;
  }
}
