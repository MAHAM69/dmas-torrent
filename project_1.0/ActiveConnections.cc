#include "ActiveConnections.h"

ActiveConnections::ActiveConnections( unsigned int max_uploads, unsigned int max_downloads )
{
    this->maxUploads = max_uploads;
    this->maxDownloads = max_downloads;
}

bool ActiveConnections::addUpload( SingleActive upload )
{
    if(uploads.size() > maxUploads)
	return false;

    uploads.push_back( upload );
    return true;
}

bool ActiveConnections::addDownload( SingleActive download )
{
    if ( downloads.size() > maxDownloads )
	return false;

    downloads.push_back( download );
    return true;
}
