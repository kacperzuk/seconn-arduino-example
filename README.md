seconn-arduino-example
===========

seconn-arduino-example is part of SeConn project. It's a protocol and set of libraries for secure communication. This repository contains example of using SeConn with Arduino. See also other repositories:

* [seconn](https://github.com/kacperzuk/seconn) - description of design and protocol, you should read it.
* [seconn-avr](https://github.com/kacperzuk/seconn-avr) - AVR library that implements the SeConn protocol
* [seconn-java](https://github.com/kacperzuk/seconn-java) - Java library that implements the SeConn protocol
* [seconn-android-example](https://github.com/kacperzuk/seconn-android-example) - Example Android project that uses seconn-java
* [seconn-arduino-example](https://github.com/kacperzuk/seconn-arduino-example) - Example Arduino sketch that uses seconn-avr

Usage
=====

You need XM-15B Bluetooth module for this example attached to pins 8 (Rx) and 9 (Tx). If you want to use another module, you'll have to customize `btinit()` function in `seconn-example.ino`.

Copy libraries/ and seconn-example/ directories to ~/Arduino. Go to ~/Arduino/seconn-example and run `arduino seconn-example.ino`.

This sketch will bring up Bluetooth with name `seconn`. It'll then wait until something connects and send HelloRequest frame. That will initiate setting up secure connection. All state changes and data from EncryptedData frames will be sent through Serial.

![Image of example terminal output](http://i.imgur.com/PlVBXfD.png)
