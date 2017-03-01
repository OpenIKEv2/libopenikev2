/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj.fernandez@dif.um.es                 *
*   Alejandro Perez Mendez     alejandro_perez@dif.um.es                  *
*                                                                         *
*   This library is free software; you can redistribute it and/or         *
*   modify it under the terms of the GNU Lesser General Public            *
*   License as published by the Free Software Foundation; either          *
*   version 2.1 of the License, or (at your option) any later version.    *
*                                                                         *
*   This library is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
*   Lesser General Public License for more details.                       *
*                                                                         *
*   You should have received a copy of the GNU Lesser General Public      *
*   License along with this library; if not, write to the Free Software   *
*   Foundation, Inc., 51 Franklin St, Fifth Floor,                        *
*   Boston, MA  02110-1301  USA                                           *
***************************************************************************/
#ifndef IPSECCONTROLLERIMPL_H
#define IPSECCONTROLLERIMPL_H

#include "ipaddress.h"
#include "payload_tsi.h"
#include "payload_tsr.h"
#include "enums.h"
#include "proposal.h"
#include "childsa.h"
#include "payload_sa.h"
#include "ikesa.h"


namespace openikev2 {

    /**
        This abstract class represents a IPsec controller concrete implementation.
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alejandro_perez@dif.um.es, pedroj.fernandez@dif.um.es>
    */
    class IpsecControllerImpl {

            /****************************** ATTRIBUTES ******************************/
        public:
            string name;                    /**< Name of the IpsecController implementation */

            /****************************** METHODS ******************************/
        public:
            /**
             * Determines if received TS Payloads are acceptable, making narrowing if needed (executed in the responder)
             * @param received_payload_ts_i Received PAYLOAD_TSi
             * @param received_payload_ts_r Reveived PAYLOAD_TSr
             * @param ike_sa IKE_SA
             * @param child_sa ChildSa object to be updated
             * @return TRUE if narrowing has been possible. FALSE otherwise.
             */
            virtual bool narrowPayloadTS( const Payload_TSi & received_payload_ts_i, const Payload_TSr & received_payload_ts_r, IkeSa& ike_sa, ChildSa & child_sa ) = 0;

            /**
             * Checks if responder narrowing is valid (executed in the initiator)
             * @param received_payload_ts_i Received PAYLOAD_TSi
             * @param received_payload_ts_r Received PAYLOLAD_TSr
             * @param child_sa ChildSa beign created
             * @return TRUE if responder narrowing is valid. FALSE otherwise.
             */
            virtual bool checkNarrowPayloadTS( const Payload_TSi & received_payload_ts_i, const Payload_TSr & received_payload_ts_r, ChildSa & child_sa ) = 0;

            /**
             * Request an SPI value
             * @return The SPI value
             */
            virtual uint32_t getSpi( const IpAddress& src, const IpAddress& dst, Enums::PROTOCOL_ID protocol ) = 0;

            /**
             * Creates an IPSEC SA
             * @param src Source address of the IPSEC SA
             * @param dst Destionation address of the IPSEC SA
             * @param childsa ChildSa object with the values of the IPSEC SA
             */
            virtual void createIpsecSa( const IpAddress& src, const IpAddress& dst, const ChildSa& childsa ) = 0;

            /**
             * Deletes an IPSEC SA.
             * @param src Source address of the IPSEC SA
             * @param dst Destionation address of the IPSEC SA
             * @param protocol IPsec protocol of the IPSEC SA
             * @param spi SPI value of the IPSEC SA
             * @return The SPI value of the updated IPSEC SA. 0 if an error was found
             */
            virtual uint32_t deleteIpsecSa( const IpAddress& src, const IpAddress& dst, Enums::PROTOCOL_ID protocol, uint32_t spi ) = 0;

            /**
             * Creates an IPSEC policy indicating all its parameters
             * @param src_sel Source selector collection
             * @param dst_sel Destination selector collection
             * @param direction Direction of the policy
             * @param priority Policy priority. This value is dependet of the concrete implementation.
             * @param ipsec_protocol IPsec protocol
             * @param mode IPsec mode
             * @param src_tunnel Source tunnel address
             * @param dst_tunnel Destination tunnel address
             */
            virtual void createIpsecPolicy( vector<TrafficSelector*> src_sel, vector<TrafficSelector*> dst_sel, Enums::DIRECTION direction, Enums::POLICY_ACTION action, uint32_t priority, Enums::PROTOCOL_ID ipsec_protocol, Enums::IPSEC_MODE mode, const IpAddress* src_tunnel, const IpAddress* dst_tunnel , bool autogen = false, bool sub = false) = 0;

            /**
             * Deletes an IPSEC policy
             * @param src_sel Source selector
             * @param dst_sel Destination selector
             * @param direction Direction of the policy
             */
            virtual void deleteIpsecPolicy( vector< TrafficSelector*> src_sel, vector< TrafficSelector*> dst_sel, Enums::DIRECTION direction ) = 0;

            /**
             * Deletes all the IPsec policies
             */
            virtual void flushIpsecPolicies() = 0;

            /**
             * Deletes all the IPSEC SAs
             */
            virtual void flushIpsecSas() = 0;

            virtual void updatePolicies(bool show) = 0;
            
            virtual void updateIpsecSaAddresses(const IpAddress & old_address, const IpAddress & new_address) = 0;
            
            virtual void updateIpsecPolicyAddresses(const IpAddress & old_address, const IpAddress & new_address) = 0;

            virtual ~IpsecControllerImpl();
    };
}

#endif
