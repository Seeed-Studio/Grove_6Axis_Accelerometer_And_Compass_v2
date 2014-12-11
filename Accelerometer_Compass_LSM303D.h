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
 
#ifndef Accelerometer_Compass_LSM303D_H
#define Accelerometer_Compass_LSM303D_H

#include <Arduino.h>

#define ACCELE_SCALE 2  // accelerometer full-scale, should be 2, 4, or 8

#define X 0
#define Y 1
#define Z 2

class LSM303D
{
	public:

		char init();
		unsigned char read(unsigned char address);
		void write(unsigned char data, unsigned char address);
		char isMagReady();
		void getMag(int * rawValues);
		void getAccel(int * rawValues);
		float getHeading(int * magValue);
		float getTiltHeading(int * magValue, float * accelValue);
	private:
};

extern  LSM303D Lsm303d;

#endif

