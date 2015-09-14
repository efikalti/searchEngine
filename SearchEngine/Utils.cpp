#include "Utils.h"
#include <thread>
#include <iostream>
#include <fstream>
#include <string>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <mutex>
#include <math.h>

using namespace std;

mutex insert_mutex;
map<unsigned int, int[2]> docs;
int num_of_docs;

Utils::Utils()
{
    //ctor
}

Utils::~Utils()
{
    //dtor
}

void printIndex(map<string, DocList> w)
{
    cout<<"Index: "<<endl;
    map<std::string, DocList>::iterator it;
    for (it=w.begin(); it!=w.end(); ++it)
    {
        cout<<"Word                  : "<<it->first<<endl;
        it->second.print();
    }

}

void Utils::printDocsCounters()
{
    cout<<"Docs: "<<endl;
    map<unsigned int, int[2]>::iterator it;
    for (it=docs.begin(); it!=docs.end(); ++it)
    {
        cout<<"Doc : "<<it->first<<" ,max frequency: "<<it->second[0]<<" ,words: "<<it->second[1]<<endl;
    }

}

/**
Returns the number of the system thread cores
*/
int Utils::getSystemThreads()
{
    unsigned int n = std::thread::hardware_concurrency();
    return n;
}


/**
Reads the file with that filename,line by line.
*/
void readFile(char* filename, int file_start, int file_end, map<string, DocList> *index)
{
    ifstream file;
    file.open (filename, ios::in);

    if (!file.is_open())
    {
        cout << "ERROR:Could not open file " << filename << endl;
        return;
    }
    string line;
    bool first = true;
    int counter = 0;
    while ( getline (file,line) )
    {
        if (first)
        {// Ignore first line
            first = false;
        }
        else if (counter > file_start && counter <= file_end)
        {// Read only the lines from file_start to file_end
            // From each line, the first word is the document id and the rest the content of the document.
            istringstream iss(line);
            bool first_token = true;
            unsigned int doc_id;
            std::map<string, DocList>::iterator it;
            Doc doc = Doc();
            DocList d_list;
            int max_tf = 0;
            int words = 0;
            while (iss)
                {
                    string word;
                    iss >> word;
                    if (first_token)
                    { // Read document id.
                        doc_id = atoi(word.c_str());
                        first_token = false;
                    }
                    else if (!word.empty())
                    { // Insert word in inverted index.
                        //Search if word exists
                        if ( (*index)[word].getNum() == 0 )
                        {
                          // Word does not exist in index, add it.
                          // Create document id entry.
                          doc.setID(doc_id);
                          doc.setNum(1);
                          // Create DocList and add the newly created Doc item.
                          d_list = DocList();
                          d_list.addDoc(doc);
                          // Add DocList in index for this word.
                          insert_mutex.lock();
                          (*index)[word] = d_list;
                          insert_mutex.unlock();

                          if (max_tf < 1)
                          {
                              max_tf = 1;
                          }
                        }
                        else
                        {
                          // Word exists in index, retrieve its' DocList.
                          d_list = (*index)[word];
                          int pos = d_list.searchDoc(doc_id);
                          if (pos == -1)
                          {
                              // Document with this id does not exist in the DocList of this word.
                              doc.setID(doc_id);
                              doc.setNum(1);
                              d_list.addDoc(doc);
                              if (max_tf < 1)
                              {
                                  max_tf = 1;
                              }
                          }
                          else
                          {
                              d_list.incrementDoc(pos);
                              int m = d_list.getDocNum(pos);
                              if (m > max_tf)
                              {
                                  max_tf = m;
                              }
                          }
                          // Update DocList
                          insert_mutex.lock();
                          (*index)[word] = d_list;
                          insert_mutex.unlock();
                        }
                        words++;
                    }
                }
            docs[doc_id][0] = max_tf;
            docs[doc_id][1] = words;
        }
        else if (counter > file_end)
        {// Close file and return.
            file.close();
            return;
        }
        counter++;
    }
    file.close();
    return;
}

/**
Reads the number of lines of input file.
*/
int Utils::getNumberOfLines(char* filename)
{
    ifstream file;
    file.open (filename, ios::in);

    if (!file.is_open())
    {
        cout << "ERROR:Could not open file " << filename << endl;
        return 0;
    }
    string line;
    int lines;
    while ( getline (file,line) )
    {// Read first line containing the number of lines
        lines =atoi( line.c_str() );
        file.close();
        return lines;
    }
    return 0;
}
void something()
{

}

/**
Create, start the threads and create the index.
*/
void Utils::createIndex(int n_threads, map<string, DocList>* index, char* filename)
{
    int lines = getNumberOfLines(filename);
    num_of_docs = lines;
    int diff;
    if (lines <= n_threads)
    {
        diff = 1;
        n_threads = lines;
    }
    else
    {
        diff = lines / n_threads;
    }
    thread threads[n_threads];

    cout<<n_threads<< " threads created.\n";
    int file_start = 0;
    int file_end = diff;
    for (int i=0; i<n_threads; i++)
    {
        threads[i] = thread(readFile, filename, file_start, file_end, index);
        threads[i].join();
        cout<<"Thread: "<<i+1<<" for line "<<file_start<<" to "<<file_end<<endl;
        if (file_end < lines)
        {
            file_start += diff;
            if (lines <  file_end + diff)
            {
                file_end = lines - file_end;
            }
            else
            {
                file_end += diff;
            }
        }
    }
}

/**
Reads the path that contains the documents and returns true if it exists.
*/
bool Utils::getFilePath()
{
    string path;
    cout << "Enter the path to the documents' directory:";
    cin >> path;

    struct dirent *pDirent;
        DIR *pDir;

        pDir = opendir (path.c_str());
        if (pDir == NULL) {
                cout << "ERROR: Cannot open directory " << path << endl;
                return false;
        }

        char *filename = new char[100];
        while ((pDirent = readdir(pDir)) != NULL) {
            char* file = pDirent->d_name;
            if ( strcmp(pDirent->d_name, ".") != 0 && strcmp(pDirent->d_name, "..") != 0)
            {
                cout << pDirent->d_name << endl;

                strcpy(filename, path.c_str());
                strcat(filename, "/");
                strcat(filename, file);
                //readFile(filename, 0, 0);
            }
        }
        delete [] filename;

        closedir (pDir);
        free(pDirent);

    return true;
}

/**
Reads and calculates answer for a query given the index.
*/
void Utils::submitQuery(string query, map<string, DocList>* index)
{
    map<unsigned int, float> answer_set;
    DocList d_list;
    vector<Doc> word_docs;
    float TF;
    float IDF;
    int words;
    int max_freq;
    int id;
    float weight;

    istringstream iss(query);
    while (iss)
    {
        string word;
        iss >> word;
        if (!word.empty())
        {
            d_list = (*index)[word];
            d_list.print();
            word_docs = d_list.getDocs();
            for (unsigned int i=0; i<word_docs.size(); i++)
            {
                // Calculate TF and IDF for each document
                id = word_docs[i].getID();
                words = docs[id][1];
                max_freq = docs[id][0];
                TF = (  (float) (word_docs[i].getNum()) / (float) (words) ) / ( (float) max_freq / (float) words );
                IDF = log( (float) (num_of_docs) / (float)(d_list.getNum()) );
                weight = TF * IDF;
                answer_set[id] += weight;
            }
        }
    }
    map<float, vector<int>> sorted_answers;
    map<unsigned int, float>::iterator it;
    for (it=answer_set.begin(); it!=answer_set.end(); ++it)
    {
        sorted_answers[it->second].push_back(it->first);
    }
    map<float, vector<int>>::iterator it2;
    for (it2=sorted_answers.begin(); it2!=sorted_answers.end(); ++it2)
    {
        vector<int> ids = it2->second;
        for(unsigned int i=0; i<ids.size(); i++)
        {
            cout<<"Doc: "<<ids[i]<<", value: "<<it2->first<<endl;
        }
    }
}

