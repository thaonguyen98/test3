
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* Cai Đăt WIFI va Mât Khâu *********************************/

#define WLAN_SSID       "NGUYEN VAN THAO"
#define WLAN_PASS       "11051998"

/************************* Khai bao Adafruit.io  *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "thaonguen"
#define AIO_KEY         "2ae99187b8e54dbeaf4cd0c6798b48bd"



// Tao mot ESP8266 WiFiClient Đê Kêt Nôi Vơi sever MQTT.
WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Cai Đăt trong io.adafruit ***************************************/


Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/photocell");


Adafruit_MQTT_Subscribe cocacola = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/cocacola");
Adafruit_MQTT_Subscribe hongtra = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/hongtra");
Adafruit_MQTT_Subscribe trada = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/trada");
Adafruit_MQTT_Subscribe Device4 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Device4");


/*************************** ************************************/


void MQTT_connect();   //ket noi voi MQTT

void setup() {
   // khai bao cac chan trang thai ban dau la muc cao vi module relay kich muc thap.
  pinMode(D0, OUTPUT);
  digitalWrite(D0, HIGH);//tat_den
  pinMode(D1, OUTPUT);
  digitalWrite(D1, HIGH);//tat_den
   pinMode(D2, OUTPUT);
  digitalWrite(D2, HIGH);//tat_den
  pinMode(D3, OUTPUT);
  digitalWrite(D3, HIGH);//tat_den
  pinMode(D4, OUTPUT);
  digitalWrite(D4, HIGH);//tat_den

  /*******************************************/
  Serial.begin(115200);  //toc đo baud.
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

  // thong bao ket noi voi wifi.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");  //khi dang ket noi.
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");  //ket noi wifi thanh cong.
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // cai dăt MQTT on off trong phan feed..
  mqtt.subscribe(&cocacola);
  mqtt.subscribe(&hongtra);
  mqtt.subscribe(&trada);
  mqtt.subscribe(&Device4);
 
}

uint32_t x=0;

void loop() {
  // Đam bao ket noi MQTT đen may chu van con hoat dong.
  // ket noi va tu dong ket noi lai khi b ngat ket noi.
  
  MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &cocacola) {
      Serial.print(F("Got: "));
      Serial.println((char *)cocacola.lastread);
      if (!strcmp((char*) cocacola.lastread, "ON"))
      {
        //khi dieu khien bat thiet bi thi cho muc logic xuong thap(LOW).
        digitalWrite(D0, LOW);
        delay(6000);
        digitalWrite(D0, HIGH);
      }
    }
     if (subscription == &hongtra) {
      Serial.print(F("Got: "));
      Serial.println((char *)hongtra.lastread);
      if (!strcmp((char*) hongtra.lastread, "ON"))
      {
       
        digitalWrite(D1, LOW);
        delay(6000);
        digitalWrite(D1, HIGH);
      }
     } 
      if (subscription == &trada) {
        Serial.print(F("Got: "));
        Serial.println((char *)trada.lastread);
        if (!strcmp((char*) trada.lastread, "ON"))
        {
        
        digitalWrite(D2, LOW);
        delay(6000);
        digitalWrite(D2, HIGH);
        }
      }

      //viet_cho_den4//
      if (subscription == &Device4) {
        Serial.print(F("Got: "));
        Serial.println((char *)Device4.lastread);
        if (!strcmp((char*) Device4.lastread, "ON"))
        {
        
        digitalWrite(D3,LOW);
        }
        else
        {
          digitalWrite(D3, HIGH);
        }
      }
        
  }

 
  Serial.print(F("\nSending photocell val "));
  Serial.print(x);
  Serial.print("...");
  if (! photocell.publish(x++)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

 
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */
}


void MQTT_connect() {
  int8_t ret;

  // Dung neu da ket noi
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");   //in ra man hinh may tinh da ket noi vơi MQTT.

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // Ket noi tra ve 0 cho da ket noi
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  
       retries--;
       if (retries == 0) {
         
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
