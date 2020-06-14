#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "qsm.h"

int main()
{
    printf("start of main\n\r");
    qsm_init();

    // test by posting events to queue
    qsm_post_event(EVENT_POR);
    qsm_post_event(EVENT_JOIN_NETWORK);
    qsm_post_event(EVENT_TRANSMIT);
    qsm_post_event(EVENT_TRANSMIT);
    qsm_post_event(EVENT_TRANSMIT);
    qsm_post_event(EVENT_TRANSMIT);
    qsm_post_event(EVENT_TRANSMIT);
    qsm_post_event(EVENT_TRANSMIT);
    qsm_post_event(EVENT_TRANSMIT);
    qsm_post_event(EVENT_FAULT);
    qsm_post_event(EVENT_FAULT);

    while(true)
    {
       qsm_engine();
    }

    return 0;
}
