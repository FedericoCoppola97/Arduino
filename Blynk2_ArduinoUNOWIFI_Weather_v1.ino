/*************************************************************
Author:      Federico Coppola
Description: Simple weather station using Blynk 2.0 and Arduino UNO WIFI Rev2
             Data collection every 5 minutes.
Version:     1
 *************************************************************/
#define BLYNK_PRINT Serial //debug su serial (COM)

//Parametri necessari, reperibili da blynk.cloud
#define BLYNK_TEMPLATE_ID "XXXXXXXX"
#define BLYNK_DEVICE_NAME "YYYYYYYYYYYYYY"

//Librerie per WIFI, DHT22 e Blynk 2.0
#include <SPI.h>
#include <WiFiNINA.h>
#include <BlynkSimpleWiFiNINA.h>
#include <DHT.h>

//Token di autenticazione. Si ottiene in fase di creazione device su blynk.cloud
char auth[] = "ZZZZZZZZZZZZZZZZZZ";

// Credenziali WIFI (Arduino UNO WIFI Rev2 opera solo in 2.4Ghz). Cifratura WPA2.
char ssid[] = "WIFINETWORK_SSID";
char pass[] = "WIFINETWORK_PASSWORD";

// Costanti per gestione DHT22
#define PIN_DHT 7
#define MODELLO_DHT DHT22
DHT dht22(PIN_DHT, MODELLO_DHT);
float temp=0.0,umid=0.0;

BlynkTimer task_pianificato; //dichiarazione oggetto timer Blynk per pianificazione lettura dati da DHT22

void LetturaDHT(); //prototipo procedura lettura dati DHT22 e gestione virtualPIN Blynk

void setup()
{
  Serial.begin(9600); //inizializzo Seriale per debug
  task_pianificato.setInterval(300000L,LetturaDHT); //esecuzione procedura "LetturaDHT" ogni 5 minuti, ovvero 300000ms
  dht22.begin(); //inizializzo sensore DHT22
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80); //connessione verso Blynk 2.0
  LetturaDHT(); //eseguo prima lettura dati ambientali
}

void loop()
{
  Blynk.run();
  task_pianificato.run();
}

//Procedure per gestione VirtualPIN Blynk e DHT22
void LetturaDHT()
{
  //lettura dati umidità e temperatura
  umid=dht22.readHumidity();
  temp=dht22.readTemperature();

  //Output su console seriale per Debug
  Serial.print("Temperatura: ");Serial.print(temp);
  Serial.print("Umidità: ");Serial.println(umid);

  //Invio dati a Blynk attraverso Internet
  Blynk.virtualWrite(V0, temp);
  Blynk.virtualWrite(V1, umid); 

  Serial.println("Eseguita lettura ed invio dati...");
}
