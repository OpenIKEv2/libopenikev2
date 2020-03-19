/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
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
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
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
