#include "PeersMessage.h"

PeersMessage::PeersMessage()
{
    length = 0;
    messageId = 0;
}

PeersMessage::PeersMessage( unsigned char messageId, unsigned int length )
{
    this->messageId = messageId;
    this->length = length;
}

PeersMessage::~PeersMessage()
{
}

unsigned int PeersMessage::getLength()
{
    return length;
}

void PeersMessage::setLength( unsigned int length )
{
    this->length = length;
}

unsigned char PeersMessage::getMessageId()
{
    return messageId;
}

void PeersMessage::setMessageId( unsigned char messageId )
{
    this->messageId = messageId;
}
