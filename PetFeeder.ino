#include <LiquidCrystal_I2C.h>
#include <DS3231.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
DS3231 rtc(SDA, SCL);
Time t;

int portions_crt = 1;
int interval = 1;

int next_feed_hour = 19;
int next_feed_minute = 24;
int next_feed_seconds = 01;

void update_lcd() {
  lcd.setCursor(0, 0);
  lcd.print("Interval: ");
  lcd.print(interval);
  lcd.setCursor(0, 1);
  lcd.print("Portii: ");
  lcd.print(portions_crt);
}

void setup() {
  lcd.init();
  lcd.backlight();
  rtc.begin();
  Serial.begin(9600);
}

void loop() {
  t = rtc.getTime();

  Serial.print("Current Time: ");
  Serial.print(t.hour);
  Serial.print(":");
  Serial.print(t.min);
  Serial.print(":");
  Serial.println(t.sec);

  Serial.print("Next Feeding Time: ");
  Serial.print(next_feed_hour);
  Serial.print(":");
  Serial.print(next_feed_minute);
  Serial.print(":");
  Serial.println(next_feed_seconds);
}

