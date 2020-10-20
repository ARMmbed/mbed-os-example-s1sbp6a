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

DigitalOut  red_led(GPIO04);
DigitalOut  blue_led(GPIO05);
DigitalOut  green_led(GPIO06);
DigitalOut   buzzer(GPIO03);
InterruptIn button(GPIO07);

static int mode = 0;

void all_off(void)
{
    red_led.write(0);
    blue_led.write(0);
    red_led.write(0);
    green_led.write(0);
}

void flip()
{
    mode ++;
    all_off();
    mode %= 4;
}

void toggle(DigitalOut *out, const Kernel::Clock::duration thread_delay)
{
    int state = out->read();

    out->write(!state);

    ThisThread::sleep_for(thread_delay);
}

int gpio_examle(void)
{

    printf("start gpio_example\r\n");
    printf("if you press the Test button, LED and buzzer will be changed\r\n");

    button.fall(&flip);
    ThisThread::sleep_for(100ms);

    while (running) {
        switch (mode) {
            case 0 :
            toggle(&red_led, 500ms);
            break;
            case 1 :
            toggle(&green_led, 500ms);
            break;
            case 2 :
            toggle(&blue_led, 500ms);
            break;
            case 3 :
            toggle(&buzzer, 1ms);
            break;
        }
    }
    printf("Stop gpio_example\r\n");

    return 0;
}
