/********************************************************************
 * Dependencies:    See INCLUDES section below
 * Processor:       PICxxxx
 * Compiler:        XCxx
 * Board:           ---
 * Author:          Sherwin Tiongson
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement:
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the "Company") for its PICmicro? Microcontroller is intended and
 * supplied to you, the Company's customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *********************************************************************
 ********************************************************************/
#include <stdio.h>
#include "qsm.h"
#include "qsm_event_handler.h"

void EV_HANDLER_POR(void)
{
    printf("EV_HANDLER_POR \n\r");
    qsm_set_state( STATE_ISOLATED );
}

void EV_HANDLER_JOIN(void)
{
    printf("EV_HANDLER_JOIN \n\r");
    qsm_set_state( STATE_JOINED );
}

void EV_HANDLER_TXRX(void)
{
    printf("EV_HANDLER_TXRX \n\r");
    qsm_set_state( STATE_JOINED );
}

void EV_HANDLER_FAULT(void)
{
    printf("EV_HANDLER_FAULT \n\r");
    qsm_set_state( STATE_FAULTY );
}

void EV_HANDLER_NULL(void)
{
     printf("EV_HANDLER_NULL \n\r");
}
/* end of file */
