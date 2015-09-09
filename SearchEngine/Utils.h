#ifndef UTILS_H
#define UTILS_H

class Utils
{
    public:
        Utils();
        virtual ~Utils();
        int getSystemThreads();
        bool getFilePath();
        bool readFile(char* filename);
    protected:
    private:
};

#endif // UTILS_H
