#ifndef ACTIVECONNECTIONS_H_
#define ACTIVECONNECTIONS_H_

#include <vector>
#include <iostream>
#include "SingleActive.h"

using namespace std;

/**
 *
 */
class ActiveConnections
{
    private: 
	vector<SingleActive> uploads;
	vector<SingleActive> downloads;
	
	unsigned int maxUploads;
	unsigned int maxDownloads;
	
    public:
	ActiveConnections( unsigned int max_uploads, unsigned int max_downloads );
	bool addUpload( SingleActive upload );
	bool addDownload( SingleActive download );
};

#endif /*ACTIVECONNECTIONS_H_*/
