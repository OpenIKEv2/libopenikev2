/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "payload_vendor.h"

#include "exception.h"
#include "utils.h"

namespace openikev2 {

    Payload_VENDOR::Payload_VENDOR( auto_ptr<ByteArray> vendor_id )
            : Payload ( PAYLOAD_VENDOR, false ) {

        assert( vendor_id.get() != NULL );
        this->vendor_id = vendor_id;
    }

    Payload_VENDOR::Payload_VENDOR( string vendor_id_str )
            : Payload ( PAYLOAD_VENDOR, false ) {
        assert( vendor_id_str.size() > 0 );

        this->vendor_id.reset ( new ByteArray( vendor_id_str.c_str(), vendor_id_str.size(), vendor_id_str.size() ) );
    }

    Payload_VENDOR::Payload_VENDOR( const Payload_VENDOR& other )
            : Payload ( PAYLOAD_VENDOR, false ) {

        this->vendor_id = other.vendor_id->clone();
    }

    auto_ptr<Payload_VENDOR> Payload_VENDOR::parse( ByteBuffer& byte_buffer ) {
        // reads payload size
        uint16_t payload_size = byte_buffer.readInt16();

        // Size must be at least size of fixed header
        if ( payload_size < 4 )
            throw ParsingException( "Payload_VENDOR length cannot be < 4 bytes." );

        // reads the vendor ID
        auto_ptr<ByteArray> vendor_id = byte_buffer.readByteArray( payload_size - 4 );

        return auto_ptr<Payload_VENDOR> ( new Payload_VENDOR( vendor_id ) );
    }

    Payload_VENDOR::~Payload_VENDOR() {}

    void Payload_VENDOR::getBinaryRepresentation( ByteBuffer & byte_buffer ) const {
        // writes payload_length value
        byte_buffer.writeInt16( 4 + this->vendor_id->size() );

        // writes the vendor id
        byte_buffer.writeByteArray( *this->vendor_id );
    }

    ByteArray& Payload_VENDOR::getVendorId() {
        return * this->vendor_id;
    }


    string Payload_VENDOR::toStringTab( uint8_t tabs ) const {
        ostringstream oss;

        oss << Printable::generateTabs( tabs ) << "<PAYLOAD_VENDOR> {\n";

        oss << Printable::generateTabs( tabs + 1 ) << "vendor_id=" << this->vendor_id->toStringTab( tabs + 1 ) << endl;

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();

    }

    auto_ptr<Payload> Payload_VENDOR::clone( ) const {
        return auto_ptr<Payload> ( new Payload_VENDOR( *this ) );
    }

}

