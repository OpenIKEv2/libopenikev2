/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
 ***************************************************************************/
#include "enums.h"
#include "utils.h"

namespace openikev2 {
    string Enums::PROTOCOL_ID_STR( PROTOCOL_ID protocol ) {
        switch ( protocol ) {
            case Enums::PROTO_NONE:
                return "PROTO_NONE";
            case Enums::PROTO_AH:
                return "PROTO_AH";
            case Enums::PROTO_ESP:
                return "PROTO_ESP";
            case Enums::PROTO_IKE:
                return "PROTO_IKE";
            default:
                return intToString( protocol );
        }
    }

    string Enums::POLICY_TYPE_STR( POLICY_TYPE policy_type ) {
        switch ( policy_type ) {
            case Enums::POLICY_MAIN:
                return "MAIN";
            case Enums::POLICY_SUB:
                return "SUB";
            default:
                return intToString( policy_type );
        }
    }

    string Enums::POLICY_ACTION_STR( POLICY_ACTION policy_action ) {
        switch ( policy_action ) {
            case Enums::POLICY_ALLOW:
                return "ALLOW";
            case Enums::POLICY_BLOCK:
                return "BLOCK";
            default:
                return intToString( policy_action );
        }
    }

    string Enums::ATTRIBUTE_TYPE_STR( ATTRIBUTE_TYPE attr_type ) {
        switch ( attr_type ) {
            case Enums::ATTR_KEY_LEN:
                return "ATTR_KEY_LEN";
            default:
                return intToString( attr_type );
        }
    }

    string Enums::TRANSFORM_TYPE_STR( TRANSFORM_TYPE transform_type ) {
        switch ( transform_type ) {
            case Enums::ENCR:
                return "ENCR";
            case Enums::INTEG:
                return "INTEG";
            case Enums::D_H:
                return "DH";
            case Enums::PRF:
                return "PRF";
            case Enums::ESN:
                return "ESN";
            default:
                return intToString( transform_type );
        }
    }

    string Enums::ENCR_ID_STR( ENCR_ID encr_id ) {
        switch ( encr_id ) {
            case Enums::ENCR_3DES:
                return "ENCR_3DES";
            case Enums::ENCR_3IDEA:
                return "ENCR_3IDEA";
            case Enums::ENCR_AES_CBC:
                return "ENCR_AES_CBC";
            case Enums::ENCR_AES_CTR:
                return "ENCR_AES_CTR";
            case Enums::ENCR_BLOWFISH:
                return "ENCR_BLOWFISH";
            case Enums::ENCR_CAST:
                return "ENCR_CAST";
            case Enums::ENCR_DES:
                return "ENCR_DES";
            case Enums::ENCR_DES_IV32:
                return "ENCR_DES_IV32";
            case Enums::ENCR_DES_IV64:
                return "ENCR_DES_IV64";
            case Enums::ENCR_IDEA:
                return "ENCR_IDEA";
            case Enums::ENCR_NONE:
                return "ENCR_NONE";
            case Enums::ENCR_NULL:
                return "ENCR_NULL";
            case Enums::ENCR_RC5:
                return "ENCR_RC5";
            default:
                return intToString( encr_id );
        }
    }

    string Enums::PRF_ID_STR( PRF_ID prf_id ) {
        switch ( prf_id ) {
            case Enums::PRF_AES128_CBC:
                return "PRF_AES128_CBC";
            case Enums::PRF_HMAC_MD5:
                return "PRF_HMAC_MD5";
            case Enums::PRF_HMAC_SHA1:
                return "PRF_HMAC_SHA1";
            case Enums::PRF_HMAC_TIGER:
                return "PRF_HMAC_TIGER";
            default:
                return intToString( prf_id );
        }
    }

    string Enums::INTEG_ID_STR( INTEG_ID integ_id ) {
        switch ( integ_id ) {
            case Enums::AUTH_AES_XCBC_96:
                return "AUTH_AES_XCBC_96";
            case Enums::AUTH_DES_MAC:
                return "AUTH_DES_MAC";
            case Enums::AUTH_HMAC_MD5_96:
                return "AUTH_HMAC_MD5_96";
            case Enums::AUTH_HMAC_SHA1_96:
                return "AUTH_HMAC_SHA1_96";
            case Enums::AUTH_KPDK_MD5:
                return "AUTH_KPDK_MD5";
            case Enums::AUTH_NONE:
                return "AUTH_NONE";
            default:
                return intToString( integ_id );
        }
    }

    string Enums::DH_ID_STR( DH_ID dh_id ) {
        switch ( dh_id ) {
            default:
                return intToString( dh_id );
        }
    }

    string Enums::ESN_ID_STR( ESN_ID esn_id ) {
        switch ( esn_id ) {
            case Enums::ESN_YES:
                return "ESN_YES";
            case Enums::ESN_NO:
                return "ESN_NO";
            default:
                return intToString( esn_id );
        }
    }


    string Enums::DIRECTION_STR( DIRECTION dir ) {
        switch ( dir ) {
            case Enums::DIR_IN:
                return "DIR_IN";
            case Enums::DIR_FWD:
                return "DIR_FWD";
            case Enums::DIR_OUT:
                return "DIR_OUT";
            default:
                return intToString( dir );
        }
    }

    string Enums::ROLE_ID_STR( ROLE_ID role_id ) {
        switch ( role_id ) {
            case Enums::ROLE_ANY:
                return "ROLE_ANY";
            case Enums::ROLE_INITIATOR:
                return "ROLE_INITIATOR";
            case Enums::ROLE_RESPONDER:
                return "ROLE_RESPONDER";
            default:
                return intToString( role_id );
        }
    }

    string Enums::ID_TYPE_STR( ID_TYPE id_type ) {
        switch ( id_type ) {
            case Enums::ID_DER_ASN1_DN:
                return "ID_DER_ASN1_DN";
            case Enums::ID_DER_ASN1_GN:
                return "ID_DER_ASN1_GN";
            case Enums::ID_FQDN:
                return "ID_FQDN";
            case Enums::ID_IPV4_ADDR:
                return "ID_IPV4_ADDR";
            case Enums::ID_IPV6_ADDR:
                return "ID_IPV6_ADDR";
            case Enums::ID_KEY_ID:
                return "ID_KEY_ID";
            case Enums::ID_RFC822_ADDR:
                return "ID_RFC822_ADDR";
            default:
                return intToString( id_type );
        }
    }

    string Enums::IP_PROTO_STR( uint8_t ip_proto ) {
        switch ( ip_proto ) {
            case IP_PROTO_ANY:
                return "ANY";
            case IP_PROTO_ICMP:
                return "ICMP";
            case IP_PROTO_TCP:
                return "TCP";
            case IP_PROTO_UDP:
                return "UDP";
            case IP_PROTO_ICMPv6:
                return "ICMPv6";
            case IP_PROTO_MH:
                return "MH";
            default:
                return intToString( ip_proto );
        }
    }

    string Enums::IPSEC_MODE_STR( IPSEC_MODE mode ) {
        switch ( mode ) {
            case Enums::TRANSPORT_MODE:
                return "TRANSPORT_MODE";
            case Enums::TUNNEL_MODE:
                return "TUNNEL_MODE";
            default:
                return intToString( mode );
        }
    }

    string Enums::CERT_ENCODING_STR( uint16_t encoding ) {
        switch ( encoding ) {
            case Enums::CERT_ARL:
                return "CERT_ARL";
            case Enums::CERT_CRL:
                return "CERT_CRL";
            case Enums::CERT_DNS:
                return "CERT_DNS";
            case Enums::CERT_HASH_URL:
                return "CERT_HASH_URL";
            case Enums::CERT_HASH_URL_BUNDLE:
                return "CERT_HASH_URL_BUNDLE";
            case Enums::CERT_KERBEROS:
                return "CERT_KERBEROS";
            case Enums::CERT_PGP:
                return "CERT_PGP";
            case Enums::CERT_X509_PKCS_7:
                return "CERT_X509_PKCS_7";
            case Enums::CERT_RAW_RSA:
                return "CERT_RAW_RSA";
            case Enums::CERT_RESERVED:
                return "CERT_RESERVED";
            case Enums::CERT_SPKI:
                return "CERT_SPKI";
            case Enums::CERT_X509_ATTRIBUTE:
                return "CERT_X509_ATTRIBUTE";
            case Enums::CERT_X509_SIGNATURE:
                return "CERT_X509_SIGNATURE";
            default:
                return intToString( encoding );
        }
    }

    string Enums::AUTH_METHOD_STR( AUTH_METHOD auth_method ) {
        switch ( auth_method ) {
            case Enums::AUTH_METHOD_NONE:
                return "AUTH_METHOD_NONE";
            case Enums::AUTH_METHOD_PSK:
                return "AUTH_METHOD_PSK";
            case Enums::AUTH_METHOD_RSA:
                return "AUTH_METHOD_RSA";
            case Enums::AUTH_METHOD_DSS:
                return "AUTH_METHOD_DSS";
            default:
                return intToString( auth_method );
        }
    }
}

