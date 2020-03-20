/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
 ***************************************************************************/
#ifndef OPENIKEV2NETWORKPREFIX_H
#define OPENIKEV2NETWORKPREFIX_H

#include "ipaddress.h"
#include "attribute.h"

namespace openikev2 {

    /**
     This class represents a Network prefix, composed by an network address and a prefix size
     @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class NetworkPrefix : public Attribute {
            /****************************** ATTRIBUTES ******************************/
        protected:
            auto_ptr<IpAddress> network_address;        /**< Network address */
            uint16_t prefixlen;                         /**< Prefix length */

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new NetworkPrefix object
             * @param network_address Network address
             * @param prefixlen Prefix length
             */
            NetworkPrefix( auto_ptr<IpAddress> network_address, uint16_t prefixlen );

            /**
             * Creates a new NetworkPrefix object cloning another one
             * @param other Other NetworkPrefix object
             */
            NetworkPrefix( const NetworkPrefix& other );

            /**
             * Gets the network address
             * @return The network address
             */
            virtual IpAddress& getNetworkAddress() const;

            /**
             * Get the prefix length
             * @return The prefix length
             */
            virtual uint16_t getPrefixLen() const;

            /**
             * Obtains the broadcast address of the network
             * @return The broadcast address of the network
             */
            virtual auto_ptr<IpAddress> getBroadCastAddress() const;

            /**
             * Obtains the mask of the network
             * @return The mask of the network
             */
            virtual auto_ptr<ByteArray> getMask() const;

            /**
             * Obtains the mask of the network
             * @param prefixlen Prefix length
             * @param address_size Mask size
             * @return The mask of the network
             */
            static auto_ptr<ByteArray> getMask(uint16_t prefixlen, uint16_t mask_size);

            /**
             * Obtains the prefix length of the indicated mask
             * @param mask Mask
             * @return The prefix length
             */
            static uint16_t getPrefixLen(const ByteArray& mask);

            /**
            * Returns a clone of this object
            * @return A clone of this object
            */
            virtual auto_ptr<NetworkPrefix> clone() const;

            /**
            * Compares if this NetworkPrefix object is equal to other
            * @param other The other NetworkPrefix
            * @return TRUE if both are equals, FALSE otherwise
            */
            virtual bool operator==( const NetworkPrefix &other ) const;

            /**
            * Indicates if the IpAddress is contained in the network prefix.
            * @param ip_address IpAddress to be checked
            * @return TRUE if the address is contained in the network prefix. FALSE otherwise
            */
            virtual bool containsIpAddress( const IpAddress& ip_address ) const;

            virtual auto_ptr<Attribute> cloneAttribute() const ;

            virtual string toStringTab( uint8_t tabs ) const;

            virtual ~NetworkPrefix();

    };

}

#endif
