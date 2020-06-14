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

#ifndef QSM_CONFIG_H
#define	QSM_CONFIG_H

#ifdef	__cplusplus
extern "C" {

#endif

typedef enum state_e
{
    STATE_ISOLATED = 0,

    STATE_JOINED,

    STATE_FAULTY,

    /* DO NOT RENAME THIS STATE!! */
    QSM_STATE_MAX

} state_t;

typedef enum huvent_e
{
    EVENT_POR = 0,

    EVENT_JOIN_NETWORK,

    EVENT_TRANSMIT,

    EVENT_FAULT,

    /* DO NOT RENAME THIS EVENT!! */
    QSM_EVENT_MAX
} qsm_event_t;

#define     QSM_INITIAL_STATE           STATE_ISOLATED

#ifdef	__cplusplus
}
#endif

#endif	/* QSM_CONFIG_H */

