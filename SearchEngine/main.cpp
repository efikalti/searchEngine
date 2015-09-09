#include <iostream>
#include <string>
#include <map>
#include "Utils.h"
#include "Doc.h"
#include "DocList.h"

using namespace std;

void printWordIndex(map<string, DocList> w);

int main(int argc, char* argv[])
{
    Utils u;
    map <string, DocList> word_index;

    int num_of_threads = u.getSystemThreads();
    cout << num_of_threads << endl;

    if (u.readFile(argv[1]))
    {
        cout << "path success."<<endl;
    }

    Doc a = Doc(1);
    Doc b = Doc(2);
    a.add(5);
    DocList l = DocList();
    l.addDoc(a);
    l.addDoc(b);

    word_index["one"] = l;
    printWordIndex(word_index);

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
