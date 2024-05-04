// Author: Francisco Contreras
// Office: VFX Artist - Senior Compositor
// Website: videovina.com

#ifndef SUBMIT_HPP
#define SUBMIT_HPP

#include <QObject>
#include <iostream>
using namespace std;
#include "../utils/tcp.h"
#include <util.h>

class submit : public QObject
{
public:
    void init(int argc, char const *argv[]);
};

#endif // SUBMIT_HPP
