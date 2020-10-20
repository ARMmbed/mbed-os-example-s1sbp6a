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
#include "rtos.h"
#include "bp6a_example.h"

volatile bool running = false;

void print_menu(void)
{
    printf("=========   Menu   =========\r\n");
    printf("   bia example  : b\r\n");
    printf("   ecg example  : e\r\n");
    printf("   ppg example  : p\r\n");
    printf("   gpio example : g\r\n");
    printf("   i2c example  : i\r\n");
    printf("   stop example : q\r\n");
    printf("============================\r\n");

}

int main(void)
{
    char c;

    printf("SIDK BP6A Example\r\n");
    ThisThread::sleep_for(100ms);

    Thread *thread = NULL;
    print_menu();

    while(1) {
        printf("Select menu >>");
        fflush(stdout);
        // flush((int)STDOUT_FILENO);
        read(STDOUT_FILENO, &c, 1);
        printf(" %c \r\n", c);
        ThisThread::sleep_for(100ms);

        if (running && c != 'q') {
            printf("You can start another example after stopping  the current example\r\n");
            continue;
        }
        switch(c) {
            case 'g' :
                running = true;
                thread = new Thread();
                thread->start(gpio_examle);
                break;
            case 'e' :
                running = true;
                thread = new Thread();

                thread->start(ecg_example);
                break;
            case 'b' :
                running = true;
                thread = new Thread();
                thread->start(bia_example);
                break;
            case 'p' :
                running = true;
                thread = new Thread();
                thread->start(ppg_example);
                break;
            case 'i' :
                running = true;
                thread = new Thread();
                thread->start(i2c_example);
                break;
            case 'q' :
                printf("stop current example!!\r\n");
                running = false;
                if (thread)
                    thread->join();
                thread = NULL;
                print_menu();
                break;
            default :
                print_menu();
                break;
        }

    }
    return 0;
}
