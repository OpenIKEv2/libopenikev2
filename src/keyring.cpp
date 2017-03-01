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
#include "keyring.h"
#include "cryptocontroller.h"

#include <assert.h>

namespace openikev2 {

    KeyRing::~KeyRing() {}

    void KeyRing::generateIkeSaKeys( ByteArray & nonce_i, ByteArray & nonce_r, uint64_t spi_i, uint64_t spi_r, ByteArray & shared_secret, ByteArray* old_sk_d ) {
        // Concat the two nonces (Ni | Nr)
        ByteBuffer nonces( nonce_i.size() + nonce_r.size() );
        nonces.writeByteArray( nonce_i );
        nonces.writeByteArray( nonce_r );

        // Generate SKEYSEED
        // if not rekeying then old_sk_d = NULL
        // SKEYSEED = prf(Ni | Nr, g^ir)
        if ( old_sk_d == NULL ) {
            assert( shared_secret.size() > 0 );
            this->skeyseed = this->prf->prf( nonces, shared_secret );
        }

        // If rekeying, then old_sk_d != NULL
        // SKEYSEED = prf(SK_d (old), [g^ir (new)] | Ni | Nr)
        else {
            // temp = [g^ir] | Ni | Nr
            ByteBuffer temp( shared_secret.size() + nonces.size() );
            temp.writeByteArray( shared_secret );
            temp.writeByteArray( nonces );
            this->skeyseed = this->prf->prf( *old_sk_d, temp );
        }

        // Total size of the concatenated keys (SK_d | SK_ai | SK_ar | SK_ei | SK_er | SK_pi | SK_pr)
        uint16_t total_size = this->prf->prf_size + this->integ_key_size * 2 + this->encr_key_size * 2 + this->prf->prf_size * 2;

        // generate the S value used in the prf+ function  (S = Ni | Nr | SPIi | SPIr)
        ByteBuffer sequence( nonces.size() + 8 + 8 );
        sequence.writeByteArray( nonces );
        sequence.writeBuffer( &spi_i, 8 );
        sequence.writeBuffer( &spi_r, 8 );

        auto_ptr<ByteArray> concat_keys = this->prf->prfPlus( *this->skeyseed, sequence, total_size );

        ByteBuffer temp( *concat_keys );

        // Copies the values of the keys on their buffers
        this->sk_d = temp.readByteArray( this->prf->prf_size );
        this->sk_ai = temp.readByteArray( this->integ_key_size );
        this->sk_ar = temp.readByteArray( this->integ_key_size );
        this->sk_ei = temp.readByteArray( this->encr_key_size );
        this->sk_er = temp.readByteArray( this->encr_key_size );
        this->sk_pi = temp.readByteArray( this->prf->prf_size );
        this->sk_pr = temp.readByteArray( this->prf->prf_size );
    }

    void KeyRing::generateChildSaKeys( ByteArray & nonce_i, ByteArray & nonce_r, ByteArray & sk_d, ByteArray * shared_secret ) {
        // nonces = [g^ir (new)] | Ni | Nr
        auto_ptr<ByteBuffer> nonces;
        if ( shared_secret != NULL ) {
            nonces.reset ( new ByteBuffer( shared_secret->size() + nonce_i.size() + nonce_r.size() ) );
            nonces->writeByteArray( *shared_secret );
            nonces->writeByteArray( nonce_i );
            nonces->writeByteArray( nonce_r );
        }
        else {
            nonces.reset ( new ByteBuffer( nonce_i.size() + nonce_r.size() ) );
            nonces->writeByteArray( nonce_i );
            nonces->writeByteArray( nonce_r );
        }

        // Total size of the concatenated keys = (encr_size + inter_size) * 2
        uint16_t total_size = ( this->encr_key_size + this->integ_key_size ) * 2;

        // Generates the key material
        auto_ptr<ByteArray> concat_keys = this->prf->prfPlus( sk_d, *nonces, total_size );

        ByteBuffer temp( concat_keys->size() );
        temp.writeByteArray( *concat_keys );

        // Copies the values of the keys on their buffers
        this->sk_ei = temp.readByteArray( this->encr_key_size );
        this->sk_ai = temp.readByteArray( this->integ_key_size );
        this->sk_er = temp.readByteArray( this->encr_key_size );
        this->sk_ar = temp.readByteArray( this->integ_key_size );
    }

    string KeyRing::toStringTab( uint8_t tabs ) const {
        ostringstream oss;

        oss << Printable::generateTabs( tabs ) << "<KEY_RING> {\n";

        if ( this->skeyseed.get() != NULL )
            oss << Printable::generateTabs( tabs + 1 ) << "SKEYSEED=" << this->skeyseed->toStringTab( tabs + 1 ) << "\n";

        if ( this->sk_d.get() != NULL )
            oss << Printable::generateTabs( tabs + 1 ) << "SK_d=" << this->sk_d->toStringTab( tabs + 1 ) << "\n";

        if ( this->sk_ai.get() != NULL )
            oss << Printable::generateTabs( tabs + 1 ) << "SK_ai=" << this->sk_ai->toStringTab( tabs + 1 ) << "\n";

        if ( this->sk_ar.get() != NULL )
            oss << Printable::generateTabs( tabs + 1 ) << "SK_ar=" << this->sk_ar->toStringTab( tabs + 1 ) << "\n";

        if ( this->sk_ei.get() != NULL )
            oss << Printable::generateTabs( tabs + 1 ) << "SK_ei=" << this->sk_ei->toStringTab( tabs + 1 ) << "\n";

        if ( this->sk_er.get() != NULL )
            oss << Printable::generateTabs( tabs + 1 ) << "SK_er=" << this->sk_er->toStringTab( tabs + 1 ) << "\n";

        if ( this->sk_pi.get() != NULL )
            oss << Printable::generateTabs( tabs + 1 ) << "SK_pi=" << this->sk_pi->toStringTab( tabs + 1 ) << "\n";

        if ( this->sk_pr.get() != NULL )
            oss << Printable::generateTabs( tabs + 1 ) << "SK_pr=" << this->sk_pr->toStringTab( tabs + 1 ) << "\n";

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();
    }
}
