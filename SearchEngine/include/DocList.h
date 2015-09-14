#ifndef DOCLIST_H
#define DOCLIST_H

#include <vector>
#include "Doc.h"

class DocList
{
    public:
        DocList();
        virtual ~DocList();
        void addDoc(Doc doc);
        void incrementDoc(int pos);
        void print();
        int searchDoc(int id);
        int getNum();
        int getDocNum(int pos);
        std::vector<Doc> getDocs();
    protected:
    private:
        int size;
        int pos;
        int num; //number of appearances in all docs
        std::vector<Doc> docs;
};

#endif // DOCLIST_H
