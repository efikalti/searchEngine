#ifndef DOCLIST_H
#define DOCLIST_H

#include <vector>
#include <iostream>
#include "Doc.h"

class DocList
{
    public:
        DocList(); //constructor
        virtual ~DocList();
        void addDoc(Doc doc); // add document in the list
        void incrementDoc(int pos); // increment the number of appearances for the document in this position
        void print(); // Print all the information for this word
        int searchDoc(int id); // Search for the document with this id in the list
        int getNum(); // Return number of appearances
        int getDocNum(int pos); // Return the document in this position
        std::vector<Doc> getDocs(); // Return list with documents
        void printTo(std::ostream &stream); // Print to specific stream
    protected:
    private:
        int num; //number of appearances, one for each document
        std::vector<Doc> docs; // List with documents
};

#endif // DOCLIST_H
