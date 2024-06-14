# PicoHome

**PicoHome** is a home automation system built using Raspberry Pi Pico. It offers a web-based interface to control various aspects of your home, making it smarter and more secure. The system is divided into three main modules: Lights, Security, and Curtains.

## Features

- **Lights Module:** Control lights via a web interface. Turn them on/off and adjust the intensity, monitored by a photoresistor.
- **Security Module:** Enhanced home security with RFID-based entry, a keypad for alarm activation, and PIR sensor for motion detection.
- **Curtains Module:** Open and close curtains through the web interface, powered by a stepper motor.

## Modules

### 1. **Lights Module**

![Light](https://github.com/joseamorenom/PicoHome/blob/main/Hardware/Step%20Motor/light_control_unit.jpg)

This module allows you to control the lights in your home using the web interface. You can:

- Turn lights on or off.
- Adjust the light intensity, which is monitored and verified by a photoresistor.

### 2. **Security Module**

![Access](https://github.com/joseamorenom/PicoHome/blob/main/Hardware/Step%20Motor/access_control_unit.jpg)

This module ensures the security of your home with several features:

- **RFID Sensor:** Reads a correct RFID card to activate the electronic lock.
- **Keypad:** Used to activate the alarm system. Pressing the asterisk key initiates a 30-second countdown for the user to leave the house. After this, the PIR sensor is activated indefinitely.
- **PIR Sensor:** Detects movement in the house. If movement is detected, the user has 30 seconds to enter the correct code to deactivate the system. Failing to do so or entering the wrong code three times will trigger a loud alarm and send an alert to the web interface.

### 3. **Curtains Module**

![Blind](https://github.com/joseamorenom/PicoHome/blob/main/Hardware/Step%20Motor/blind_control_unit.jpg)

This module allows you to control the curtains in your home through the web interface. The curtains can be:

- Opened or closed using a stepper motor module that moves the curtain rod.

## Getting Started

To get started with PicoHome, follow these steps:

1. Clone the repository:
    ```sh
    git clone https://github.com/yourusername/picohome.git
    ```
2. Navigate to the project directory:
    ```sh
    cd picohome
    ```
3. Follow the setup instructions for each module provided in their respective directories.

## Requirements

- Raspberry Pi Pico
- RFID sensor and compatible cards
- Keypad
- PIR sensor
- Photoresistor
- Stepper motor and driver
- Web server setup

## Usage

1. Deploy the web interface on a local server.
2. Connect the hardware components as per the provided schematics.
3. Access the web interface through your browser to control the lights, security system, and curtains.

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request with your improvements.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contact

For questions or support, please open an issue in this repository.
