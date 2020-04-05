# Stepper motor extention board for Raspberry Pi (firmware)

[![Stepper motor extention board for Raspberry Pi Krzysztof Brzozowski](https://krzysztofbrzozowski.pl/media/2020/03/06/shield-silniki-krokowe-kontroler-akumulatora-ladowanie-USB-C.jpg)](https://krzysztofbrzozowski.pl/projekt/steronik-silnikow-krokowych-rpi-kontroler-aku-usbc/)

Simple extension board for controlling stepper motors (up to 3). Board contains Li-Po battery controller (2-4s). It is also possible to charge battery via USB C. Basic firmware you can find here [stepper_motor_extention_board_firmware.git](https://github.com/krzysztofbrzozowski/stepper_motor_extention_board_firmware.git).
Controller is using simple algorithm for controlling ttepper motors [packets table](https://krzysztofbrzozowski.pl/media/2020/04/05/packet_encoding.png).

Whole project is describerd in polish on my webpage: https://krzysztofbrzozowski.pl/projekt/steronik-silnikow-krokowych-rpi-kontroler-aku-usbc/

## Repository Contents

- **/rpi_stepper_motor_extention_board** - Coplete STM32CubeIDE project
