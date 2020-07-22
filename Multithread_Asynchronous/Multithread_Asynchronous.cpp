#include "ThreadPool.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <iterator>
#include <string>
#include <io.h>

using namespace std;

//textfile struct
class textfile {
public:
    string title;
    string body;

    textfile() {
        title = " ";
        body = " ";
    };
    textfile(const char* fileName) {
        this->title = fileName;
    }
};

list<textfile*> txtlist;
vector<char*> fileVector;

//get all .txt files in directory
//store at fileVector
void get_files_inDirectory(const string& _path, const string _filter) {
    string searching = _path + _filter;

    _finddata_t fd;
    long handle = _findfirst(searching.c_str(), &fd);

    if (handle == -1)
        return;

    int result = 0;
    do {
        fileVector.push_back(fd.name);
        result = _findnext(handle, &fd);
    } while (result != -1);

    _findclose(handle);
}

//load txt file
//store at txtlist
void loadTxt(const char* fileName) {
    ifstream in(fileName);
    string tmp = "";
    textfile* txtfile = new textfile(fileName);

    if (in.is_open()) {
        in >> txtfile->body;
    }
    txtlist.push_back(txtfile);
    in.close();
}


int main() { 
    int count = 0;
    ThreadPool pool(7);
    get_files_inDirectory("directory", ".txt");

    while (!fileVector.empty()) {
        pool.EnqueueJobs([]() {loadTxt(fileVector.back()); });
    }

    list<textfile*> ::iterator it;
    while (it != txtlist.end()) {
        textfile* tmp = *it;
        tmp = new textfile();
        cout << "Title : " << tmp->title << " ON" << endl;
        count++;
    }

    cout << "Total " << count << "was loaded" << endl;
    return 0;
}