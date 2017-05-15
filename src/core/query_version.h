#ifndef _QUERY_VERSION_H_
#define _QUERY_VERSION_H_


#include "comm/olup.h"


class QueryVersion
{
    public:
        QueryVersion();
        ~QueryVersion();

    public:
        int process(OLUP &olup);

    private:
        
};

#endif


