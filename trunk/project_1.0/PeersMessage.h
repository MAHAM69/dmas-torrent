#ifndef PEERSMESSAGE_H_
#define PEERSMESSAGE_H_

class PeersMessage
{
    private:
	/** length of a message */
	unsigned int length;
    
	/** type of a message sent */
	unsigned char messageId;
    public:
	PeersMessage();
	PeersMessage( unsigned char, unsigned int );
	virtual ~PeersMessage();

	/** Getter for message length. */
	unsigned int getLength();
	/** Setter for message length. */
	void setLength( unsigned int length );

	/** Getter for message id. */
	unsigned char getMessageId();
	/** Setter for message id. */
	void setMessageId( unsigned char messageId );
};

#endif /*PEERSMESSAGE_H_*/
