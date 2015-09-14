#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <map>
#include <sys/time.h>
#include <sys/resource.h>

#include "Utils.h"
#include "Doc.h"
#include "DocList.h"

using namespace std;

void printWordIndex(map<string, DocList> w);

int main(int argc, char* argv[])
{
    Utils u;
    struct rusage usage;
    struct timeval start, end;
    map <string, DocList> word_index;

    int num_of_threads = u.getSystemThreads();
    cout << "cpus: " << num_of_threads << endl;

    getrusage(RUSAGE_SELF, &usage);
    start = usage.ru_stime;
    // Build Index.
    u.createIndex(num_of_threads, &word_index, argv[1]);
    // Building of Index finished.
    getrusage(RUSAGE_SELF, &usage);
    end = usage.ru_stime;

    printf("Started building Inverted Index at : %ld.%lds\n", start.tv_sec, start.tv_usec);
    printf("Ended at                           : %ld.%lds\n", end.tv_sec, end.tv_usec);


    printWordIndex(word_index);
    u.printDocsCounters();


    u.submitQuery("efi is great!", &word_index);


    map<string, int> doc;

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
