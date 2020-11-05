#include "mbed.h"
#include "bia_api.h"
#include "bp6a_example.h"

#define BIA_DATA_READY  0x01

EventFlags bia_event_flags;

uint16_t *p_bia_data = NULL;
uint32_t bia_data_len = 0;


void bia_cb(void *pData, uint32_t len)
{
    p_bia_data = (uint16_t *)pData;
    bia_data_len = len;
    bia_event_flags.set(BIA_DATA_READY);
}

int bia_example(void)
{
    uint32_t imp;

    printf("BIA ExampleStart\r\n");

    ThisThread::sleep_for(1000ms);

    if (bia_init() < 0) {
        printf("bia_init fail\r\n");
    }

    bia_conf(50, 0x20);

    bia_calibration();
    bia_set_callback(bia_cb);
    ThisThread::sleep_for(200ms);

    bia_start();

    while(running) {
        bia_event_flags.wait_any(BIA_DATA_READY);
        imp = bia_cal_impedance(p_bia_data, bia_data_len);
        printf("%10ld\r\n", imp);
    }

    bia_stop();
    bia_close();
    printf("BIA stop\r\n");

    return 0;

}
