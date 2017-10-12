/*===================================================================================*//**
	Message
	
	A message to be sent by the MessagingSystem.
    
    Copyright 2017 Erick Fernandez de Arteaga. All rights reserved.
        https://www.linkedin.com/in/erick-fda
        https://bitbucket.org/erick-fda

    @author Erick Fernandez de Arteaga
	@version 0.0.0
	@file
	
	@see Message
	@see Message.h
	
*//*====================================================================================*/

/*========================================================================================
	Dependencies
========================================================================================*/
#include "Message.h"

/*----------------------------------------------------------------------------------------
	Constructors and Destructors
----------------------------------------------------------------------------------------*/
///
///	Message constructor.
///
Message::Message(MESSAGE_TYPE type, bool isUrgent, BaseMessageContent* content_p):
	_type(type), _isUrgent(isUrgent), _content_p(content_p)
{
	//SDL_Log("New Message Created");
}

///
///	Default Message destructor.
///
Message::~Message()
{
	/* Check the type of this message and cast and delete the content accordingly. */
	if (_type == MESSAGE_TYPE::BaseMessageType)
	{
		BaseMessageContent* contentToDelete = static_cast<BaseMessageContent*>(_content_p);
		delete contentToDelete;
	}
	else if (_type == MESSAGE_TYPE::FileLoadMessageType)
	{
		FileLoadMessageContent* contentToDelete = static_cast<FileLoadMessageContent*>(_content_p);
		delete contentToDelete;
	}
	else if (_type == MESSAGE_TYPE::FileLoadedMessageType)
	{
		FileLoadedMessageContent* contentToDelete = static_cast<FileLoadedMessageContent*>(_content_p);
		delete contentToDelete;
	}
}

/*----------------------------------------------------------------------------------------
	Instance Getter Methods
----------------------------------------------------------------------------------------*/
///
///	Return the message's type.
///
MESSAGE_TYPE Message::getType()
{
	return _type;
}

///
///	Return whether the message is urgent.
///
bool Message::getIsUrgent()
{
	return _isUrgent;
}

///
///	Return a pointer to the message's content.
///
BaseMessageContent* Message::getContent()
{
	return _content_p;
}

/*----------------------------------------------------------------------------------------
	Instance Setter Methods
----------------------------------------------------------------------------------------*/
///
///	Sets the message's content.
/// This will delete the old content's memory!
///
void Message::setContent(BaseMessageContent* newContent)
{
	delete _content_p;
	_content_p = newContent;
}

/*----------------------------------------------------------------------------------------
	Instance Methods
----------------------------------------------------------------------------------------*/

