#include "DocList.h"
#include <iostream>

using namespace std;

DocList::DocList()
{
    this->num = 0;
    //this->docs.reserve(10);
}

DocList::~DocList()
{
    //dtor
}

void DocList::addDoc(Doc doc)
{
    this->num += doc.getNum();
    this->docs.push_back(doc);
}

void DocList::print()
{
    cout<<"Appearances           : "<<this->num<<endl;
    cout<<"Documents               "<<endl;
    cout<<"--------------------------------"<<endl;
    for (int i=0; i<this->docs.size(); i++)
    {
        this->docs[i].print();
    }
}
