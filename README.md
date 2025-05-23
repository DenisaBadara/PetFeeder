# Automatic Pet Feeder

This project is an **automatic pet feeding system** designed for scheduled and manual feeding using a servo-controlled dispenser. The system is built using an ATmega microcontroller and integrates the following components:

- **LiquidCrystal_I2C** display for showing the current feeding interval and portion count.
- **DS3231 RTC module** for accurate time tracking.
- **Servo motor** controlled via Timer1 PWM (OC1A) for dispensing food.
- **Three buttons**:
  - One for increasing the number of portions per feeding.
  - One for increasing the interval between feedings (in hours).
  - One for triggering manual feeding on demand.

The feeding process is triggered:
- Automatically at the scheduled time (based on the RTC and the configured interval).
- Manually by pressing a physical button.

The system displays feeding parameters and logs useful time-related data over the serial interface. It uses hardware interrupts for efficient and responsive user input.

[![Watch the demo](https://img.youtube.com/vi/VuAz4uaTwEY/0.jpg)](https://youtu.be/VuAz4uaTwEY)

More details and project background available on [OCW - PM Project](https://ocw.cs.pub.ro/courses/pm/prj2025/ajipa/denisa.badara)
