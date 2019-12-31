# Synth Brain

## Setup

* install platformio with `pip install -U platformio`
* modify platformio.ini to specify programmer. (see here: http://docs.platformio.org/en/latest/platforms/atmelavr.html#upload-using-programmer)
    * list serial ports with `ls /dev/tty.*`

## Setup fused and burn bootloader

Todo (done by arduino IDE for now)

## Compile & Upload Program 

* to compile code run `pio run`
* connect ISP (Diamex-AVR) with SPI Interface
* to programm mcu over isp, run `pio run -t program`

## Debugging

* connect a sparkfun FTDI Basic (or similar) to gnd and tx pin (PIN3)
* setup port in platformio.ini
* run `pio device monitor`