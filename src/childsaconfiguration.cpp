/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
 ***************************************************************************/
#include "childsaconfiguration.h"
#include "assert.h"

namespace openikev2 {
    ChildSaConfiguration::ChildSaConfiguration( auto_ptr<Proposal> proposal ) {
        assert( proposal.get() != NULL );

        this->proposal = proposal;

        this->lifetime_hard = 0xFFFFFFF;
        this->lifetime_soft = 0xFFFFFFF;

        this->max_bytes_hard = 0xFFFFFFF;
        this->max_bytes_soft = 0xFFFFFFF;

        this->max_allocations_hard = 0xFFFFFFF;
        this->max_allocations_soft = 0xFFFFFFF;

        this->attributemap.reset( new AttributeMap() );
    }

    ChildSaConfiguration::~ChildSaConfiguration() {}

    string ChildSaConfiguration::toStringTab( uint8_t tabs ) const {
        ostringstream oss;

        oss << Printable::generateTabs( tabs ) << "<IPSEC_CONFIGURATION> {\n";

        oss << this->proposal->toStringTab( tabs + 1 );

        oss << Printable::generateTabs( tabs + 1 ) << "max_allocations_soft=[" << this->max_allocations_soft << "]\n";

        oss << Printable::generateTabs( tabs + 1 ) << "max_allocations_hard=[" << this->max_allocations_hard << "]\n";

        oss << Printable::generateTabs( tabs + 1 ) << "max_bytes_soft=[" << this->max_bytes_soft << "]\n";

        oss << Printable::generateTabs( tabs + 1 ) << "max_bytes_hard=[" << this->max_bytes_hard << "]\n";

        oss << Printable::generateTabs( tabs + 1 ) << "lifetime_soft=[" << this->lifetime_soft << "]\n";

        oss << Printable::generateTabs( tabs + 1 ) << "lifetime_hard=[" << this->lifetime_hard << "]\n";

        oss << this->attributemap->toStringTab( tabs + 1 );

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();
    }

    auto_ptr<ChildSaConfiguration> ChildSaConfiguration::clone( ) const {
        auto_ptr<ChildSaConfiguration> result( new ChildSaConfiguration( this->proposal->clone() ) );

        result->lifetime_hard = this->lifetime_hard;
        result->lifetime_soft = this->lifetime_soft;
        result->max_allocations_hard = this->max_allocations_hard;
        result->max_allocations_soft = this->max_allocations_soft;
        result->max_bytes_hard = this->max_bytes_hard;
        result->max_bytes_soft = this->max_bytes_soft;
        result->attributemap = this->attributemap->clone();

        return result;
    }

    void ChildSaConfiguration::setProposal( auto_ptr< Proposal > proposal ) {
        this->proposal = proposal;
    }

    Proposal& ChildSaConfiguration::getProposal( ) {
        return *this->proposal;
    }
}

