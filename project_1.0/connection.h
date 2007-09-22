#include <vector>
#include <iostream>

using namespace std;

class PeersConnected
{
public :
	// name of the peer that sent handshake
	char peerName[20];
	// am I choking connection
	bool amChoking;
	// am i interested
	bool amInterested;
	
	// peer is chocking
	bool peerChoking;
	// peer interested
	bool peerInterested;
	

	PeersConnected(char* peer_name)
	{
		strcpy(peerName, peer_name);
		// standard (after handshake) settings of a connection
		amChoking = true;
		amInterested = false;
		peerChoking = true;
		peerInterested = false;
	}
};

// class represents single active upload or download connection, listend in 'ActiveConnections'
class SingleActive
{
public :
	char peerName[20];
	
	SingleActive(char* peer_name)
	{
		strcpy(peerName, peer_name);
	}
};

class ActiveConnections
{
private: 
	
	vector<SingleActive> uploads;
	vector<SingleActive> downloads;
	
	unsigned int maxUploads;
	unsigned int maxDownloads;
	
public:
	ActiveConnections(unsigned int max_uploads, unsigned int max_downloads)
	{
		maxUploads = max_uploads;
		maxDownloads = max_downloads;
	}
	
	bool addUpload(SingleActive upload)
	{
		if(uploads.size() > maxUploads)
			return false;
		
		uploads.push_back(upload);		
		return true;
	}
	
	bool addDownload(SingleActive download)
	{
		if(downloads.size() > maxDownloads)
			return false;
		
		downloads.push_back(download);
		return true;
	}
};

