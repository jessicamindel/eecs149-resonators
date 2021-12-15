# Baton control

## Setup

Baton code is located in ```buckler/software/apps/BATON```.

The buckler board communicates over UART serial via USB. When plugged in the baton code will automatically start sending data over the default port.

In order to view your ports, use: ```ls /dev/tty.*```

To set up server.py to read data being sent on the port, set the ```serialport``` variable at the top of server.py to the port that the buckler is sending data over.

Example: ```serialport = '/dev/tty.usbmodem0006825138641'```


## Orientation and signals

Hold the baton with the USB port facing you, pins/display facing to the right.

Rotate around the axis running parallel with your arm to adjust tempo.

Tilt upwards and downwards while keeping the pins/display facing to the right to adjust volume.

![diagram](./diagram.png)