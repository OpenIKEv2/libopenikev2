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
#ifndef OPENIKEV2NETWORKPREFIX_H
#define OPENIKEV2NETWORKPREFIX_H

#include "ipaddress.h"
#include "attribute.h"

namespace openikev2 {

    /**
     This class represents a Network prefix, composed by an network address and a prefix size
     @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alejandro_perez@dif.um.es, pedroj.fernandez@dif.um.es>
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
