#include "FileEngine.h"
#include "Message.h"
#include "MessageTypes.h"
#include "FileHelper.h"
#include "MessagingSystem.h"

FileEngine::FileEngine()
{
	//we do all the hard work in start()
}

void FileEngine::start()
{
	//subscribe to messages
	subscribe(MESSAGE_TYPE::FileLoadMessageType);

	//start loop
	_isRunning = true;
	_thread_p = new std::thread(&FileEngine::loop, this);
}

FileEngine::~FileEngine()
{
	_isRunning = false;
	_thread_p->join();
	delete(_thread_p);

	unsubscribe(MESSAGE_TYPE::FileLoadMessageType);
}

void FileEngine::loop()
{
	//do setup on thread

	while (_isRunning)
	{
		for (int i = 0; i < CYCLE_MESSAGES_CONST; i++)
		{
			if (!_urgentMessageQueue.empty())
			{
				//handle urgent messages first
				_urgentMessageQueueMutex_p->lock();
				Message *msg = _urgentMessageQueue.front().get();
				HandleMessage(msg);
				_urgentMessageQueue.pop();
				_urgentMessageQueueMutex_p->unlock();

			}
			else if (!_messageQueue.empty())
			{
				//then non-urgent messages
				_messageQueueMutex_p->lock();
				Message *msg = _messageQueue.front().get();
				HandleMessage(msg);
				_messageQueue.pop();
				_messageQueueMutex_p->unlock();
			}
			else
			{
				//all empty, break
				break;
			}
		}		

		//avoid busywaiting!
		std::this_thread::sleep_for(std::chrono::milliseconds(CYCLE_DELAY_MS_CONST));
	}

	//do teardown on thread

}

void FileEngine::HandleMessage(Message *inBaseMessage)
{
	
	MESSAGE_TYPE contentType = inBaseMessage->getType();
	switch (contentType)
	{
		case MESSAGE_TYPE::FileLoadMessageType:
		{
			FileLoadMessageContent inMessage = *static_cast<FileLoadMessageContent*>(inBaseMessage->getContent()); //this seems safe
			HandleNormalMessage(inMessage);
			break;
		}
		case MESSAGE_TYPE::FileLoadImageMessageType:
		{
			FileLoadImageMessageContent inMessage = *static_cast<FileLoadImageMessageContent*>(inBaseMessage->getContent()); //this seems safe
			HandleImageMessage(inMessage);
			break;
		}
		default:
			SDL_Log("Filesystem: Received unknown message type");
			break;
	}
}

void FileEngine::HandleNormalMessage(FileLoadMessageContent inMessage)
{
	size_t hash = 0;
	std::string content = std::string();

	if (inMessage.relative)
	{
		hash = HashFilePath(inMessage.path, true);
		content = FileHelper::loadFileFromStringRelative(inMessage.path);
	}
	else
	{
		hash = HashFilePath(inMessage.path, false);
		content = FileHelper::loadFileFromString(inMessage.path);
	}

	FileLoadedMessageContent *outMessage = new FileLoadedMessageContent();

	outMessage->hash = hash;
	outMessage->content = content;
	outMessage->path = inMessage.path;
	outMessage->relative = inMessage.relative;

	MessagingSystem::instance().postMessage(std::make_shared<Message>(Message(MESSAGE_TYPE::FileLoadedMessageType, false, outMessage)));
}

void FileEngine::HandleImageMessage(FileLoadImageMessageContent inMessage)
{

}

size_t FileEngine::HashFilePath(std::string path, bool relative)
{
	std::string prefix;
	if (relative)
		prefix = "R|";
	else
		prefix = "A|";

	std::string strToHash = prefix + path;

	return std::hash<std::string>{}(strToHash);
}
