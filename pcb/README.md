# PIN out

## 4 Socket PINs (J1)

1 - GND
2 - 3V3
3 - GPIO 3 / RXD
4 - GPIO 1 / TXD

## 2 Socket PINs

1, 2 GPIO0 - Boot up PINS (J2)

# Wiring ConnectionsUSB-to-UART 

 - USB TX - Connect to GPIO3 (RXD) on the ESP 
 - USB RX - Connect to GPIO1 (TXD) on the ESP
 - GND - Connect adapter GND to ESP GND

Enter into flashing mode
 - GPIO0 - Connect to GND, just connect 1 with 2 togeter via jumper

# Flashing
 - Enter flashing mode (blue LED should flash once)
 - In vs code on the bottom select esp12e, ttyUSB0
 - Click PlatformIO upload

At this point you should see percentage progress and LED should flash in some intervals

## esptool flashing - alternative (didn't test)

 - Install esptool in python env
 - When you build project, firmware can be found in:
```
blinker/.pio/build/esp12e/firmware.bin
```
 - Flash
```
esptool.py --port COM3 write_flash 0x00000 firmware.bin
```

For ESP32

```
esptool.py --port COM3 --chip esp32 write_flash 0x10000 firmware.bin
```
