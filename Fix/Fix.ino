#include <SoftwareSerial.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 11         // Pin data sensor DHT11
#define DHTTYPE DHT11    // Jenis sensor DHT Anda (DHT11, DHT21, atau DHT22)

#define SOIL_MOISTURE A0          // Pin sensor kelembaban tanah
#define RELAY_PIN 3                // Pin relay 
#define BUZZER_PIN 10               // Pin buzzer

SoftwareSerial BT(8, 9);    // RX, TX (pin yang berbeda dengan Serial)
const int col = 16;
const int row = 2;
LiquidCrystal_I2C lcd(0x27, col, row);

DHT dht(DHTPIN, DHTTYPE);


int soilMoisture;
float temperature;
float humidity;

void monitor();

void setup() {
  Serial.begin(9600);
  BT.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  dht.begin();
}

void loop() {
  digitalWrite(RELAY_PIN, HIGH);
  soilMoisture = analogRead(SOIL_MOISTURE);
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  monitor();
  // if (Serial.available() > 0) {
  //   char command = Serial.read();

  //   if (command == '1') {
  //     digitalWrite(RELAY_PIN, HIGH);
  //     lcd.println("Pompa air telah dihidupkan");
  //   } else if (command == '0') {
  //     digitalWrite(RELAY_PIN, HIGH);
  //     lcd.println("Pompa air telah dimatikan");
  //   } else {
  //     lcd.println("Perintah tidak valid");
  //   }
  // }
  
  // Periksa apakah sensor DHT berhasil membaca suhu
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Gagal membaca data dari sensor DHT!");
    return;
  }

  // Mengirimkan data ke Android dengan menggunakan Bluetooth
  BT.print(temperature);
  BT.print(" °C");
  BT.print(",");
  BT.print(humidity);
  BT.print(" %");
  BT.print(",");
  BT.print(soilMoisture); 
  BT.print(";");


  // Periksa apakah kelembaban tanah dan suhu memenuhi ambang batas
  if (soilMoisture < 500 ) {
    digitalWrite(RELAY_PIN, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Tanah Basah");
    Serial.println("Tanah masih dalam keadaan Basah");
    digitalWrite(BUZZER_PIN, 0); 
    }
  else if(soilMoisture < 900 ){
    lcd.setCursor(0, 1);
    lcd.print("Kelembapan Cukup");
    Serial.println("Kelembapan Tanah Masih Cukup");
    digitalWrite(RELAY_PIN, HIGH);
    analogWrite(BUZZER_PIN, 127);
    delay(500);
    analogWrite(BUZZER_PIN, 0);
  }
  else {
    lcd.clear();
    lcd.println("Menyiram tanaman...");
    analogWrite(BUZZER_PIN, 255);
    digitalWrite(RELAY_PIN, LOW); // Nyalakan pompa air
    delay(3000); // Biarkan pompa air menyiram selama 5 detik (sesuaikan sesuai kebutuhan Anda)
    digitalWrite(RELAY_PIN, HIGH); // Matikan pompa air setelah selesai menyiram
    analogWrite(BUZZER_PIN, 0); // Matikan buzzer
  }
  delay(500);
}

void monitor(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Humi: ");
  lcd.print(humidity);
  lcd.print("% ");
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("'C");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Moisture: ");
  lcd.print(soilMoisture);
  delay(2000);
}
