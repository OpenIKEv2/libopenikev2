/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
 ***************************************************************************/
#ifndef OPENIKEV2CHILDSAREQUEST_H
#define OPENIKEV2CHILDSAREQUEST_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "printable.h"
#include "attributemap.h"
#include "payload_tsi.h"
#include "payload_tsr.h"

namespace openikev2 {

    /**
    This class represents a CHILD_SA request. It contains the needed parameters to become a ChildSa class.
     @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class ChildSaRequest : public Printable {
        public:
            auto_ptr<Payload_TS> my_traffic_selector;       /**< My traffic selector for this ChildSa */
            auto_ptr<Payload_TS> peer_traffic_selector;     /**< Peer traffic selector for this ChildSa */
            Enums::IPSEC_MODE mode;                         /**< IPsec mode of the ChildSa */
            Enums::PROTOCOL_ID ipsec_protocol;              /**< IPsec protocol ID */

        public:
            /**
             * Creates a new ChildSaRequest object
             * @param ipsec_protocol IPsec protocol
             * @param mode IPsec mode
             * @param my_traffic_selector Source traffic selector
             * @param peer_traffic_selector Destination traffic selector
             */
            ChildSaRequest(Enums::PROTOCOL_ID ipsec_protocol, Enums::IPSEC_MODE mode, auto_ptr<Payload_TS> my_traffic_selector, auto_ptr<Payload_TS> peer_traffic_selector);

            /**
             * Creates a new ChildSaRequest cloning another one
             * @param other Other ChildSaRequest to be cloned
             */
            ChildSaRequest(const ChildSaRequest& other);

            /**
             * Clones the ChildSaRequest object
             * @return A clone of this object
             */
            virtual auto_ptr<ChildSaRequest> clone();

            virtual string toStringTab( uint8_t tabs ) const;

            virtual ~ChildSaRequest();

    };

}

#endif
