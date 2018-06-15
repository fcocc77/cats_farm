#ifndef SHARED_VARIABLES_H
#define SHARED_VARIABLES_H

#include <vector>
using namespace std;
#include "../utils/json.h"
using namespace nlohmann;



struct shared_variables{
    json server_display;
    json jobs;
    json servers;
    bool stopUpdate;

};



#endif //SHARED_VARIABLES_H