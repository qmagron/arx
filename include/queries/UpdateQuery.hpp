#ifndef __Update_Query__
#define __Update_Query__

#include "GenericQuery.hpp"

class UpdateQuery : public GenericQuery {

private:


public:
    UpdateQuery(std::string);
    ~UpdateQuery() = default;
};

#endif