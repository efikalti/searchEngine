#include "Utils.h"
#include <thread>

Utils::Utils()
{
    //ctor
}

Utils::~Utils()
{
    //dtor
}

int Utils::getSystemThreads()
{
    unsigned int n = std::thread::hardware_concurrency();
    return n;
}
