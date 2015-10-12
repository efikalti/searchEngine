#include "DocList.h"
#include <iostream>

using namespace std;

DocList::DocList()
{
    this->num = 0;
}

DocList::~DocList()
{
    //dtor
}

int DocList::getNum()
{
    return this->num;
}

void DocList::addDoc(Doc doc)
{
    this->num += doc.getNum();
    this->docs.push_back(doc);
}

void DocList::incrementDoc(int pos)
{
    this->docs[pos].add();
}

int DocList::searchDoc(int id)
{
    for (unsigned int i=0; i<this->docs.size(); i++)
    {
        if (this->docs[i].getID() == id)
        return i;
    }
    return -1;
}

int DocList::getDocNum(int pos)
{
    return this->docs[pos].getNum();
}

vector<Doc> DocList::getDocs()
{
    return this->docs;
}

void DocList::print()
{
    cout<<"Appearances           : "<<this->num<<endl;
    cout<<"Documents               "<<endl;
    cout<<"--------------------------------"<<endl;
    for (unsigned int i=0; i<this->docs.size(); i++)
    {
        this->docs[i].print();
    }
}

void DocList::printTo(ostream &stream)
{
    stream<<"Appearances           : "<<this->num<<endl;
    stream<<"Documents               "<<endl;
    stream<<"--------------------------------"<<endl;
    for (unsigned int i=0; i<this->docs.size(); i++)
    {
        this->docs[i].printTo(stream);
    }
}
