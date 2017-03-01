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
