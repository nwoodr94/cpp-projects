### Flight Computer

The [Scientific Payload](https://github.com/nwoodr94/cpp-projects/blob/master/scientific-payload.cpp) project is a script I wrote for the University of Washington's Rockets and Instrumentation Laboratory. This script ran on an Arduino MEGA that I wired up to read data from sensors in a field test environment.

The computer reads the following physical properties of a typical spaceflight trajectory, and writes data to an SD card.

1. Acceleration (x,y,z)
2. Gyroscope (roll,pitch,yaw)
3. Magnetometer (x,y,z)
5. Pressure
6. Temperature
7. Altitude
8. Time


The [Space-Daemon](https://github.com/nwoodr94/cpp-projects/blob/master/space-daemon.png) was a multiple stage sounding rocket designed to carry this payload through Mach 2 and to an altitude of 21 km.
