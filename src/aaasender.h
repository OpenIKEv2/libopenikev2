/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef AAASENDER_H
#define AAASENDER_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "eappacket.h"
#include "bytearray.h"
#include "semaphore.h"


namespace openikev2 {

    /**
        This abstract class represents objects that want to receive AAA responses.
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class AAASender {
        /****************************** ATTRIBUTES ******************************/
        public:
            string     aaa_username;
            string     aaa_server_addr;
            string     aaa_server_secret;
            uint8_t    aaa_server_port;
            Semaphore *aaa_semaphore;
            auto_ptr<ByteArray> aaa_msk;
            auto_ptr<EapPacket> aaa_eap_packet_to_send;
            auto_ptr<EapPacket> aaa_eap_packet_received;
            /****************************** METHODS ******************************/
        public:
            /**
             * This method is called when AAA controller receives the response.
             */
            virtual void AAA_receive( auto_ptr<EapPacket> eap_packet ) = 0;

            virtual ~AAASender();
    };
}
#endif

