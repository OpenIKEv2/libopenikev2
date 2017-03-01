/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Alejandro Perez Mendez     alejandro_perez@dif.um.es                  *
 *   Pedro J. Fernandez Ruiz    pedroj.fernandez@dif.um.es                 *
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
#ifndef OPENIKEV2AUTHENTICATOR_H
#define OPENIKEV2AUTHENTICATOR_H

#include "payload_auth.h"
#include "autovector.h"
#include "payload_cert.h"
#include "payload_cert_req.h"
#include "payload_eap.h"
#include "message.h"

namespace openikev2 {

    class IkeSa;

    /**
     This abstract class represets an Authenticator, that performs all the authentication related task.
     @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alejandro_perez@dif.um.es, pedroj.fernandez@dif.um.es>
    */
    class Authenticator : public Printable {

            /****************************** METHODS ******************************/
        public:
            /**
             * Indicates if the initiator should uses EAP authentication method
             * @return TRUE if the initiator should uses EAP. FALSE otherwise
             */
            virtual bool initiatorUsesEap() = 0;

            /**
             * Generates the Certificate Request payloads indicate to the peer the preferred CAs
             * @param ike_sa IkeSa
             * @return The Certificate Request payload collection
             */
            virtual AutoVector<Payload_CERT_REQ> generateCertificateRequestPayloads( const IkeSa& ike_sa ) = 0;

            /**
             * Generates the Certificate Payloads to be included in the IKE_AUTH message
             * @param ike_sa IkeSa
             * @param payload_cert_req_r Received Certificate Request payloads
             * @return The Certificate Payload collection to be included in the IKE_AUTH message
             */
            virtual AutoVector<Payload_CERT> generateCertificatePayloads( const IkeSa& ike_sa, const vector<Payload_CERT_REQ*> payload_cert_req_r ) = 0;

            /**
             * Generate the AUTH payload to be included in the AUTH message
             * @param ike_sa IkeSa
             * @return The new AUTH payload
             */
            virtual auto_ptr<Payload_AUTH> generateAuthPayload( const IkeSa& ike_sa ) = 0;

            /**
             * Verifies the AUTH payload included in the AUTH message
             * @param received_message Received AUTH message to be verified
             * @param ike_sa IkeSa
             * @return TRUE if the AUTH payload can be verified. FALSE otherwise
             */
            virtual bool verifyAuthPayload( const Message& received_message, const IkeSa& ike_sa ) = 0;

            /**
             * Processes an EAP request and generates the adecuated response (initiator only)
             * @param eap_request The received EAP request
             * @return The adecuated response. NULL if an error is found.
             */
            virtual auto_ptr<Payload_EAP> processEapRequest( const Payload_EAP& eap_request ) = 0;

            /**
             * Processes an EAP success (initiator only)
             * @param eap_success The received EAP success
             * @return The adecuated response. NULL if an error is found.
             */
            virtual void processEapSuccess( const Payload_EAP& eap_success ) = 0;

            virtual void processFinish( ) = 0;

            /**
            * Generates the Initial EAP request message to be included in the IKE_AUTH response (responder only)
            * @param  peer_id Peer ID
            * @return EAP payload to be included in the IKE_AUTH response
            */
            virtual auto_ptr<Payload_EAP> generateInitialEapRequest( const ID& peer_id ) = 0;

            /**
             * Processes the EAP response and generates the next EAP request, EAP success or EAP failure message (responder only)
             * @param eap_response Received EAP response
             * @return EAP request, succes or failure message to be sent
             */
            virtual auto_ptr<Payload_EAP> processEapResponse( const Payload_EAP& eap_response, const ID& peer_id ) = 0;

            /**
             * Generates the AUTH payload to be included at the end of an successful EAP authentication
             * @param ike_sa IkeSa
             * @return AUTH payload to be included
             */
            virtual auto_ptr<Payload_AUTH> generateEapAuthPayload( const IkeSa& ike_sa ) = 0;

            /**
             * Verifies the AUTH payload included at the end of an successful EAP authentication
             * @param received_message Received IKE_AUTH message
             * @param ike_sa IkeSa
             */
            virtual bool verifyEapAuthPayload( const Message& received_message, const IkeSa& ike_sa ) = 0;

            /**
             * Clones this Authenticator
             * @return A clone of this Authenticator
             */
            virtual auto_ptr<Authenticator> clone() const = 0;

            virtual string toStringTab( uint8_t tabs ) const = 0;

            virtual ~Authenticator();
    };

}

#endif
