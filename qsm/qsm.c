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
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "qsm.h"
#include "qsm_event_handler.h"

#define QSM_QUEUE_SIZE  10

typedef struct _qsm_lookup_table_e
{
    qsm_event_t event;

    /* An array of function pointers.*/
    void (*event_handler[QSM_STATE_MAX]) ();

} _qsm_lookup_table_t;

/* EVENT HANDLERS */
#define 	HNDLR__POR       EV_HANDLER_POR
#define 	HNDLR_JOIN       EV_HANDLER_JOIN
#define 	HNDLR_TXRX       EV_HANDLER_TXRX
#define 	HNDLR_FULT       EV_HANDLER_FAULT
#define 	HNDLR_NULL       EV_HANDLER_NULL

/*
 *  EVENT, STATE and EVENT_HANDLER Look-up Table.
 *
 *  The EVENTS are the row numbers and the STATES are the
 *  column numbers. Each element on the table is the
 *  function pointer to the EVENT_HANDLER function.
 *
 */
const _qsm_lookup_table_t _qsm_lookup_table [] =
{
/*=======================================================================================================================*/
/*                               ISOLATED	       JOINED             FAULT         */
/*========================================================================================================================*/
    /*  EVENTS */
    {EVENT_POR, 		        {HNDLR__POR,       HNDLR_NULL,         HNDLR_FULT,    } },
    {EVENT_JOIN_NETWORK, 	    {HNDLR_JOIN,       HNDLR_NULL,         HNDLR_FULT,    } },
    {EVENT_TRANSMIT,            {HNDLR_NULL,       HNDLR_TXRX,         HNDLR_FULT,    } },
    {EVENT_FAULT,	            {HNDLR_FULT,       HNDLR_FULT,         HNDLR_FULT,    } },
};

typedef enum _qsm_queue_resp_e
{
    QSM_QUEUE_OK = 0,
    QSM_QUEUE_EMPTY,
    QSM_QUEUE_FULL,
} _qsm_queue_resp_t;

typedef struct _qsm_queue_s
{
    uint8_t head;
    uint8_t tail;
    uint8_t buffer[QSM_QUEUE_SIZE];
    uint8_t buffer_len;
    _qsm_queue_resp_t status;
} _qsm_queue_t;

static void _qsm_queue_init( _qsm_queue_t* b);
static void _qsm_queue_reset( _qsm_queue_t * b);
static _qsm_queue_resp_t _qsm_queue_get( _qsm_queue_t * b, uint8_t* byte_out);
static _qsm_queue_resp_t _qsm_queue_put( _qsm_queue_t * b, uint8_t data_byte);
static uint8_t _qsm_queue_count ( _qsm_queue_t const *b);
static bool _qsm_queue_isfull ( _qsm_queue_t const *b);
static bool _qsm_queue_isempty( _qsm_queue_t const *b);
static _qsm_queue_resp_t _qsm_queue_get( _qsm_queue_t* b, uint8_t* byte_out);
static _qsm_queue_resp_t _qsm_queue_put( _qsm_queue_t* b, uint8_t data_byte);
static void _qsm_queue_reset( _qsm_queue_t * b);
static void _qsm_queue_init( _qsm_queue_t* b );

static state_t _qsm_state = QSM_INITIAL_STATE;
static _qsm_queue_t _qsm_queue;

void qsm_init(void)
{
    _qsm_queue_init( &_qsm_queue );
    _qsm_queue_reset( &_qsm_queue );
    _qsm_state = QSM_INITIAL_STATE;
}

qsm_resp_t qsm_post_event( qsm_event_t event)
{
    if( QSM_QUEUE_OK != _qsm_queue_put( &_qsm_queue, event ) )
    {
        return QSM_FULL;
    }
    return QSM_OK;
}

state_t qsm_get_state( void )
{
       return _qsm_state;
}

void qsm_set_state( state_t new_state )
{
    _qsm_state = new_state;
    return;
}

/*!
 * @name  qsm_engine().
 * @brief Call this function in your main loop.
 */
void qsm_engine(void)
{
    qsm_event_t event = QSM_INITIAL_STATE;
    _qsm_queue_resp_t resp = QSM_QUEUE_OK;

    resp = _qsm_queue_get( &_qsm_queue, (uint8_t*) &event);

    switch(resp)
    {
        case QSM_QUEUE_OK:
        {
            if( NULL != _qsm_lookup_table[event].event_handler[_qsm_state] )
            {
                _qsm_lookup_table[event].event_handler[_qsm_state]();
            }
            break;
        }
        case QSM_QUEUE_EMPTY:
        {
            /* nothing to do when event queue is empty */
            break;
        }
        case QSM_QUEUE_FULL:
        default:
        {
            break;
        }
    }
}

static uint8_t _qsm_queue_count ( _qsm_queue_t const *b)
{
    return (b ? (b->head - b->tail) : 0);
}

static bool _qsm_queue_isfull ( _qsm_queue_t const *b)
{
    return (b ? (_qsm_queue_count(b) == b->buffer_len) : true);
}

static bool _qsm_queue_isempty( _qsm_queue_t const *b)
{
    return (b ? (_qsm_queue_count(b) == 0) : true);
}

static _qsm_queue_resp_t _qsm_queue_get( _qsm_queue_t* b, uint8_t* byte_out)
{
    if (!_qsm_queue_isempty(b))
    {
        *byte_out = b->buffer[b->tail % b->buffer_len];
        b->tail++;
        b->status = QSM_QUEUE_OK;
    }
    else
    {
    	b->status = QSM_QUEUE_EMPTY;
    }
    return  b->status;
}

static _qsm_queue_resp_t _qsm_queue_put( _qsm_queue_t* b, uint8_t data_byte)
{
    if (b)
    {
        if (!_qsm_queue_isfull(b))
        {
                b->buffer[b->head % b->buffer_len] = data_byte;
                b->head++;
                b->status = QSM_QUEUE_OK;
        } else
        {
                b->status = QSM_QUEUE_FULL;
        }
    }
    return b->status;
}

static void _qsm_queue_reset( _qsm_queue_t * b)
{
    if (b)
    {
        b->head = 0;
        b->tail = 0;
        b->status = QSM_QUEUE_OK;
    }
    return;
}

static void _qsm_queue_init( _qsm_queue_t* b )
{
    if (b)
    {
        b->head = 0;
        b->tail = 0;
        b->buffer_len = QSM_QUEUE_SIZE;
        b->status = QSM_QUEUE_OK;
        memset((void*) b->buffer, 0x00, QSM_QUEUE_SIZE);
    }
    return;
}
/* end of file */
