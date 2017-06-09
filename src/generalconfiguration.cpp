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
#include "generalconfiguration.h"
#include "utils.h"

namespace openikev2 {


    GeneralConfiguration::GeneralConfiguration() {
        this->cookie_threshold = 0xFFFF;
        this->cookie_lifetime = 0xFFFF;
        this->ike_max_halfopen_time = 0xFFFF;
        this->attributemap.reset ( new AttributeMap() );
        this->radvd_enabled = false;
    }

    GeneralConfiguration::~GeneralConfiguration() {  }

    string GeneralConfiguration::toStringTab( uint8_t tabs ) const {
        ostringstream oss;

        oss << Printable::generateTabs( tabs ) << "<GENERAL_CONFIGURATION> {\n";

        oss << Printable::generateTabs( tabs + 1 ) << "cookie_threshold=" << this->cookie_threshold << "\n";

        oss << Printable::generateTabs( tabs + 1 ) << "cookie_lifetime=" << this->cookie_lifetime << "\n";

        oss << Printable::generateTabs( tabs + 1 ) << "ike_max_halfopen_time=" << this->ike_max_halfopen_time << "\n";

	oss << Printable::generateTabs( tabs + 1 ) << "radvd_enabled=" << this->radvd_enabled << "\n";

	oss << Printable::generateTabs( tabs + 1 ) << "radvd_config_file=" << this->radvd_config_file << "\n";

        if ( this->vendor_id.get() != NULL )
            oss << Printable::generateTabs( tabs + 1 ) << "vendor_id=" << this->vendor_id->toStringTab( tabs + 1 ) << "\n";

        oss << this->attributemap->toStringTab( tabs + 1 );

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();
    }

    auto_ptr<GeneralConfiguration> GeneralConfiguration::clone( ) const {
        auto_ptr<GeneralConfiguration> result ( new GeneralConfiguration() );

        result->cookie_lifetime = this->cookie_lifetime;
        result->cookie_threshold = this->cookie_threshold;
        result->ike_max_halfopen_time = this->ike_max_halfopen_time;

        if ( this->vendor_id.get() != NULL )
            result->vendor_id = this->vendor_id->clone();

        result->attributemap = this->attributemap->clone();

        return result;
    }
}

