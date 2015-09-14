#ifndef UTILS_H
#define UTILS_H

#include <map>
#include <string>
#include "DocList.h"

class Utils
{
    public:
        Utils();
        virtual ~Utils();
        int getSystemThreads();
        bool getFilePath();
        //void readFile(char* filename, int file_start, int file_end, std::map<std::string, DocList> *index);
        void createIndex(int n_threads, std::map<std::string, DocList> *index, char* filename);
        int getNumberOfLines(char* filename);
        void printDocsCounters();
        void submitQuery(std::string query, std::map<std::string, DocList>* index);
    protected:
    private:
};

#endif // UTILS_H
