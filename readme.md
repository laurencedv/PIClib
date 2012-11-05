PIC C libs
==========

Those are peripherals, soft-peripherals and specefic devices C librairies for PIC18 and PIC32.
There is also definition header and macro/tool header included in the "header" folder.

PIC18
-----

Included libs for PIC18:
* ADC
* Clock control
* EUSART (software buffer included)
* Interrupt Compile-time macro
* LIN bus (not really working but a good example)
* MSSP I2C (master only)
* OneWire
* Servo Motor (10 servo with 1 timer!)
* SPI (with advance communication control)
* Timers

* Real-Time control
* Ring-Buffer
* Delay

* LCD HD44780
* DS18B20 (not complete)
* nRF24L01+ (not complete)

Keep in mind that those libs are old, and are far from optimal.

PIC32
-----

Included libs for PIC32:
* ADC
* Clock control (not complete)
* CPU (not complete)
* Interrupt (compile-time and run-time)
* Output Compare (PWM mode only)
* PPS
* SPI (with advance communication control)
* Timers (except the core timer)
* UART (with advance communication control)

* Real-Time control
* Ring-Buffer (variable element size)

* nRF24L01+ (not working)