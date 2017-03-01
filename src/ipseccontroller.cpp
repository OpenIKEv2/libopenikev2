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
#include "ipseccontroller.h"
#include "threadcontroller.h"

namespace openikev2 {
    IpsecControllerImpl* IpsecController::implementation = NULL;

    void IpsecController::setImplementation( IpsecControllerImpl* impl ) {
        implementation = impl;
    }

    bool IpsecController::narrowPayloadTS( const Payload_TSi& received_payload_ts_i, const Payload_TSr & received_payload_ts_r, IkeSa& ike_sa, ChildSa& child_sa ) {
        assert ( implementation != NULL );
        return implementation->narrowPayloadTS( received_payload_ts_i, received_payload_ts_r, ike_sa, child_sa );
    }

    uint32_t IpsecController::getSpi( const IpAddress& src, const IpAddress& dst, Enums::PROTOCOL_ID protocol ) {
        assert ( implementation != NULL );
        return implementation->getSpi( src, dst, protocol );
    }

    void IpsecController::createIpsecSa( const IpAddress& src, const IpAddress& dst, const ChildSa& childsa ) {
        assert ( implementation != NULL );
        implementation->createIpsecSa( src, dst, childsa );
    }

    uint32_t IpsecController::deleteIpsecSa( const IpAddress& src, const IpAddress& dst, Enums::PROTOCOL_ID protocol, uint32_t spi ) {
        assert ( implementation != NULL );
        return implementation->deleteIpsecSa( src, dst, protocol, spi );
    }

    bool IpsecController::checkNarrowPayloadTS( const Payload_TSi & received_payload_ts_i, const Payload_TSr & received_payload_ts_r, ChildSa & child_sa ) {
        assert ( implementation != NULL );
        return implementation->checkNarrowPayloadTS( received_payload_ts_i, received_payload_ts_r, child_sa );
    }

    void IpsecController::createIpsecPolicy( const TrafficSelector& src_sel, const TrafficSelector & dst_sel, Enums::DIRECTION direction,  Enums::POLICY_ACTION action,  uint32_t priority, Enums::PROTOCOL_ID ipsec_protocol, Enums::IPSEC_MODE mode, const IpAddress* src_tunnel, const IpAddress* dst_tunnel, bool autogen, bool sub) {
        assert ( implementation != NULL );

        vector< TrafficSelector*> ts_i_collection, ts_r_collection;

        ts_i_collection.push_back( ( TrafficSelector* ) & src_sel );
        ts_r_collection.push_back( ( TrafficSelector* ) & dst_sel );

        createIpsecPolicy( ts_i_collection, ts_r_collection, direction, action, priority, ipsec_protocol, mode, src_tunnel, dst_tunnel , autogen, sub);
    }

    void IpsecController::createIpsecPolicy( vector< TrafficSelector*> src_sel, vector< TrafficSelector*> dst_sel, Enums::DIRECTION direction, Enums::POLICY_ACTION action, uint32_t priority, Enums::PROTOCOL_ID ipsec_protocol, Enums::IPSEC_MODE mode, const IpAddress* src_tunnel, const IpAddress* dst_tunnel, bool autogen, bool sub) {
        assert ( implementation != NULL );

        if ( direction == Enums::DIR_ALL ) {
            implementation->createIpsecPolicy( src_sel, dst_sel, Enums::DIR_OUT, action, priority, ipsec_protocol, mode, src_tunnel, dst_tunnel, autogen, sub);
            implementation->createIpsecPolicy( dst_sel, src_sel, Enums::DIR_IN, action, priority, ipsec_protocol, mode, dst_tunnel, src_tunnel, false, sub);
            implementation->createIpsecPolicy( dst_sel, src_sel, Enums::DIR_FWD, action, priority, ipsec_protocol, mode, dst_tunnel, src_tunnel, false, sub);
        }
        else
            implementation->createIpsecPolicy( src_sel, dst_sel, direction, action, priority, ipsec_protocol, mode, src_tunnel, dst_tunnel, autogen );
    }

    void IpsecController::deleteIpsecPolicy( vector< TrafficSelector*> src_sel, vector< TrafficSelector*> dst_sel, Enums::DIRECTION direction ) {
        assert ( implementation != NULL );
        implementation->deleteIpsecPolicy( src_sel, dst_sel, direction );
    }

    void IpsecController::flushIpsecSas( ) {
        assert ( implementation != NULL );
        implementation->flushIpsecSas();
    }

    void IpsecController::updatePolicies( bool show ) {
        assert ( implementation != NULL );
        implementation->updatePolicies(show);
    }

    void IpsecController::flushIpsecPolicies( ) {
        assert ( implementation != NULL );
        implementation->flushIpsecPolicies();
    }

    void IpsecController::updateIpsecSaAddresses( const IpAddress & old_address, const IpAddress & new_address ) {
        assert ( implementation != NULL );
        implementation->updateIpsecSaAddresses(old_address, new_address);
    }
    
    void IpsecController::updateIpsecPolicyAddresses( const IpAddress & old_address, const IpAddress & new_address ) {
        assert ( implementation != NULL );
        implementation->updateIpsecPolicyAddresses(old_address, new_address);
    }
}


