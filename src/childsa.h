/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
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
#ifndef CHILDSA_H
#define CHILDSA_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "printable.h"
#include "proposal.h"
#include "enums.h"
#include "diffiehellman.h"
#include "keyring.h"
#include "payload_ts.h"
#include "payload_conf.h"
#include "childsaconfiguration.h"
#include "attributemap.h"
#include "childsarequest.h"

using namespace std;

namespace openikev2 {

    /**
        This class represents a CHILD_SA
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class ChildSa: public Printable {

            /****************************** ENUMS ******************************/
        public:
            /** ChildSa states */
            enum CHILD_SA_STATE {
                CHILD_SA_CREATING,           /**< ChildSa is not yet created */
                CHILD_SA_ESTABLISHED,        /**< ChildSa is established */
                CHILD_SA_DELETING,           /**< A delete informational has been sent for this ChildSa */
                CHILD_SA_REKEYED,            /**< The ChildSa has been rekeyed (usually by the other peer) */
                CHILD_SA_REKEYING,           /**< The ChildSa is being rekeyed and it is waiting for the response */
            };

            /****************************** ATTRIBUTES ******************************/
        protected:
            auto_ptr<ChildSaConfiguration> child_sa_configuration; /**< IPsec configuration to be used with this ChildSa */
            CHILD_SA_STATE state;                                  /**< ChildSa state */

        public:
            uint32_t inbound_spi;                           /**< SPI of the inbound IPsec SA */
            uint32_t outbound_spi;                          /**< SPI of the outbound IPsec SA */
            uint32_t rekeyed_spi;                           /**< SPI of the inbound IPsec SA of the rekeyed SA (if applicable) */
            bool child_sa_initiator;                        /**< Indicates if we are the initiators of this ChildSa */
            auto_ptr<Payload_TS> my_traffic_selector;       /**< My traffic selector for this ChildSa */
            auto_ptr<Payload_TS> peer_traffic_selector;     /**< Peer traffic selector for this ChildSa */
            auto_ptr<ByteArray> my_nonce;                   /**< My nonce for this ChildSa */
            auto_ptr<ByteArray> peer_nonce;                 /**< Peer nonce for this ChildSa */
            auto_ptr<KeyRing> keyring;                      /**< Keyring for this ChildSa */
            auto_ptr<DiffieHellman> pfs_dh;                 /**< DiffieHellman object for this ChildSa when PFS is desired */
            Enums::IPSEC_MODE mode;                         /**< IPsec mode of the ChildSa */
            Enums::PROTOCOL_ID ipsec_protocol;              /**< IPsec protocol ID */
            auto_ptr<AttributeMap> attributemap;            /**< Extra Attributes */

            /****************************** METHODS ******************************/
        protected:
            /**
             * Returns the textual representation of a CHILD_SA_STATE value
             * @param state CHILD_SA_STATE value
             * @return Textual respresentation
             */
            static string CHILD_SA_STATE_STR( CHILD_SA_STATE state );

        public:
            /**
             * Creates a new empty ChildSa
             * @param inbound_spi Indicates the inbound SPI value for this ChildSa
             * @param child_sa_initiator Indicates if we are the initiators of this ChildSa
             */
            ChildSa( uint32_t inbound_spi, bool child_sa_initiator );

            /**
             * Creates a new empty ChildSa
             * @param inbound_spi Indicates the inbound SPI value for this ChildSa
             * @param ipsec_protocol Indicates the IPSEC protocol (AH or ESP) for this ChildSa
             * @param mode Indicates the IPSEC mode (TRANSPORT or TUNNEL) for this ChildSa
             * @param child_sa_initiator Indicates if we are the initiators of this ChildSa
             */
            ChildSa( uint32_t inbound_spi, Enums::PROTOCOL_ID ipsec_protocol, bool child_sa_initiator );

            /**
             * Creates a new ChildSa object based on a ChildSaRequest template
             * @param inbound_spi Indicates the inbound SPI value for this ChildSa
             * @param child_sa_request ChildSaRequest object
             */
            ChildSa( uint32_t inbound_spi, auto_ptr<ChildSaRequest> child_sa_request);

            /**
             * Creates a new ChildSa intendened for rekeying purposes (in the initiator)
             * @param inbound_spi Indicates the inbound SPI value for this ChildSa
             * @param child_sa_initiator Indicates if we are the initiators of this ChildSa
             * @param rekeyed_child_sa ChildSa being rekeyed
             */
            ChildSa( uint32_t inbound_spi, bool child_sa_initiator, const ChildSa& rekeyed_child_sa );

            /**
             * Determines what ChildSa has the minimal nonce value
             * @param child_sa1 ChildSa object to compare its nonces
             * @param child_sa2 ChildSa object to compare its nonces
             * @return The ChildSa with the minimal nonce value
             */
            static ChildSa& hasMinNonce( const ChildSa& child_sa1, const ChildSa& child_sa2 );

            /**
             * Gets the proposal to be used for this ChildSa.
             * @return ChildSa proposal
             */
            virtual Proposal& getProposal() const;

            /**
             * Gets the IPsec configuration of this ChildSa
             * @return IPsec configuration
             */
            virtual ChildSaConfiguration& getChildSaConfiguration() const;

            /**
             * Changes the ChildSa proposal.
             * @param new_proposal New proposal to use with this ChildSa.
             */
            virtual void setProposal( auto_ptr<Proposal> new_proposal );

            /**
             * Changes the entire ChildSaConfiguration of the CHILD_SA
             * @param child_sa_configuration New IPsec configuration to be established
             */
            virtual void setChildSaConfiguration( auto_ptr<ChildSaConfiguration> child_sa_configuration );

            /**
             * Indicates if a ChildSa is equal to another.
             * @param other Other ChildSa
             * @return TRUE if both are equals. FALSE otherwise.
             */
            virtual bool equals( const ChildSa& other ) const;

            /**
             * Sets the state for this ChildSa
             * @param next_state The new state
             */
            virtual void setState( CHILD_SA_STATE next_state );

            /**
             * Gets the ChildSa ID (the inbound SPI) in a textual representation
             * @return The ChildSa ID
             */
            virtual auto_ptr<ByteArray> getId() const;

            /**
             * Gets the ChildSa state
             * @return The current state
             */
            virtual CHILD_SA_STATE getState() const;

            /**
             * Gets the string to be shown in the log
             * @return The ChildSa ID
             */
            virtual string getLogId() const;

            virtual string toStringTab( uint8_t tabs ) const;

            virtual ~ChildSa();

    };
}

#endif
