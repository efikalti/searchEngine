#ifndef DOC_H
#define DOC_H
#include <iostream>

class Doc
{
    public:
        Doc(); // Constructor
        Doc(int id); // Constructor with id
        virtual ~Doc();
        int getID(); // Return id of this Document
        int getNum(); // Return number of appearances in this Document
        void setID( int id); // Set id for this Document
        void setNum(int num); // Set number of appearances for this Document
        void add(); // Increase number of appearances for this Document by 1
        void print(); // Print all Document information
        void printTo(std::ostream &stream); // Print to specific stream
    protected:
    private:
        int ID; // ID of document
        int num_of_app; //number of appearances in this document
};

#endif // DOC_H
