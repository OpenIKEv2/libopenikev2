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
#include "networkprefix.h"
#include "utils.h"
#include "bytebuffer.h"
#include "exception.h"
#include "networkcontroller.h"

namespace openikev2 {

    NetworkPrefix::NetworkPrefix( auto_ptr< IpAddress > network_address, uint16_t prefixlen ) {
        if (prefixlen > network_address->getAddressSize() * 8 )
            throw Exception("Prefixlen is greater than address size. Prefixlen=" + intToString(prefixlen) + " Address=" + network_address->toString() );
            
        this->network_address = network_address;
        this->prefixlen = prefixlen;

        auto_ptr<ByteArray> addr = this->network_address->getBytes();
        auto_ptr<ByteArray> mask = this->getMask();
        auto_ptr<ByteBuffer> bytes ( new ByteBuffer ( addr->size() ) );

        for ( uint16_t i = 0; i < addr->size(); i++ )
            bytes->writeInt8( ( *addr )[i] & ( *mask ) [i] );

        this->network_address = NetworkController::getIpAddress( this->network_address->getFamily(), auto_ptr<ByteArray> ( bytes ) );
    }

    NetworkPrefix::NetworkPrefix( const NetworkPrefix & other ) {
        this->network_address = other.network_address->clone();
        this->prefixlen = other.prefixlen;
    }

    NetworkPrefix::~NetworkPrefix() {
    }

    IpAddress & NetworkPrefix::getNetworkAddress() const {
        return *this->network_address;
    }

    uint16_t NetworkPrefix::getPrefixLen() const {
        return this->prefixlen;
    }

    auto_ptr< NetworkPrefix > NetworkPrefix::clone() const {
        return auto_ptr<NetworkPrefix> ( new NetworkPrefix( *this ) );
    }

    string NetworkPrefix::toStringTab( uint8_t tabs ) const {
        return this->network_address->toString() + "/" + intToString( this->prefixlen );
    }


    bool NetworkPrefix::containsIpAddress( const IpAddress & ip_address ) const {
        // checks family
        if ( this->network_address->getFamily() != ip_address.getFamily() )
            return false;

        // checks size
        if ( this->network_address->getAddressSize() != ip_address.getAddressSize() )
            return false;

        uint16_t bytelen = ( this->prefixlen / 8 );
        if ( this->prefixlen % 8 )
            bytelen++;

        auto_ptr<ByteArray> addr1 = this->network_address->getBytes();
        auto_ptr<ByteArray> addr2 = ip_address.getBytes();
        auto_ptr<ByteArray> mask_A = this->getMask();

        for ( uint16_t i = 0; i < bytelen;i++ )
            if ( ( ( *addr1 )[i] & ( *mask_A )[i] ) != ( ( *addr2 )[i] & ( *mask_A )[i] ) )
                return false;

        return true;
    }

    auto_ptr< ByteArray > NetworkPrefix::getMask() const {
        return this->getMask( this->prefixlen, this->network_address->getAddressSize() );
    }

    auto_ptr< ByteArray > NetworkPrefix::getMask( uint16_t prefixlen, uint16_t mask_size ) {
        auto_ptr<ByteBuffer> mask ( new ByteBuffer ( mask_size ) );

        for ( uint8_t i = 0; i < prefixlen / 8; i++ )
            mask->writeInt8( 0xFF );

        if ( prefixlen % 8 )
            mask->writeInt8( 0xFF << ( 8 - ( prefixlen % 8 ) ) );

        mask->fillBytes( mask_size - mask->size(), 0 );
        return auto_ptr<ByteArray> ( mask );
    }

    auto_ptr< IpAddress > NetworkPrefix::getBroadCastAddress() const {
        auto_ptr<ByteArray> addr = this->network_address->getBytes();
        auto_ptr<ByteArray> mask = this->getMask();

        auto_ptr<ByteBuffer> result ( new ByteBuffer ( addr->size() ) );

        for ( uint8_t i = 0; i < addr->size(); i++ )
            result->writeInt8( ( *addr )[i] | ~( *mask ) [i] );

        return auto_ptr<IpAddress> ( NetworkController::getIpAddress( this->network_address->getFamily(), auto_ptr<ByteArray> ( result ) ) );
    }

    auto_ptr< Attribute > NetworkPrefix::cloneAttribute() const {
        return auto_ptr<Attribute> ( this->clone() );
    }

    bool NetworkPrefix::operator ==( const NetworkPrefix & other ) const {
        if ( this->prefixlen != other.prefixlen )
            return false;

        return ( *this->network_address == *other.network_address );
    }

    uint16_t NetworkPrefix::getPrefixLen( const ByteArray & mask ) {
        uint8_t current_prefixlen = 0;

        for ( uint32_t i = 0; i < mask.size(); i++ ) {
            for ( uint32_t j = 0; j < 8; j++ ) {
                uint8_t guard = 0x80 >> j;
                if ( mask[i] & guard )
                    current_prefixlen++;
                else
                    break;
            }
        }

        return current_prefixlen;
    }

}













