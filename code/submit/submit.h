#ifndef SUBMIT_H
#define SUBMIT_H

#include <QObject>
#include <iostream>
using namespace std;
#include "../utils/tcp.h"
#include "../utils/util.h"



class submit : public QObject{
public:
	void init( int argc, char const *argv[] );
};




#endif //SUBMIT_H