/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
 ***************************************************************************/
#ifndef OPENIKEV2SOCKETADDRESS_H
#define OPENIKEV2SOCKETADDRESS_H

#include "ipaddress.h"

namespace openikev2 {

    /**
    This abstract class represents a socket address (IP address and port number).
     @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class SocketAddress: public Printable {
            /****************************** METHODS ******************************/
        public:
            /**
            * Compares if this IpAddress object is equal to other
            * @param other The other IpAddress
            * @return TRUE if both are equals, FALSE otherwise
            */
            virtual bool operator==( const SocketAddress &other ) const;

            /**
            * Gets the IP address
            * @return The IP address
            */
            virtual IpAddress& getIpAddress() const = 0;

            /**
             * Gets the port number
             * @return The port number
             */
            virtual uint16_t getPort() const = 0;

            /**
             * Sets the IP address
             * @param ip_address New IP address
             */
            virtual void setIpAddress( auto_ptr<IpAddress> ip_address) = 0;

            /**
             * Sets the port number
             * @param port New port number
             */
            virtual void setPort( uint16_t port ) = 0;

            /**
            * Creates a new SocketAddress object cloning this.
            * @return The new IPaddress object.
            */
            virtual auto_ptr<SocketAddress> clone() const = 0;

            virtual string toStringTab( uint8_t tabs ) const = 0;

            virtual ~SocketAddress();
    };

}

#endif
