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
#include <algorithm>

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

/**
Print inverted index.
*/
void  Utils::printIndex(map<string, DocList> *w)
{
    cout<<"Index: "<<endl;
    map<std::string, DocList>::iterator it;
    for (it=w->begin(); it!=w->end(); ++it)
    {
        cout<<"Word                  : "<<it->first<<endl;
        it->second.print();
    }

}

/**
Print index to file.
*/
void Utils::printIndexToFile(string filename, map<string, DocList> *w)
{
      ofstream file;
      file.open (filename, ios::trunc);
      file<<"Index: "<<endl;
      map<std::string, DocList>::iterator it;
      for (it=w->begin(); it!=w->end(); ++it)
      {
        file<<"Word                  : "<<it->first<<endl;
        it->second.printTo(file);
      }
      file.close();
}

/**
Print the document counters, max frequency and number of words for each document.
*/
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
Returns the number of the system thread cores.
*/
int Utils::getSystemThreads()
{
    unsigned int n = std::thread::hardware_concurrency();
    return n;
}


/**
Reads the file with that filename,line by line.
It calculates the max frequency and the number of words for each document and saves them in the docs global map.
For each document content, splits the content to words,removes punctuation and inserts the word with the document id into the
inverted index.
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
    int counter = 0;
    while ( getline (file,line) )
    {
        if (counter > file_start && counter <= file_end)
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
                        string temp;
                        remove_copy_if(word.begin(), word.end(), back_inserter(temp), ptr_fun<int, int>(&std::ispunct));
                        word = temp;

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
Reads and returns the number of lines of input file.
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
        lines = atoi( line.c_str() );
        file.close();
        return lines;
    }
    return 0;
}

/**
Create, start the threads and create the index.
*/
void Utils::createIndex(map<string, DocList>* index, char* filename)
{
    int lines = getNumberOfLines(filename);
    int n_threads = getSystemThreads();
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
    int file_start = 0;
    int file_end = diff;
    for (int i=0; i<n_threads; i++)
    {
        threads[i] = thread(readFile, filename, file_start, file_end, index);
        threads[i].join();
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
Reads the query file, and submits each query to the system.
*/
void Utils::readQueries(char* filename, map<string, DocList>* index, string output)
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
    while ( getline (file,line) )
    {// Read first line containing the number of lines
        if (first)
        {
            first = false;
        }
        else
        {
            // Read each query and submit it.
            istringstream iss(line);
            bool first_token = true;
            bool second_token = false;
            string query = "";
            int answers;
            while (iss)
            {
                string word;
                iss >> word;
                if (first_token)
                {
                    first_token = false;
                    second_token = true;
                }
                else if (second_token)
                {
                    answers = atoi( word.c_str() );
                    second_token = false;
                }
                else
                {
                    if (!word.empty())
                    {
                        query = query + " " + word;
                    }
                }
            }
            if (output.compare("screen") == 0)
            {
                submitQuery(query, index, answers, cout);
            }
            else
            {
                ofstream output_file;
                output_file.open ("answers.txt", ios::app);
                submitQuery(query, index, answers, output_file);
                output_file.close();
            }
            query = "";
        }
    }
    file.close();
}

/**
Reads and calculates answer set for a query given the inverted index.
*/
void Utils::submitQuery(string query, map<string, DocList>* index,int answers, ostream &output)
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
            if (d_list.getNum() > 0)
            {
                word_docs = d_list.getDocs();
                for (unsigned int i=0; i<word_docs.size(); i++)
                {
                    // Calculate TF and IDF for each document
                    id = word_docs[i].getID();
                    words = docs[id][1];
                    max_freq = docs[id][0];
                    TF = (  (float) (word_docs[i].getNum()) / (float) (words) ) / ( (float) max_freq / (float) words );
                    IDF = log((float) (num_of_docs) / (float)(d_list.getNum()));
                    if (IDF == 0)
                    {
                        IDF = 1;
                    }
                    weight = TF * IDF;
                    answer_set[id] += weight;
                }
            }
        }
    }

    output<<"query:"<<query<<endl;
    output<<"answer set:"<<endl;
    map<float, vector<int>, greater<float>> sorted_answers;
    map<unsigned int, float>::iterator it;
    answers = min(answers, (int)answer_set.size());
    for (it=answer_set.begin(); it!=answer_set.end(); ++it)
    {
        sorted_answers[it->second].push_back(it->first);
    }
    map<float, vector<int>>::iterator it2;
    int j = 0;
    for (it2=sorted_answers.begin(); it2!=sorted_answers.end(); ++it2)
    {
        vector<int> ids = it2->second;
        for(unsigned int i=0; i<ids.size(); i++)
        {
            output<<"Doc: "<<ids[i]<<", value: "<<it2->first<<endl;
            j++;
            if (j >= answers)
            {
                break;
            }
        }
        if (j >= answers)
        {
            break;
        }
    }
    output<<"---------------------------------"<<endl;
}

