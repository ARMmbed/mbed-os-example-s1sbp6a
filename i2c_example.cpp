/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
#include "mbed.h"
#include "bp6a_example.h"

#define MPU9250_ADDRESS 0x68
#define MAG_ADDRESS 0x0C

#define MPU6050_GYRO_CONFIG			0x1B
#define MPU6050_ACCEL_CONFIG		0x1C

#define MPU6050_ACCEL_XOUT_H		0x3B
#define MPU6050_ACCEL_XOUT_L		0x3C
#define MPU6050_ACCEL_YOUT_H		0x3D
#define MPU6050_ACCEL_YOUT_L		0x3E
#define MPU6050_ACCEL_ZOUT_H		0x3F
#define MPU6050_ACCEL_ZOUT_L		0x40
#define MPU6050_TEMP_OUT_H			0x41
#define MPU6050_TEMP_OUT_L			0x42
#define MPU6050_GYRO_XOUT_H			0x43
#define MPU6050_GYRO_XOUT_L			0x44
#define MPU6050_GYRO_YOUT_H			0x45
#define MPU6050_GYRO_YOUT_L			0x46
#define MPU6050_GYRO_ZOUT_H			0x47
#define MPU6050_GYRO_ZOUT_L			0x48

#define MPU6050_PWR_MGMT_1			0x6B
#define MPU6050_PWR_MGMT_2			0x6C

#define MPU6050_WHO_AM_I			0x75

I2C *imu;

void i2c_write_reg(uint8_t addr, uint8_t reg, uint8_t data)
{
    char buf[2];

    buf[0] = reg;
    buf[1] = data;
    imu->write(MPU9250_ADDRESS, (char *)buf, 2, false);
}

void i2c_read_reg(uint8_t addr, uint8_t reg, uint8_t *data, int len)
{
    imu->write(MPU9250_ADDRESS, (char *)&reg, 1, false);
    imu->read(MPU9250_ADDRESS, (char *)data, len, false);
}

int i2c_example(void)
{
    printf("i2c IMU example\r\n");

    ThisThread::sleep_for(1000ms);


    imu = new I2C(I2C2_SDA, I2C2_SCL);

    ThisThread::sleep_for(500ms);

    uint8_t Buf[15] = {0,};
    // Setup MPU6050 Power Management
    i2c_write_reg(MPU9250_ADDRESS, MPU6050_PWR_MGMT_1, 0);

    ThisThread::sleep_for(500ms);

    i2c_write_reg(MPU9250_ADDRESS, MPU6050_PWR_MGMT_2, 0);
    Buf[0] = 0;
    // Read WHO_AM_I Register
    i2c_read_reg(MPU9250_ADDRESS, MPU6050_WHO_AM_I, Buf, 1);

    printf("Who am I : 0x%02x\r\n", Buf[0]);

    ThisThread::sleep_for(500ms);


    // Read ACCEL_CONFIG Register
    i2c_read_reg(MPU9250_ADDRESS, MPU6050_ACCEL_CONFIG, Buf, 1);
    printf("Accel Config : 0x%02x\r\n", Buf[0]);

    // Read GYRO_CONFIG Register
    i2c_read_reg(MPU9250_ADDRESS, MPU6050_GYRO_CONFIG, Buf, 1);

    printf("Gyro Config : 0x%02x\r\n", Buf[0]);


    while (running) {
        i2c_read_reg(MPU9250_ADDRESS, MPU6050_ACCEL_XOUT_H, Buf, 14);

        // Accelerometer
        int16_t ax=-(Buf[0] << 8 | Buf[1]);
        int16_t ay=-(Buf[2] << 8 | Buf[3]);
        int16_t az=Buf[4] << 8 | Buf[5];

        // Gyroscope
        int16_t gx=-(Buf[8] << 8 | Buf[9]);
        int16_t gy=-(Buf[10] << 8 | Buf[11]);
        int16_t gz=Buf[12] << 8 | Buf[13];

        printf("Accelerometer : %d %d %d\r\n", ax, ay, az);
        printf("Gyroscope : %d %d %d\r\n", gx, gy, gz);

        ThisThread::sleep_for(1000ms);

    }
    printf("i2c example stop\r\n");

    return 0;

}
