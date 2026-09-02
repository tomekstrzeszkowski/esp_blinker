
![Image](blinker/blinker.png)

# PIN out

## 4 Socket PINs (J1)

1 - GND
2 - 3V3
3 - GPIO 3 / RXD
4 - GPIO 1 / TXD

## 2 Socket PINs

1, 2 GPIO0 - Boot up PINS (J2), connect for entering flashing mode

# Wiring ConnectionsUSB-to-UART 

 - USB TX - Connect to GPIO3 (RXD) on the ESP 
 - USB RX - Connect to GPIO1 (TXD) on the ESP
 - GND - Connect adapter GND to ESP GND

Enter into flashing mode
 - GPIO0 - Connect to GND, just connect 1 with 2 togeter via jumper

# Flashing
 - Connect UART to USB converter to J1, don't connect to USB yet
 - Enter flashing mode, connect 2 PINs together using jumper in J2 socket
 - Connect converter to USB, The blue LED should flicker once
 - Follow your preferred flashing IDE

## VS Code and PlatformIO

 - On the bottom bar, select esp12e, ttyUSB0
 - Click PlatformIO upload

At this point you should see percentage progress and LED should flash in some intervals

## esptool and PlatformIO

 - Install requirements.txt, this should install esptool and PlatformIO
 - PlatformIO builds firmware in this location
```
blinker/.pio/build/esp12e/firmware.bin
```
 - Build `firmware.bin` (list available targets `pio run --list-targets`)

```
pio run -e esp12e
```
This should produce firmware in the location as above.

 - Flash
```
esptool --port /dev/ttyUSB0 write-flash 0x00000 blinker/.pio/build/esp12e/firmware.bin
```

For ESP32

```
esptool --port /dev/ttyUSB0 --chip esp32 write-flash 0x10000 firmware.bin
```
