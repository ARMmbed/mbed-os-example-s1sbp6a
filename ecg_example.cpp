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
#include "ecg_api.h"
#include "bp6a_example.h"

#define ECG_DATA_READY 0x01
EventFlags ecg_event_flags;


uint16_t *pECG = NULL;
uint16_t ecg_len = 0;

void ecg_cb(void *pData, uint32_t len)
{
    pECG = (uint16_t *)pData;
    ecg_len = len;
    ecg_event_flags.set(ECG_DATA_READY);
}

int ecg_example(void)
{
    printf("ECG ExampleStart\r\n");

    ThisThread::sleep_for(1000ms);


    if (ecg_init()) {
        printf("ECG init fail\r\n");
        return -1;
    }
    if (ecg_set_callback(ecg_cb) < 0) {
        printf("set callback fail\r\n");
        return -1;

    }

    if (ecg_set_decimation_rate(ECG_DECIMATION_4) < 0) {
        printf("set decimation rate fail\r\n");
        return -1;
    }


    if (ecg_start()) {
        printf("ECG start fail\r\n");
        return -1;
    }

    int cnt = 0;
    uint32_t flags;

    while (running)
    {
        flags = ecg_event_flags.wait_any(ECG_DATA_READY);
        if (flags & ECG_DATA_READY)
        for (int i = 0; i < ecg_len; i++) {
            printf("%6d\r\n", pECG[i]);
        }
    }
    ecg_stop();
    ecg_close();
    printf("stop ECG Example\r\n");
    return 0;
}
