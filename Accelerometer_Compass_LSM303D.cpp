/*
 * A library for LSM303D based Acc&Compass Grove
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

/* LSM303 Address definitions */
#define LSM303D_ADDR  0x1E  // assuming SA0 grounded

/* LSM303 Register definitions */
#define TEMP_OUT_L		0x05
#define TEMP_OUT_H		0x06
#define	STATUS_REG_M	0x07
#define OUT_X_L_M 		0x08
#define OUT_X_H_M 		0x09
#define OUT_Y_L_M 		0x0A
#define OUT_Y_H_M 		0x0B
#define OUT_Z_L_M 		0x0C
#define OUT_Z_H_M 		0x0D
#define	WHO_AM_I		0x0F
#define	INT_CTRL_M		0x12
#define	INT_SRC_M		0x13
#define	INT_THS_L_M		0x14
#define	INT_THS_H_M		0x15
#define	OFFSET_X_L_M	0x16
#define	OFFSET_X_H_M	0x17
#define	OFFSET_Y_L_M	0x18
#define	OFFSET_Y_H_M	0x19
#define	OFFSET_Z_L_M	0x1A
#define	OFFSET_Z_H_M	0x1B
#define REFERENCE_X 	0x1C
#define REFERENCE_Y 	0x1D
#define REFERENCE_Z 	0x1E
#define CTRL_REG0 		0x1F
#define CTRL_REG1 		0x20
#define CTRL_REG2		0x21
#define CTRL_REG3 		0x22
#define CTRL_REG4 		0x23
#define CTRL_REG5 		0x24
#define CTRL_REG6 		0x25
#define CTRL_REG7 		0x26
#define	STATUS_REG_A	0x27
#define OUT_X_L_A 		0x28
#define OUT_X_H_A 		0x29
#define OUT_Y_L_A 		0x2A
#define OUT_Y_H_A 		0x2B
#define OUT_Z_L_A 		0x2C
#define OUT_Z_H_A 		0x2D
#define	FIFO_CTRL		0x2E
#define	FIFO_SRC		0x2F
#define	IG_CFG1			0x30
#define	IG_SRC1			0x31
#define	IG_THS1			0x32
#define	IG_DUR1			0x33
#define	IG_CFG2			0x34
#define	IG_SRC2			0x35
#define	IG_THS2			0x36
#define	IG_DUR2			0x37
#define	CLICK_CFG		0x38
#define	CLICK_SRC		0x39
#define	CLICK_THS		0x3A
#define	TIME_LIMIT		0x3B
#define	TIME_LATENCY	0x3C
#define	TIME_WINDOW		0x3D
#define	ACT_THS			0x3E
#define	ACT_DUR			0x3F

#define MAG_SCALE_2 	0x00//full-scale is +/-2Gauss
#define MAG_SCALE_4 	0x20//+/-4Gauss
#define MAG_SCALE_8 	0x40//+/-8Gauss
#define MAG_SCALE_12 	0x60//+/-12Gauss
 
char LSM303D::init()
{
	char rtn = -1;
	
	Wire.begin();  // Start up I2C, required for LSM303 communication
	if(read(WHO_AM_I) != 0x49) return rtn; // return wrong if no LSM303D was found 
	write(0x57, CTRL_REG1);  // 0x57 = ODR=50hz, all accel axes on
	write((3<<6)|(0<<3), CTRL_REG2);  // set full-scale
	write(0x00, CTRL_REG3);  // no interrupt
	write(0x00, CTRL_REG4);  // no interrupt
	write((4<<2), CTRL_REG5);  // 0x10 = mag 50Hz output rate
	write(MAG_SCALE_2, CTRL_REG6); //magnetic scale = +/-1.3Gauss
	write(0x00, CTRL_REG7);  // 0x00 = continouous conversion mode
	rtn = 0;
	return rtn;
}

unsigned char LSM303D::read(unsigned char address)
{
	char temp;

	Wire.beginTransmission(LSM303D_ADDR);
	Wire.write(address);
	Wire.endTransmission();

	Wire.requestFrom(LSM303D_ADDR, 1);
	while(!Wire.available());
	temp = Wire.read();
	Wire.endTransmission();

	return temp;
}

void LSM303D::write(unsigned char data, unsigned char address)
{
	Wire.beginTransmission(LSM303D_ADDR); 
	Wire.write(address);
	Wire.write(data);
	Wire.endTransmission();
}

char LSM303D::isMagReady()
{
	char temp;
	temp = read(STATUS_REG_M) & 0x03;
	return temp;
}

void LSM303D::getMag(int * rawValues)
{
	rawValues[X] = ((int)read(OUT_X_H_M) << 8) | (read(OUT_X_L_M));
	rawValues[Y] = ((int)read(OUT_Y_H_M) << 8) | (read(OUT_Y_L_M));
	rawValues[Z] = ((int)read(OUT_Z_H_M) << 8) | (read(OUT_Z_L_M));
}

void LSM303D::getAccel(int * rawValues)
{
	rawValues[X] = ((int)read(OUT_X_H_A) << 8) | (read(OUT_X_L_A));
	rawValues[Y] = ((int)read(OUT_Y_H_A) << 8) | (read(OUT_Y_L_A));
	rawValues[Z] = ((int)read(OUT_Z_H_A) << 8) | (read(OUT_Z_L_A));
}

float LSM303D::getHeading(int * magValue)
{
	// see section 1.2 in app note AN3192
	float heading = 180*atan2(magValue[Y], magValue[X])/PI;  // assume pitch, roll are 0

	if (heading <0)
	heading += 360;

	return heading;
}

float LSM303D::getTiltHeading(int * magValue, float * accelValue)
{
	// see appendix A in app note AN3192 
	float pitch = asin(-accelValue[X]);
	float roll = asin(accelValue[Y]/cos(pitch));

	float xh = magValue[X] * cos(pitch) + magValue[Z] * sin(pitch);
	float yh = magValue[X] * sin(roll) * sin(pitch) + magValue[Y] * cos(roll) - magValue[Z] * sin(roll) * cos(pitch);
	float zh = -magValue[X] * cos(roll) * sin(pitch) + magValue[Y] * sin(roll) + magValue[Z] * cos(roll) * cos(pitch);
	float heading = 180 * atan2(yh, xh)/PI;

	if (yh >= 0)
		return heading;
	else
		return (360 + heading);
}

LSM303D Lsm303d;

