#ifndef SUBMIT_HPP
#define SUBMIT_HPP

#include <QObject>
#include <iostream>
using namespace std;
#include "../utils/tcp.hpp"
#include <util.hpp>

class submit : public QObject
{
  public:
	void init(int argc, char const *argv[]);
};

#endif //SUBMIT_HPP