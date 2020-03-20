/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
 ***************************************************************************/
#ifndef OPENIKEV2MESSAGERECEIVEDCOMMAND_H
#define OPENIKEV2MESSAGERECEIVEDCOMMAND_H

#include "command.h"
#include "message.h"

namespace openikev2 {

    /**
        This class represents a Message Received Command, indicating the need of process a received Message
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class MessageReceivedCommand : public Command {

            /****************************** ATTRIBUTES ******************************/
        protected:
            auto_ptr<Message> message;                  /**< Received message */

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new MessageReceivedCommand setting the received message
             * @param message Received message
             */
            MessageReceivedCommand( auto_ptr<Message> message );

            virtual IkeSa::IKE_SA_ACTION executeCommand( IkeSa& ike_sa );
            virtual string getCommandName() const;

            virtual ~MessageReceivedCommand();

    };

}

#endif
