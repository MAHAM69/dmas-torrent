#ifndef CVSPARSER_H_
#define CVSPARSER_H_

#include <string>
#include <iostream>

using namespace std;

/**
 * @author: Alek Malaszkiewicz
 */
class CVSParser
{
private:
protected:
public:
    CVSParser();
    virtual ~CVSParser();
    void parse( string, string&, string& );
};

#endif /*CVSPARSER_H_*/
