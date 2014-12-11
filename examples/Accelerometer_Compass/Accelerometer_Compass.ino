/*
 * LSM303D test demo
 *
 * Copyright (c) 2012 Seeed Technology Limited
 * Website    : www.seeed.cc
 * Author     : Jacky Zhang
 * Create Time: Dec 2014
 * Change Log :
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <Accelerometer_Compass_LSM303D.h>
#include <Wire.h>

/* Global variables */
int accel[3];  // we'll store the raw acceleration values here
int mag[3];  // raw magnetometer values stored here
float realAccel[3];  // calculated acceleration values here
float heading, titleHeading;

void setup()
{
	Serial.begin(9600);  // Serial is used for debugging
	if(Lsm303d.init() != 0)  // Initialize the LSM303, using a SCALE full-scale range
	{
		Serial.println("\r\nLSM303D is not found");
		while(1);
	}
	else
	{
		Serial.println("\r\nLSM303D is found");
	}
}

void loop()
{
	Serial.println("\r\n**************");
	//getLSM303_accel(accel);  // get the acceleration values and store them in the accel array
	Lsm303d.getAccel(accel);
	while(!Lsm303d.isMagReady());// wait for the magnetometer readings to be ready
	Lsm303d.getMag(mag);  // get the magnetometer values, store them in mag
	
	for (int i=0; i<3; i++)
	{
		realAccel[i] = accel[i] / pow(2, 15) * ACCELE_SCALE;  // calculate real acceleration values, in units of g
	}
	heading = Lsm303d.getHeading(mag);
	titleHeading = Lsm303d.getTiltHeading(mag, realAccel);
	
	printValues();
	
	delay(200);  // delay for serial readability
}

void printValues()
{  
	Serial.println("Acceleration of X,Y,Z is");
	for (int i=0; i<3; i++)
	{
		Serial.print(realAccel[i]);
		Serial.println("g");
	}
	/* print both the level, and tilt-compensated headings below to compare */
	Serial.println("The clockwise angle between the magnetic north and x-axis: ");
	Serial.print(heading, 3); // this only works if the sensor is level
	Serial.println(" degrees");
	Serial.print("The clockwise angle between the magnetic north and the projection");
	Serial.println(" of the positive x-axis in the horizontal plane: ");
	Serial.print(titleHeading, 3);  // see how awesome tilt compensation is?!
	Serial.println(" degrees");
}