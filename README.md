# Home Automation Project

This repository contains a home automation project that integrates an ESP8266 microcontroller with a Flask-based web server. The project allows remote control of GPIO pins on the ESP8266 via a web interface.

## TÜBİTAK 4006 Science Fair Participation

This project was submitted to the TÜBİTAK 4006 Science Fair competition in 2021, where it was recognized as a finalist. The project showcased integration of IoT technologies, combining an ESP8266 microcontroller with a Flask-based web server to enable remote control and monitoring of devices. Its real-time communication capabilities and user-friendly interface contributed to its success in the competition.

## Table of Contents

- [Features](#features)
- [File Structure](#file-structure)
- [Commands and Communication](#commands-and-communication)
    - [Command Format](#command-format)
    - [Communication Flow](#communication-flow)
    - [Example Response](#example-response)
    - [Error Handling](#error-handling)
    - [OTA Updates](#ota-updates)
- [License](#license)

## Features
- **ESP8266 Firmware**: Provides Over-The-Air (OTA) updates and GPIO control via TCP.
- **Flask Web Server**: Hosts a web interface to control and monitor the ESP8266.
- **Real-Time Communication**: Uses sockets for communication between the web server and ESP8266.

## File Structure

The project is organized into the following directories and files:

- **`webserver/`**: Contains the Flask-based web server code.
    - **`app.py`**: The main application file for the Flask server, which provides routes for controlling the ESP8266.
    - **`requirements.txt`**: Lists the Python dependencies required to run the Flask server.
- **`firmware/`**: Contains the ESP8266 firmware code.
    - **`OTAtcp.ino`**: The Arduino sketch for the ESP8266, enabling Over-The-Air (OTA) updates and GPIO control.
- **`.vscode/`**: Contains Visual Studio Code workspace settings.
    - **`settings.json`**: Configuration for excluding unnecessary files from the workspace.
- **`LICENSE`**: The GNU General Public License (GPL) under which this project is distributed.
- **`README.md`**: Documentation for the project.

## Commands and Communication

This project enables communication between the Flask web server and the ESP8266 microcontroller over a TCP socket. The server sends textual commands to the ESP8266, which interprets and executes them to control its GPIO pins.

### Command Format

Commands sent from the web server to the ESP8266 follow this format:

```
gpioX:action
```

- `X`: GPIO pin number (e.g., `0` or `2`)
- `action`: The operation to perform (`on`, `off`, or `show`)

#### Examples

- `gpio0:on` &mdash; Turn GPIO0 **on** (set HIGH)
- `gpio0:off` &mdash; Turn GPIO0 **off** (set LOW)
- `gpio0:show` &mdash; Query the current status of GPIO0
- `gpio2:on` &mdash; Turn GPIO2 **on**
- `gpio2:off` &mdash; Turn GPIO2 **off**
- `gpio2:show` &mdash; Query the current status of GPIO2

### Communication Flow

1. **Web Server Initiates Connection**:  
    The Flask server opens a TCP connection to the ESP8266 at its IP address and port (default: `8080`).

2. **Command Transmission**:  
    The server sends a command string (e.g., `gpio0:on\n`) over the socket.

3. **ESP8266 Processes Command**:  
    The ESP8266 parses the command, performs the requested action, and sends a response.

4. **Response Handling**:  
    The server reads the response and updates the web interface accordingly.

### Example Response

- After sending `gpio0:on`, the ESP8266 responds with:
  ```
  ok. GPIO0 = 1
  ```
- For `gpio0:show`, the response is:
  ```
  GPIO0 = 1
  ```

### Error Handling

If an invalid command is received, the ESP8266 responds with:
```
error
```

### OTA Updates

The ESP8266 firmware supports Over-The-Air (OTA) updates using the ArduinoOTA library. This allows firmware updates without physical access to the device.

## License

This project is licensed under the [GNU General Public License v3.0](./LICENSE). See the LICENSE file for details.