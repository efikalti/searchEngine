#include "Utils.h"
#include <thread>
#include <iostream>
#include <fstream>
#include <string>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

using namespace std;

Utils::Utils()
{
    //ctor
}

Utils::~Utils()
{
    //dtor
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
Reads the path that contains the documents and returns true if it exists.
*/
bool Utils::readFile(char* filename)
{
    ifstream file;
    file.open (filename, ios::in);

    if (!file.is_open())
    {
        cout << "ERROR:Could not open file " << filename << endl;
        return false;
    }
    string line;
    bool first = true;
    int lines;
    while ( getline (file,line) )
    {
        if (first)
        {
            lines =atoi( line.c_str() );
            cout << "lines: " << lines << endl;
            first = false;
        }
      cout << line << '\n';
    }

    file.close();

    return true;
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
                readFile(filename);
            }
        }
        delete [] filename;

        closedir (pDir);
        free(pDirent);

    return true;
}

