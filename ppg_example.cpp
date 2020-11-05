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
#include "ppg_api.h"
#include "bp6a_example.h"

#define PPG_CH0_DATA_READY  0x03
#define PPG_CH1_DATA_READY  0x04
#define PPG_CH_DATA_READY   0x03

DigitalOut  ch0_led(GPIO04);
DigitalOut  ch1_led(GPIO05);

EventFlags ppg_event_flags;

volatile unsigned char data_ready = 0;
bool r = false;
bool b = false;


void ppg_ch0_cb(void)
{

    ch0_led.write(r);
    r = !r;

    ppg_event_flags.set(PPG_CH0_DATA_READY);
}

void ppg_ch1_cb(void)
{
    ppg_event_flags.set(PPG_CH1_DATA_READY);

    ch1_led.write(b);
    b = !b;
}

int ppg_example(void)
{

    uint8_t drv_cur[4] = {0x1F, 0x3F, 0x1F, 0x1F};
    printf("PPG ExampleStart\r\n");

    ThisThread::sleep_for(1000ms);

    if (ppg_init(10, 2))
        printf("PPG init fail\r\n");


    ppg_conf_ch(0, true, false, true, false);
    ppg_conf_ch(1, false, true, false, false);
    ppg_set_led_drv(100, drv_cur);
    ppg_set_cb(ppg_ch0_cb, ppg_ch1_cb);
    ppg_start();

    uint32_t ppg_ready = 0;
    uint32_t *ppg_data;
    uint32_t len;
    uint32_t i;

    while(running) {

        ppg_ready = ppg_event_flags.wait_any(PPG_CH0_DATA_READY | PPG_CH1_DATA_READY);
        if (ppg_ready & PPG_CH0_DATA_READY) {
            ppg_data = ppg_get_data(0, 2, &len);
            for(i = 0; i < len; i++)
                printf("%10d\n", ppg_data[i]);
        }
        if (ppg_ready & PPG_CH1_DATA_READY) {
            // printf("Channel 1 Ready \r\n");
        }

    }
    ppg_stop();
    ppg_close();

    return 0;
}
