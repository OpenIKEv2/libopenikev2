/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef PAYLOAD_H
#define PAYLOAD_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "printable.h"
#include "bytebuffer.h"

namespace openikev2 {

    /**
        This abstract class represents a generic Payload of a Message.
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class Payload : public Printable {

            /****************************** ENUMS ******************************/
        public:
            /** Payload types */
            enum PAYLOAD_TYPE {
                PAYLOAD_NONE = 0,        /**< Payload NONE */
                PAYLOAD_SA = 33,         /**< Security Association Payload */
                PAYLOAD_KE,              /**< Key Exchange Payload */
                PAYLOAD_IDi,             /**< Identification (initiartor) Payload */
                PAYLOAD_IDr,             /**< Identification (responder) Payload */
                PAYLOAD_CERT,            /**< Certificate Payload */
                PAYLOAD_CERT_REQ,        /**< Certificate Request Payload */
                PAYLOAD_AUTH,            /**< Authentication Payload */
                PAYLOAD_NONCE,           /**< Nonce Payload */
                PAYLOAD_NOTIFY,          /**< Notify Payload */
                PAYLOAD_DEL,             /**< Delete Payload */
                PAYLOAD_VENDOR,          /**< Vendor Payload */
                PAYLOAD_TSi,             /**< Traffic Selector (initiator) Payload */
                PAYLOAD_TSr,             /**< Traffic Selector (responder) Payload */
                PAYLOAD_SK,              /**< Encrypted Payload */
                PAYLOAD_CONF,            /**< Configuration Payload */
                PAYLOAD_EAP,             /**< Extensible Authentication Payload */
            };

            /****************************** ATTRIBUTES ******************************/
        public:
            PAYLOAD_TYPE type;      /**< Payload type.*/
            bool critical;          /**< Indicates if this payload is critical.*/

            /****************************** METHODS ******************************/
        protected:
            /**
             * Creates a new Payload
             * @param type Payload type
             * @param critical Indicates if the payloads is critical or not
             */
            Payload ( Payload::PAYLOAD_TYPE type, bool critical );

        public:
            /**
             * Returns the textual representation of a PAYLOAD_TYPE value
             * @param type Payload type
             * @return Textual representation
             */
            static string PAYLOAD_TYPE_STR( Payload::PAYLOAD_TYPE type );

            /**
             * Appends the binary representation of the Payload at the end of the buffer, without the fixed header
             * @param byte_buffer Byte buffer where append the binary representation
             */
            virtual void getBinaryRepresentation( ByteBuffer& byte_buffer ) const = 0;

            /**
             * Cets a clone of this payload
             * @return Clone of the payload.
             */
            virtual auto_ptr<Payload> clone() const = 0;

            virtual string toStringTab( uint8_t tabs ) const = 0;

            virtual ~Payload();
    };
}

#endif
