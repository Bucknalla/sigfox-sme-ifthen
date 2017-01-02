<p align="center"><img src ="http://www.sigfox.com/themes/custom/sigfox/images/logo-2016.svg" width="300"></p>

## Sigfox Smart Everything - If Then Tutorial

This tutorial uses a Smart Everything Fox Dev Kit (SME) to show you how to use Sigfox with a 'If... Then...' use case. In this particular example, the SME will check values from its sensors for Temperature and Humidity and only send a message if they exceed a certain user-defined value.

### Prerequisites

##### Hardware

* Smart Everything Development Kit (Fox or Tutto)
* 868 Mhz Antenna (Included with SME)
* Micro USB Cable

##### Software

* Arduino IDE (Windows, MacOS or Linux)

### Introduction

To get started make sure you check out the [getting stared doc](http://docs-europe.electrocomponents.com/webdocs/144b/0900766b8144b09d.pdf) for the SME to set up the Arduino IDE and installing the correct libraries.

To configure this example for your own projects/prototypes, change the 'TEMP' and 'HUM' target values to that of your own preference. You may also change 'DEBUG' to 1 to enable USBSerial output.

### Future Improvements

Currently this example doesn't support any power saving features that you might employ on a typical battery powered projects.

* Power Saving / Deep Sleep Modes
* Implement callbacks to define the user specified 'Targets'
* Design a 3D printed enclosure for the device

If you want to try and help out with improvement, please fork this repo and make a pull request!
