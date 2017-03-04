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
#include "attributemap.h"
#include "exception.h"

namespace openikev2 {

    AttributeMap::AttributeMap() {}

    AttributeMap::~AttributeMap() {
        // deletes all the reamining attributes
        map<string, Attribute*>::iterator it = this->attribute_map.begin();
        while ( it != this->attribute_map.end() ) {
            delete ( *it ).second;
            it++;
        }
    }

    void AttributeMap::addAttribute( string key, auto_ptr<Attribute> attribute ) {
        // Find if this key already exist in the attribute map
        map<string, Attribute*>::iterator it = this->attribute_map.find( key );
        if ( it != this->attribute_map.end() )
            throw DuplicatedAttributeException( "Duplicated attribute <" + key + ">" );

        // Inserts the attribute in the map
        this->attribute_map.insert( pair<string, Attribute*>( key, attribute.release() ) );
    }

    void AttributeMap::deleteAttribute( string key ) {
        map<string, Attribute*>::iterator it = this->attribute_map.find( key );
        if ( it != this->attribute_map.end() ) {
            delete ( *it ).second;
            this->attribute_map.erase( it );
        }
    }

    bool AttributeMap::existsAttribute( string key ) {
        map<string, Attribute*>::iterator it = this->attribute_map.find( key );
        if ( it != this->attribute_map.end() ) {
            return true;
        }
        return false;
    }

    auto_ptr<AttributeMap> AttributeMap::clone( ) const {
        auto_ptr<AttributeMap> result( new AttributeMap() );

        for ( map<string, Attribute*>::const_iterator it = this->attribute_map.begin(); it != this->attribute_map.end(); it++ )
            result->addAttribute( ( *it ).first, ( *it ).second->cloneAttribute() );

        return result;
    }

    string AttributeMap::toStringTab( uint8_t tabs ) const {
        ostringstream oss;

        // Prints object name
        oss << Printable::generateTabs( tabs ) << "<ATTRIBUTE_MAP> {\n";

        // For all the attributes
        for ( map<string, Attribute*>::const_iterator it = this->attribute_map.begin(); it != this->attribute_map.end(); it++ )
            oss << Printable::generateTabs( tabs + 1 ) << ( *it ).first << " = " << ( *it ).second->toString() << "\n";

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();
    }

    void AttributeMap::inherit( AttributeMap & other ) {
        //for ( map<string, Attribute*>::iterator it = other.attribute_map.begin(); it != other.attribute_map.end(); it++ ) {
        //    try {
        //        this->addAttribute( it->first, auto_ptr<Attribute> ( it->second ) );
        //        other.attribute_map.erase( it );
        //    }
        //    catch ( DuplicatedAttributeException& ex ) {}
        //}
        this->attribute_map.swap(other.attribute_map);

    }
}



