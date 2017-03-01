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
#ifndef PAYLOAD_CONF_H
#define PAYLOAD_CONF_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "payload.h"
#include "configurationattribute.h"

#include "autovector.h"

using namespace std;

namespace openikev2 {

    /**
        This class represents a Configuration Payload.
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alejandro_perez@dif.um.es, pedroj.fernandez@dif.um.es>
    */
    class Payload_CONF : public Payload {

            /****************************** ENUMS ******************************/
        public:
            /** Configuration types */
            enum CONFIGURATION_TYPE {
                RESERVED    = 0,    /**< Reserved */
                CFG_REQUEST = 1,    /**< Configuration request */
                CFG_REPLY   = 2,    /**< Configuration reply */
                CFG_SET     = 3,    /**< Configuration set */
                CFG_ACK     = 4,    /**< Configuration ACK */
            };

            /****************************** ATTRIBUTES ******************************/
        protected:
            CONFIGURATION_TYPE configuration_type;          /**< Configuration type */
        public:
            AutoVector<ConfigurationAttribute> attributes;  /**< Configuration attribute collection */

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new Payload_CONF
             * @param configuration_type Configuration type
             */
            Payload_CONF( CONFIGURATION_TYPE configuration_type );

            /**
             * Creates a new Payload_CONF cloning another one
             * @param other Other Payload_CONF to be cloned
             */
            Payload_CONF ( const Payload_CONF& other );

            /**
             * Creates a new Payload_KE based on its binary representation.
             * @param byte_buffer Buffer with its read pointer at the "payload length" field
             */
            static auto_ptr<Payload_CONF> parse( ByteBuffer& byte_buffer );

            /**
             * Translate from Configuration type IDs to strings
             * @param conf_type Configuration type IDs
             * @return String with the configuration type
             */
            static string CONFIGURATION_TYPE_STR( CONFIGURATION_TYPE conf_type );

            /**
             * Add an attribute to the configuration
             * @param attribute Attribute to be added
             */
            void addConfigurationAttribute( auto_ptr<ConfigurationAttribute> attribute );

            /**
             * Obtains the first configuration attribute of the indicated type
             * @param type Configuration attribute type
             * @return The first configuration attribute. NULL if no attribute of such type is found
             */
            virtual ConfigurationAttribute* getFirstConfigurationAttributeByType( ConfigurationAttribute::CONFIGURATION_ATTRIBUTE_TYPE type );

            virtual ConfigurationAttribute& getUniqueConfigurationAttributeByType( ConfigurationAttribute::CONFIGURATION_ATTRIBUTE_TYPE type );

            virtual vector<ConfigurationAttribute*> getConfigurationAttributesByType( ConfigurationAttribute::CONFIGURATION_ATTRIBUTE_TYPE type );

            virtual void getBinaryRepresentation( ByteBuffer& byte_buffer ) const;

            virtual string toStringTab( uint8_t tabs ) const ;

            virtual auto_ptr<Payload> clone() const;

            virtual ~Payload_CONF();
    };
}
#endif
