/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef LOG_H
#define LOG_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>
#include <cstdlib>
#include <stdint.h>
#include <sstream>

#include "mutex.h"

#include <memory>

using namespace std;

namespace openikev2 {
    class LogImpl;

    /**
        This class allows to write log messages using several concrete implementations
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class Log {
            /****************************** ENUMS ******************************/
        public:
            /** Log message types */
            enum LOG_TYPE {
                LOG_NONE = 0x0000,       /**< Write NONE */
                LOG_INFO = 0x0001,       /**< Write informational messages */
                LOG_MESG = 0x0002,       /**< Write IKE Message contents */
                LOG_STAT = 0x0004,       /**< Write state machine transitions */
                LOG_CRYP = 0x0008,       /**< Write generated cryptographical information (keys, shared secrests, ...)*/
                LOG_THRD = 0x0010,       /**< Write thread operation messages */
                LOG_ERRO = 0x0020,       /**< Write exceptions */
                LOG_ALRM = 0x0040,       /**< Write alarm events */
                LOG_DHCP = 0x0080,       /**< Write DHCP client related events */
                LOG_IPSC = 0x0100,       /**< Write IPSEC information */
                LOG_HALF = 0x0200,       /**< Half open IKE SAs information */
                LOG_POLI = 0x0400,       /**< Policies information */
                LOG_EBUS = 0x0800,       /**< Event bus information */
                LOG_CONF = 0x1000,       /**< Configuration information */
                LOG_WARN = 0x2000,       /**< Warning information */
                LOG_ALL = 0xFFFF,        /**< Write ALL */
            };

            /****************************** ATTRIBUTES ******************************/
        protected:
            static LogImpl* implementation;             /**< Log writer implementation to be used */
            static auto_ptr<Mutex> log_mutex;           /**< Mutex_Posix to keep mutual exclusion */

            /****************************** METHODS ******************************/
        public:
            /**
             * Generates a textual representation of the LOG_TYPE enumeration values
             * @param type Log type
             * @return Textual representation
             */
            static string LOG_TYPE_STR( uint16_t type );

            /**
             * Sets a specific log writer implementation to use.
             * @param implementation Log writer implementation
             */
            static void setImplementation( LogImpl* implementation );

            /**
             * Writes a log message to the log file.
             * @param who Module writting the message
             * @param message Log message to be writed
             * @param type Type of log message (Log::LOG_INFO, D_THRD, ...)
             * @param main_info Indicates if date must be writed
             */
            static void writeMessage( string who, string message, uint16_t type, bool main_info );

            /**
             * Writes a log message to the log file, locking and unlocking the mutex.
             * @param who Module writting the message
             * @param message Log message to be writed
             * @param type Type of log message (Log::LOG_INFO, D_THRD, ...)
             * @param main_info Indicates if date must be writed
             */
            static void writeLockedMessage( string who, string message, uint16_t type, bool main_info );

            /**
             * Locks log writer, avoiding simultaneous writings of several threads
             */
            static void acquire();

            /**
             * Unlocks log writer
             */
            static void release();
    };
}

#endif
