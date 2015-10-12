#ifndef UTILS_H
#define UTILS_H

#include <map>
#include <iostream>
#include <string>
#include "DocList.h"

class Utils
{
    public:
        Utils();
        virtual ~Utils();
        int getSystemThreads(); // Returns the number of system threads.
        void createIndex(std::map<std::string, DocList> *index, char* filename); // Creates the inverted index.
        int getNumberOfLines(char* filename); // Returns the number of lines for a file.
        void printDocsCounters(); // Prints document counters.
        void submitQuery(std::string query, std::map<std::string, DocList>* index, int answers, std::ostream &output); // Reads and submits queries from file.
        void readQueries(char* filename, std::map<std::string, DocList>* index, std::string output); // Calculates the answer set for a query.
        void printIndexToFile(std::string filename, std::map<std::string, DocList>* w); // Print index to a given file.
        void printIndex(std::map<std::string, DocList>* w); // Prints index.
    protected:
    private:
};

#endif // UTILS_H
