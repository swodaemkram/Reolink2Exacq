# Reolink2Exacq.c

## Description
Reolink2Exacq is a C program that allows a Reolink camera to be used with an Exacq Vision NVR. It implements motion detection and AI (Person, Car, Animal) detection.

## Version
1.8.0b

## Copyright
Copyright 2024

## Usage
```bash
./Reolink2Exacq CameraIP UserName Password ExacqIP ExacqPort Logging AIO & disown
```
Example:
```bash
./Reolink2Exacq 10.10.10.32 admin pAsswOrd 10.10.10.19 1235 N AIO & disown
```

## Arguments
1. **CameraIP**: IP address of the Reolink camera.
2. **UserName**: Username for accessing the Reolink camera.
3. **Password**: Password for accessing the Reolink camera.
4. **ExacqIP**: IP address of the Exacq Vision NVR.
5. **ExacqPort**: Port number for communication with the Exacq Vision NVR.
6. **Instance_Name**: Name of the instance or identifier for logging purposes.
7. **Logging**: Optional flag to enable or disable logging ('Y' or 'N').
8. **AIO**: Optional flag to trigger on AI only (no false alarms on just motion).

## Example
```bash
./Reolink2Exacq 10.10.10.32 admin pAsswOrd 10.10.10.19 1235 Cam1 AIO & disown
```

## Function Documentation

### `main(int argc, char *argv[])`
Main function to handle command-line arguments and start the program.

### `validateIpAddress(const string ipAddress)`
Validates whether the provided IP address is valid.

### `CheckCameraMotionStatus(string CameraIP, string CameraUserName, string CameraPassword)`
Checks the motion status of the Reolink camera and logs it if motion is detected. Sends motion data to the Exacq server if AIOnly is not set.

### `CheckCameraTypeOfMotion(string CameraIP, string CameraUserName, string CameraPassword)`
Checks for AI detection (Person, Car, Animal) in the video stream from the Reolink camera and sends corresponding messages to the Exacq server.

### `SendDataToExacqServer(char* ExacqIP, int port, char* Message)`
Sends data to the Exacq Vision NVR server using TCP sockets.

### `WriteToLog(string log_message)`
Writes a log message with timestamp to a file in the `/var/log` directory.

## Notes
- The program uses the cURL library for HTTP requests.
- It continuously checks for motion and AI events from the Reolink camera and sends data to the Exacq server accordingly.
- Logging can be enabled or disabled by setting the `Logging` argument.
