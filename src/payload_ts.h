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
#ifndef PAYLOAD_TS_H
#define PAYLOAD_TS_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "payload.h"
#include "trafficselector.h"

#include "autovector.h"

namespace openikev2 {

    /**
        This class represents a Traffic Selector Payload.
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class Payload_TS : public Payload {

            /****************************** ATTRIBUTES ******************************/
        protected:
            AutoVector<TrafficSelector> traffic_selectors;      /**< Collection of TrafficSelector objects*/

            /****************************** METHODS ******************************/
        protected:
            /**
             * Creates a new empty Payload_TS
             */
            Payload_TS();

            /**
             * Creates a new Payload_TS inserting the indicated TrafficSelector.
             * @param traffic_selector TrafficSelector to be inserted.
             */
            Payload_TS( auto_ptr<TrafficSelector> traffic_selector );

            /**
             * Creates a new Payload_TS based on its binary representation.
             * @param byte_buffer Buffer with its read pointer at the "payload length" field
             */
            static auto_ptr<Payload_TS> parse( ByteBuffer& byte_buffer );

        public:
            /**
             * Creates a new Payload_TS from other.
             * @param other Other Payload_TS to be cloned.
             */
            Payload_TS( const Payload_TS& other );

            /**
             * Adds a TrafficSelector to the Payload.
             * @param traffic_selector TrafficSelector to be added.
             */
            virtual void addTrafficSelector( auto_ptr<TrafficSelector> traffic_selector );

            /**
             * Compares this Payload_TS with another.
             * @param other Other Payload_TS
             * @return TRUE if payloads are equals. FALSE otherwise.
             */
            virtual bool operator==( const Payload_TS& other ) const;

            /**
             * Indicates if the Payload_TS has the indicated traffic_selector
             * @param traffic_selector Traffic selector
             * @return TRUE if found. FALSE otherwise
             */
            virtual bool hasTrafficSelector ( const TrafficSelector& traffic_selector ) const;

            /**
             * Gets the internal traffic selectors of the payload
             * @return The traffic selectors
             */
            vector<TrafficSelector*> getTrafficSelectors() const;

            virtual void getBinaryRepresentation( ByteBuffer& byte_buffer ) const ;

            virtual string toStringTab( uint8_t tabs ) const ;

            virtual auto_ptr<Payload> clone() const ;

            virtual ~Payload_TS();
    };
}
#endif
