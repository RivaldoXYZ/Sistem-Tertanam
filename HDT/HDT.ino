#include <SoftwareSerial.h>
SoftwareSerial bt(8, 9); // RX, TX

#include <DHT.h> //library DHT
#define DHTPIN 2 //pin DATA konek ke pin 2 Arduino
#define DHTTYPE DHT11 //tipe sensor DHT11
DHT dht(DHTPIN, DHTTYPE); //set sensor + koneksi pin

float humi, temp;//deklarasi variabel 

void setup()
{
  Serial.begin(9600); //baud 9600
  delay(10);  
}

void loop()
{
  humi = dht.readHumidity();//baca kelembaban
  temp = dht.readTemperature();//baca suhu
  if (isnan(humi) || isnan(temp)) { //jika tidak ada hasil
    Serial.println("DHT11 tidak terbaca... !");
    return;
  }
  else{//jika ada hasilnya 
  Serial.print("Suhu=");  //kirim serial "Suhu"
  Serial.print(temp);     //kirim serial nilai suhu
  Serial.println("C");    //kirim serial "C" Celcius 
  Serial.print("Humi=");  //kirim serial "Humi"
  Serial.print(humi);     //kirim serial nilai kelembaban
  Serial.println("%RH");  //kirim serial "%RH"
  bt.print(temp); //send distance to MIT App
  bt.print("-");
  bt.print(humi); //send distance to MIT App 
  bt.println("-");
  }
  
  delay(1000);            //tunda 1 detik untuk pembacaan berikutnya


}