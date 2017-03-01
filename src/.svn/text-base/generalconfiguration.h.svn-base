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
#ifndef GENERALCONFIGURATION_H
#define GENERALCONFIGURATION_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "printable.h"
#include "attributemap.h"
#include "bytearray.h"

using namespace std;


namespace openikev2 {
    /**
        This class represents the General Configuration of OpenIKEv2
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alejandro_perez@dif.um.es, pedroj.fernandez@dif.um.es>
    */
    class GeneralConfiguration : public Printable {

            /****************************** ATTRIBUTES ******************************/
        public:
            uint32_t cookie_threshold;          /**< Number of Half-Opened IKE_SAs to start cookie DoS protection mechanism */
            uint32_t cookie_lifetime;           /**< Lifetime of the cookie secret. */
            uint32_t ike_max_halfopen_time;     /**< Maximun time to perform initial exchanges */
            bool radvd_enabled;
            string radvd_config_file;
            auto_ptr<ByteArray> vendor_id;      /**< Vendor ID to be used */
            auto_ptr<AttributeMap> attributemap; /**< Extra configuration attribute */

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new GeneralConfiguration object setting its default values
             */
            GeneralConfiguration();

            /**
             * Creates a clone of this GeneralConfiguration
             * @return A new GeneralConfiguration object
             */
            virtual auto_ptr<GeneralConfiguration> clone() const;

            virtual string toStringTab( uint8_t tabs ) const ;

            virtual ~GeneralConfiguration();

    };
};
#endif
