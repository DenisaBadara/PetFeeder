#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DS3231.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
DS3231 rtc(SDA, SCL);
Time t;

#define BTN_PORTII PD2  // INT0
#define BTN_ORA    PD3  // INT1
#define BTN_MANUAL PD5  
#define SERVO_PIN  PB1  // OC1A = pin 9

volatile bool change_portions = false;
volatile bool change_interval = false;

int portions_crt = 1;
int interval = 1;
bool feed_active = false;

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
  ICR1 = 39999; 
}

void set_servo_angle(int angle) {
  uint16_t pulse_us = map(angle, 0, 180, 1000, 2000);
  OCR1A = pulse_us * 2; 
}

void setup_interrupts() {
  EICRA |= (1 << ISC01) | (1 << ISC11); 
  EIMSK |= (1 << INT0) | (1 << INT1);  
  sei();                                
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
  setup_interrupts();
  rtc.begin();
  Serial.begin(9600);

}

void feed_now() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("FEEDING");

  for (int i = 0; i < portions_crt; i++) {
    set_servo_angle(180);
    _delay_ms(2000);
    set_servo_angle(0);
    _delay_ms(1000);
  }

  lcd.clear();
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

  if (change_portions) {
    portions_crt++;
    if (portions_crt > 10) portions_crt = 1;
    change_portions = false;
    lcd.clear();
  }

  if (change_interval) {
    interval++;
    if (interval > 23) interval = 1;
    next_feed_hour = (t.hour + interval) % 24;
    change_interval = false;
    lcd.clear();
  }

  if (!(PIND & (1 << BTN_MANUAL))) {
    feed_active = true;
    _delay_ms(300); 
  }

  if (t.hour == next_feed_hour &&
      t.min == next_feed_minute &&
      t.sec == next_feed_seconds) {
    feed_active = true;
    next_feed_hour = (t.hour + interval) % 24; 
  }

  if (feed_active) {
    feed_now();
    feed_active = false;
  }

  update_lcd();
  _delay_ms(250);
}

ISR(INT0_vect) {
  change_portions = true;
}

ISR(INT1_vect) {
  change_interval = true;
}

