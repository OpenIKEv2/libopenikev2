/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Alejandro Perez Mendez     alejandro_perez@dif.um.es                  *
 *   Pedro J. Fernandez Ruiz    pedroj.fernandez@dif.um.es                 *
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
#include "childsacollection.h"
#include "ipseccontroller.h"
#include "threadcontroller.h"

namespace openikev2 {

    ChildSaCollection::ChildSaCollection() {
        this->mutex = ThreadController::getMutex();
    }

    ChildSaCollection::~ChildSaCollection() {
        AutoLock auto_lock( *this->mutex );
        // Deletes all CHILD SA (by inbound SPI)
        for ( map<uint32_t, ChildSa*>::iterator it = this->child_sa_collection_inbound.begin(); it != this->child_sa_collection_inbound.end(); it++ ) {
            delete it->second;
        }
    }

    ChildSa * ChildSaCollection::getChildSa( uint32_t spi ) {
        AutoLock auto_lock( *this->mutex );
        map<uint32_t, ChildSa*>::iterator it = this->child_sa_collection_inbound.find( spi );
        if ( it != this->child_sa_collection_inbound.end() )
            return ( *it ).second;

        it = this->child_sa_collection_outbound.find( spi );
        if ( it != this->child_sa_collection_outbound.end() )
            return ( *it ).second;

        return NULL;
    }

    ChildSa * openikev2::ChildSaCollection::getFirstChildSa() {
        AutoLock auto_lock( *this->mutex );
        if ( this->child_sa_collection_inbound.size() > 0 ) {
            map<uint32_t, ChildSa*>::iterator it = this->child_sa_collection_inbound.begin();
            return it->second;
        }
        else
            return NULL;
    }

    void ChildSaCollection::addChildSa( auto_ptr< ChildSa > child_sa ) {
        AutoLock auto_lock( *this->mutex );
        this->child_sa_collection_inbound[ child_sa->inbound_spi ] = child_sa.get();
        this->child_sa_collection_outbound[ child_sa->outbound_spi ] = child_sa.release();

    }

    string ChildSaCollection::toStringTab( uint8_t tabs ) const {
        AutoLock auto_lock( *this->mutex );
        ostringstream oss;

        oss << Printable::generateTabs( tabs ) << "<CHILD_SA_COLLECTION> {\n";

        oss << Printable::generateTabs( tabs ) << "Count=[" << intToString( this->child_sa_collection_inbound.size() ) << "]" << endl;

        for ( map<uint32_t, ChildSa*>::const_iterator it = this->child_sa_collection_inbound.begin(); it != this->child_sa_collection_inbound.end(); it++ )
            oss << it->second->toStringTab( tabs + 1 );

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();
    }

    void openikev2::ChildSaCollection::deleteChildSa( uint32_t spi ) {
        AutoLock auto_lock( *this->mutex );

        // deletes the inbound spi
        map<uint32_t, ChildSa*>::iterator it = this->child_sa_collection_inbound.find( spi );
        assert ( it != this->child_sa_collection_inbound.end() );
        ChildSa* child_sa = it->second;
        uint32_t outbound_spi = child_sa->outbound_spi; 
        this->child_sa_collection_inbound.erase( it );

        it = this->child_sa_collection_outbound.find( outbound_spi );
        assert ( it != this->child_sa_collection_outbound.end() );
        this->child_sa_collection_outbound.erase( it );

        //delete child_sa;
    }

    bool ChildSaCollection::hasHalfClosedChildSas( ) const {
        AutoLock auto_lock( *this->mutex );
        for ( map<uint32_t, ChildSa*>::const_iterator it = this->child_sa_collection_inbound.begin(); it != this->child_sa_collection_inbound.end(); it++ ) {
            ChildSa * current = it->second;
            if ( current->getState() == ChildSa::CHILD_SA_DELETING )
                return true;
        }
        return false;
    }

    bool ChildSaCollection::hasChildSa( uint32_t spi ) {
        AutoLock auto_lock( *this->mutex );
        map<uint32_t, ChildSa*>::iterator it = this->child_sa_collection_inbound.find( spi );
        if ( it != this->child_sa_collection_inbound.end() )
            return true;

        it = this->child_sa_collection_outbound.find( spi );
        if ( it != this->child_sa_collection_outbound.end() )
            return true;
        return false;
    }

    uint32_t ChildSaCollection::size() {
        AutoLock auto_lock( *this->mutex );
        return this->child_sa_collection_inbound.size();
    }
}













