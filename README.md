# Distance-measurement-using-HC-SR04-and-GY-521-accelerometer-display-on-OLED

The HC-SR04 ultrasonic sensor uses sonar to determine distance to an object. This sensor provides 2cm to 400cm of non-contact measurement functionality with high accuracy and stable readings. This module includes an ultrasonic transmitter, a receiver and a control unit. It has 4 pins, Ground, VCC, Trig and Echo.
The MPU-6050 sensor contains a MEMS 3-axis accelerometer and a MEMS 3-axis gyro in a single chip. It is very accurate, as it contains 16-bits analog to digital conversion hardware for each channel. Therefore, it captures the x, y and z channels at the same time. The sensor uses the I2C-bus to interface with the Arduino. The accelerometer and the gyro values are called the raw values. It is used to detect angle of tilt or inclination along the x, y and z axes as shown in below figure.
![image](https://user-images.githubusercontent.com/49164733/116715122-afa73b00-a9d6-11eb-9ea1-5bd91104a136.png)

Materials required:
• Arduino UNO development kit
• Lab deck
• GY-521 Accelerometer and Gyro
• HC-SR04 Ultrasonic Distance Sensor
• OLED 1.3 I2C, Display
• Cables
• AVRISPII programmer
HC-SR04:
In order to generate the ultrasound, we need to set the Trig on a High State for 10μs. Then it will send out an 8-cycle sonic burst which travel at the speed sound and it will be received in the Echo pin. The Echo pin will output the time in microseconds the sound wave travelled.
![image](https://user-images.githubusercontent.com/49164733/116715198-c51c6500-a9d6-11eb-8bba-253e11ecbeea.png)
GY-521:
The sensor values are obtained by using the I2C serial data bus, which requires only two wires (SCL and SDA). First, we have to set up the I2C connection between the module and the Arduino.

The following figure is the experimental setup to display the accelerometer and the ultrasonic sensors on the OLED screen.
![image](https://user-images.githubusercontent.com/49164733/116715338-e5e4ba80-a9d6-11eb-9829-241619678fb0.png)
An object was placed at a certain distance away from the ultrasonic sensor to display the distance in cm on the OLED screen. The results shown in this report had an object placed at 9 cm and 5 cm. When there is no object to be detected, the screen shows “Clear!!”. Following is the code for calculating distance. Whenever echo is received, the counter in the timer1 overflow interrupt is incremented, and is multiplied with the maximum time to obtain distance. And the obtained distance is divided with CYCLES_PER_US which is the number of instructions per second.
no_of_ticks = ((overFlowCounter*TIMER_MAX)+TCNT1); // counter count dist_in_cm = (no_of_ticks/(CONVERT_TO_CM*CYCLES_PER_US)); // distance in cm
In order to show the working of the accelerometer, it was moved in different directions to observer the results on the screen. The breadboard was tilted towards the X-direction as shown in Figure 6, and Y-direction as shown in Figure 7. The values that we get on the 3-axis is called the raw data. We can convert the raw accelerometer data from into multiples of g (9.8 m/s^2) by dividing by a sensitivity scale factor of 16384.

--------------------------Results-----------------------------------
![image](https://user-images.githubusercontent.com/49164733/116715534-188eb300-a9d7-11eb-95d6-a6657d164b85.png)
![image](https://user-images.githubusercontent.com/49164733/116715561-1fb5c100-a9d7-11eb-84fd-dec1dbc481b0.png)
![image](https://user-images.githubusercontent.com/49164733/116715615-2a705600-a9d7-11eb-98c9-209b17bb7d06.png)
![image](https://user-images.githubusercontent.com/49164733/116715641-30663700-a9d7-11eb-8db7-b2b7f10238c0.png)
![image](https://user-images.githubusercontent.com/49164733/116715701-3b20cc00-a9d7-11eb-90b0-2b9757634e5e.png)
![image](https://user-images.githubusercontent.com/49164733/116715745-4542ca80-a9d7-11eb-9f2d-6004564a22a2.png)

As we have seen in Figure 3 and Figure 4, the ultrasonic sensor gives the accurate distance from the object. When there is an object in the range of 2 cm, the values fluctuate because the sensor only provides 2 cm to 400 cm non-contact measurement function. The distance sensor works by calculating and storing the echo time within the sonar range.
The values that we get on the 3-axis is called the raw data. We can convert the raw accelerometer data from into multiples of g (9.8 m/s^2) by dividing by a sensitivity scale factor of 16384.

The HC-SR04 ultrasonic gives precise details and comes with high accuracy. It comes with a wide range of applications mainly targeting distance and direction measurements.
The accelerometer is very sensitive that even a very little force will make the measurements fluctuate. It is important to note that the accelerometer results provide accurate orientation angles as long as gravity is the only force acting on the sensor. However, when moving and rotating the sensor, we are applying forces to it, which causes the measurements to fluctuate. The net result is that accelerometer data tends to be very sensitive, with brief but significant small changes in the movement. If these can be averaged out, the accelerometer provides accurate results over timescales.


