#######################################################
Arduino Sensor Library for ESP8266
#######################################################

This repository provides a collection of firmware and library implementations for interfacing various sensors with the **ESP8266** microcontroller. It is designed to simplify the integration of environmental and mechanical sensors into IoT and automation projects.

*******************
Supported Sensors
*******************

- **Ultrasonic Sensor (HC-SR04):** Accurate distance measurement and proximity detection.
- **Environmental Sensors (DHT11/DHT22):** Monitoring ambient temperature and humidity levels.
- **Rotary Encoders:** Precise position tracking and speed measurement for motors or user inputs.
- **Analog & Digital I/O:** General-purpose implementations for various modular sensors.

**************************
Technical Specifications
**************************

- **Microcontroller:** ESP8266 (NodeMCU, WeMos D1 Mini, etc.)
- **Language:** C++ (Arduino Framework)
- **Protocols:** GPIO, I2C, OneWire (depending on sensor)
- **IDE Support:** Arduino IDE, PlatformIO, or VS Code

*******************
Installation Guide
*******************

1. **Clone the Project**
   .. code-block:: bash

      git clone https://github.com/afafirmansyah/arduino-sensor-library.git

2. **Library Dependencies**
   - Open the **Library Manager** in your Arduino IDE.
   - Install required libraries for specific sensors (e.g., ``DHT sensor library`` by Adafruit).

3. **Circuit Connection**
   - Connect the sensors to the designated GPIO pins as defined in the source code headers.
   - Ensure proper power supply (3.3V for ESP8266, check if sensors require 5V).

4. **Upload and Monitor**
   - Select your ESP8266 board in the IDE.
   - Compile and upload the sketch.
   - Open the **Serial Monitor** (usually at 115200 baud) to view the sensor data.



*******
License
*******

This project is licensed under the MIT License - see the `license.txt` file for details.

*********
Contact
*********

**Ahmad Fauzi Firmansyah**
- **GitHub:** `afafirmansyah <https://github.com/afafirmansyah>`_
- **LinkedIn:** `ahmad-fauzi-firmansyah <https://linkedin.com/in/ahmad-fauzi-firmansyah/>`_
