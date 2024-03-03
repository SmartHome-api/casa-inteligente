#include <ESP8266WiFi.h>
#include "DHTesp.h"
//#include "Servo.h"



#define LED 12 
#define LED2 13
//#define PIR 15
#define BUZ 4 
#define REED 5

DHTesp dht;
//Servo servoD1;  

const char* ssid = "";
const char* password = "";

WiFiServer server(80); 
String currentLine = "";
void setup() {
  Serial.begin(115200); 

  dht.setup(14, DHTesp::DHT11); //DHT11
  pinMode(LED, OUTPUT);  //LED 
  pinMode(LED2, OUTPUT); //LED2
 // pinMode(PIR, INPUT); //Sensor de presença
  pinMode(BUZ, OUTPUT);
  pinMode(REED, INPUT);
  //servoD1.attach(D1);

  delay(10);


  Serial.println();
  Serial.println();
  Serial.println("Conectando à");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }

  IPAddress ip(192,168,144,98);
  IPAddress gateway(192,168,144,98);
  IPAddress subnet(255,255,255,0);

  WiFi.config(ip, gateway, subnet);

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Endereço IP");
  Serial.println(ip);

  server.begin();

}

int value = 0; 

void loop() {

  WiFiClient client = server.available();

  String u = String(dht.getHumidity());   //Declaração da variável de umidade 
  String t = String(dht.getTemperature()); //Declaração da variável de temperatura 
  float g = analogRead(A0);
  //Verifica o status do sensor de presença:  

  if(client) {
    Serial.println("New Client.");
    currentLine = "";
    while(client.connected()){
      if(client.available()){
         char c = client.read();
         Serial.write(c); 

         if(c == '\n') {


            if(currentLine.length() == 0){

                 client.println("HTTP/1.1 200 OK");
                 client.println("Content-type:text/html");
                 client.println();

                 //Para LEDS diferentes 
                 //client.println("<a href=\"/botl\"></a><br>");
                 //client.println("<a href=\"/botd\"></a><br>"); 
                
                 //client.println("<a href=\"/bot2l\"></a><br>");
                 //client.println("<a href=\"/bot2d\"></a><br>"); 
                 
                 client.print("Umidade: " + u);   //String de umidade 
                 client.print("Temperatura: " + t); //String de temperatura 
                 client.print("Gas: " + String(g));
                 //client.print("Presenca: ");
                 //int pres = digitalRead(PIR);    
                 int por = digitalRead(REED);              
                 if(por == 0){
                   client.print("SPor");
                 }else{
                   client.print("NPor");
                 }

                client.println();

                 break; 
            }else{
              currentLine = "";  
            }
            
         } else if(c != '\r'){
           currentLine += c; 
         }

        //Para LEDS diferentes - verifica a URL enviada como requerimento do cliente para o servidor 
         if(currentLine.endsWith("GET /botl")){
            digitalWrite(LED, HIGH);   
         }else if(currentLine.endsWith("GET /botd")){
            digitalWrite(LED, LOW);  
         }

         if(currentLine.endsWith("GET /bot2l")){
            digitalWrite(LED2, HIGH);   
         }else if(currentLine.endsWith("GET /bot2d")){
            digitalWrite(LED2, LOW);  
         }

             

         
      }
        
    }

    client.stop();
    Serial.println("Client Disconnected.");
  }

  if(g > 600)
  {
    digitalWrite(BUZ, HIGH);
  }
  else
  {
    digitalWrite(BUZ, LOW);
  }

  delay(10);

  

}
