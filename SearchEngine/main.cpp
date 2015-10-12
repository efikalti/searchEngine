#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <map>
#include <sys/time.h>
#include <sys/resource.h>

#include "Utils.h"

using namespace std;

void printWordIndex(map<string, DocList> w);

int main(int argc, char* argv[])
{
    // Output choice
    string output;
    cout<<"Do you want the output results on screen or file?(screen/file)";
    cin>>output;
    while(output.compare("file") != 0 && output.compare("screen") != 0)
    {
        cout<<"Type file to save the results in file or screen for printing the results on screen."<<endl;
        cin>>output;
    }

    // class Utils contains all the functions for building the index and answering queries.
    Utils u;
    // The inverted index.
    map <string, DocList> word_index;

    // structs to calculate time usage.
    struct rusage usage;
    struct timeval start, end;

    getrusage(RUSAGE_SELF, &usage);
    start = usage.ru_utime;
    // Build Index.
    cout<<"Building index..."<<endl;
    u.createIndex(&word_index, argv[1]);
    cout<<"Done building the index."<<endl<<endl;
    // Building of Index finished.
    getrusage(RUSAGE_SELF, &usage);
    end = usage.ru_utime;

    printf("Started building Inverted Index at : %ld.%lds\n", start.tv_sec, start.tv_usec);
    printf("Finished at                        : %ld.%lds\n", end.tv_sec, end.tv_usec);

    if (output.compare("screen") == 0)
    {
        u.printIndex(&word_index);
    }
    else
    {
        u.printIndexToFile("index.txt", &word_index);
    }

    getrusage(RUSAGE_SELF, &usage);
    start = usage.ru_utime;
    // Read and submit the queries
    cout<<"Processing queries..."<<endl<<endl;
    u.readQueries(argv[2], &word_index, output);

    getrusage(RUSAGE_SELF, &usage);
    end = usage.ru_utime;

    printf("Started submitting the queries at  : %ld.%lds\n", start.tv_sec, start.tv_usec);
    printf("Finished at                        : %ld.%lds\n", end.tv_sec, end.tv_usec);


    return 0;
}

void printWordIndex(map<string, DocList> w)
{
    map<std::string, DocList>::iterator it;
    for (it=w.begin(); it!=w.end(); ++it)
    {
        cout<<"Word                  : "<<it->first<<endl;
        it->second.print();
    }

}
