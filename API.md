# API

This document describes how to control and receive continuous data from the MultiTrack application.

The general pattern for your application is:

* Transmit Control Data to MultiTrack to initialise tracking
* Receive Numerical Data from MultiTrack at 30fps
* Receive Image Data from MultiTrack at 30fps
* Transmit control signals to MultiTrack to stop tracking

## Control Data

Control signals are transmitted to MultiTrack using OSC signals on UDP port 6700.

| Channel | Format | Type |
| /images/

## Numerical Data

Numerical data is received from MultiTrack using OSC on a host address and port of your choosing.

## Image Data

Image data is broadcast to the local computer using shared GPU textures via (Spout)[http://spout.zeal.co/]. Spout provides examples of how to access these shared textures from DirectX, OpenGL, openFrameworks, VVVV, MaxMSP, FFGL and others.

