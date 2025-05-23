#include <LiquidCrystal_I2C.h>
#include <DS3231.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
DS3231 rtc(SDA, SCL);
Time t;

#define BTN_PORTII PD2  // INT0
#define BTN_ORA    PD3  // INT1
#define BTN_MANUAL PD5  
#define SERVO_PIN  PB1  // OC1A = pin 9

int portions_crt = 1;
int interval = 1;

int next_feed_hour = 19;
int next_feed_minute = 24;
int next_feed_seconds = 01;

void setup_gpio() {
  DDRD &= ~((1 << BTN_PORTII) | (1 << BTN_ORA) | (1 << BTN_MANUAL)); 
  PORTD |= (1 << BTN_PORTII) | (1 << BTN_ORA) | (1 << BTN_MANUAL);   
}

void setup_pwm_servo() {
  DDRB |= (1 << SERVO_PIN); 

  TCCR1A = (1 << COM1A1) | (1 << WGM11);              // Fast PWM
  TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);  // Prescaler 8
  ICR1 = 39999; // 20ms (50Hz) PWM
}

void set_servo_angle(int angle) {
  uint16_t pulse_us = map(angle, 0, 180, 1000, 2000);
  OCR1A = pulse_us * 2; 
}

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
  setup_gpio();
  setup_pwm_servo();
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

