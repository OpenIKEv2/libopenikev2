/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef IPADDRESS_H
#define IPADDRESS_H

#include "stdint.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "attribute.h"
#include "bytearray.h"
#include "enums.h"

namespace openikev2 {

    /**
        This abstract class represents an IP address of any kind.
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */

    class IpAddress: public Attribute {
            /****************************** METHODS ******************************/
        public:
            /**
             * Returns IpAddress bytes representation size.
             * @return Address size
             */
            virtual uint16_t getAddressSize() const = 0;

            /**
             * Gets the IpAddress family.
             * @return Address family.
             */
            virtual Enums::ADDR_FAMILY getFamily() const = 0;

            /**
            * Compares if this IpAddress object is equal to other
            * @param other The other IpAddress
            * @return TRUE if both are equals, FALSE otherwise
            */
            virtual bool operator==( const IpAddress &other ) const;

            /**
             * Gets the byte representation of the IPAdress.
             * @return Byte representation.
             */
            virtual auto_ptr<ByteArray> getBytes() const = 0;

            /**
             * Creates a new IpAddress object cloning this as an Attribute
             * @return The new Attribute object
             */
            virtual auto_ptr<Attribute> cloneAttribute() const ;

            /**
             * Creates a new IpAddress object cloning this.
             * @return The new IPaddress object.
             */
            virtual auto_ptr<IpAddress> clone() const = 0;

            virtual string toStringTab( uint8_t tabs ) const = 0;

	    virtual string getIfaceName() = 0;

            virtual ~IpAddress();
    };

}
#endif
