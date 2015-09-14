#ifndef DOC_H
#define DOC_H

class Doc
{
    public:
        Doc();
        Doc(int id);
        virtual ~Doc();
        int getID();
        int getNum();
        void setID( int id);
        void setNum(int num);
        void add();
        void print();
    protected:
    private:
        int ID;
        int num_of_app; //number of appearances in this doc
};

#endif // DOC_H
