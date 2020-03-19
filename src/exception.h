/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef EXCEPTION_H
#define EXCEPTION_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <exception>
#include <iostream>
#include <cstdlib>
#include <stdint.h>
#include <sstream>

#include "payload.h"

using namespace std;

namespace openikev2 {

    class Exception : public exception {
        protected:
            string message;         ///< Informative text explaining exception.
        public:
            Exception( string m ) {
                message = "Exception: " + m;
            }
            virtual const char* what() const throw() {
                return message.c_str();
            };
            virtual ~Exception() throw() {};
    };

    class NoConfigurationFoundException : public Exception {
        public:
            NoConfigurationFoundException( string m ) : Exception( "NoConfigurationFoundException: " + m ) {}
    };

    class DuplicatedAttributeException : public Exception {
        public:
            DuplicatedAttributeException( string m ) : Exception( "DuplicatedAttributeException: " + m ) {}
    };

    class FileSystemException : public Exception {
        public:
            FileSystemException( string m ) : Exception( "FileSystemException: " + m ) {}
    };

    class NetworkException : public Exception {
        public:
            NetworkException( string m ) : Exception( "NetworkException: " + m ) {}
    };

    class IpsecException : public Exception {
        public:
            IpsecException( string m ) : Exception( "IpsecException: " + m ) {}
    };

    class CipherException : public Exception {
        public:
            CipherException( string m ) : Exception( "CipherException: " + m ) {}
    };

    class BufferExceededException : public Exception {
        public:
            BufferExceededException( string m ) : Exception( "BufferExceededException: " + m ) {}
    };

    class MessageException : public Exception {
        public:
            MessageException( string m ) : Exception( "MessageException: " + m ) {}
    };

    class ParsingException : public MessageException {
        public:
            ParsingException( string m ) : MessageException( "ParsingException: " + m ) {}
    };

    class ProposalException : public MessageException {
        public:
            ProposalException( string m ) : MessageException( "ProposalException: " + m ) {}
    };

    class UnknownPayloadException : public MessageException {
        public:
            Payload::PAYLOAD_TYPE payload_type;
        public:
            UnknownPayloadException( string m, Payload::PAYLOAD_TYPE payload_type ) : MessageException( "UnknownPayloadException: " + m ), payload_type( payload_type ) {}
    };
};
#endif
