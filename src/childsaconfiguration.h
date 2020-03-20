/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
 ***************************************************************************/
#ifndef CHILDSACONFIGURATION_H
#define CHILDSACONFIGURATION_H

#include "proposal.h"
#include "printable.h"
#include "attributemap.h"

using namespace std;

namespace openikev2 {

    /**
        This class represents an IPsec SA configuration
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class ChildSaConfiguration : public Printable {

            /****************************** ATTRIBUTES ******************************/
        protected:
            auto_ptr<Proposal> proposal;            /**< IPSEC proposal */
        public:
            uint32_t max_allocations_soft;          /**< Maximun number of allocations. Soft limit. */
            uint32_t max_allocations_hard;          /**< Maximun number of allocations. Hard limit. */
            uint32_t lifetime_soft;                 /**< Lifetime (in seconds) of the SA. Soft limit */
            uint32_t lifetime_hard;                 /**< Lifetime (in seconds) of the SA. Hard limit */
            uint32_t max_bytes_soft;                /**< Lifetime (in bytes) of the SA. Soft limit */
            uint32_t max_bytes_hard;                /**< Lifetime (in bytes) of the SA. Hard limit */
            auto_ptr<AttributeMap> attributemap;    /**< Using this map the class attributes can be extended dynamically */

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new ChildSaConfiguration object.
             * @param proposal IPSEC proposal
             */
            ChildSaConfiguration( auto_ptr<Proposal> proposal );

            /**
             * Creates a clone of this IPSEC configuration.
             * @return A new ChildSaConfiguration object
             */
            virtual auto_ptr<ChildSaConfiguration> clone() const;

            /**
             * Gets the internal proposal structure
             * @return The proposal structure
             */
            virtual Proposal& getProposal();

            /**
             * Sets the internal proposal structure
             * @param proposal The proposal structure
             */
            virtual void setProposal( auto_ptr<Proposal> proposal );

            virtual string toStringTab( uint8_t tabs ) const;

            virtual ~ChildSaConfiguration();
    };
};
#endif
