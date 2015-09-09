#include "Doc.h"
#include <iostream>

using namespace std;

Doc::Doc()
{
    this->num_of_app = 0;
}

Doc::~Doc()
{
    //dtor
}

Doc::Doc(int id)
{
    this->ID = id;
    this->num_of_app = 0;
}

int Doc::getID()
{
    return this->ID;
}

int Doc::getNum()
{
    return this->num_of_app;
}

void Doc::setID(int id)
{
    this->ID = id;
}

void Doc::add(int num)
{
    this->num_of_app += num;
}

void Doc::print()
{
    cout<<"Doc id                : "<<this->ID<<endl;
    cout<<"Number of appearences : "<<this->num_of_app<<endl;
    cout<<"--------------------------------"<<endl;
}
