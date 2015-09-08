#include <iostream>
#include <string>
#include "Utils.h"

using namespace std;

int main()
{
    Utils u;
    int num_of_threads = u.getSystemThreads();
    cout << num_of_threads << endl;

    if (u.getFilePath())
    {
        cout << "path success.";
    }

    return 0;
}
