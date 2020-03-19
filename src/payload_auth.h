/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef PAYLOAD_AUTH_H
#define PAYLOAD_AUTH_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "payload.h"
#include "payload_id.h"
#include "pseudorandomfunction.h"

namespace openikev2 {

    /**
        This class represents an Authentication Payload
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class Payload_AUTH : public Payload {

            /****************************** ATTRIBUTES ******************************/
        protected:
            auto_ptr<ByteArray> auth_field;     /**< AUTH field of the Payload_AUTH */
            Enums::AUTH_METHOD auth_method;     /**< Authentication method to use */

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new Payload_AUTH indicationg its authentication method and the auth_field
             * @param auth_method Authentication method
             * @param auth_field Authentication field already computed
             */
            Payload_AUTH( Enums::AUTH_METHOD auth_method, auto_ptr<ByteArray> auth_field );

            /**
            * Creates a new Payload_AUTH cloning other
            * @param other Other Payload_AUTH to clone
            */
            Payload_AUTH( const Payload_AUTH& other );

            /**
             * Creates a new Payload_AUTH based on its binary representation.
             * @param byte_buffer Buffer with its read pointer at the "payload length" field
             */
            static auto_ptr<Payload_AUTH> parse( ByteBuffer& byte_buffer );

            /**
             * Gets the authentication method of the Payload
             * @return Authentication method
             */
            Enums::AUTH_METHOD getAuthMethod() const;

            /**
             * Gets the auth field of the Payload
             * @return Auth field
             */
            ByteArray& getAuthField() const;

            /**
             * Compares this payload with another
             * @param other The other Payload_AUTH
             * @return TRUE if both are equals, FALSE otherwise
            */
            virtual bool operator==( const Payload_AUTH &other ) const;

            virtual void getBinaryRepresentation( ByteBuffer& byte_buffer ) const;

            virtual string toStringTab( uint8_t tabs ) const ;

            virtual auto_ptr<Payload> clone() const;

            virtual ~Payload_AUTH();
    };
}

#endif
