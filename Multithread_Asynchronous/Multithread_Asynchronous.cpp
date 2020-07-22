#include <future>
#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <list>
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
vector<string> fileVector;

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

int do_work(int x) {
    x++;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return x;
}

void do_work_parallel() {
    int xy = 3;
    auto f1 = std::async([]() { do_work(xy); });
    auto f2 = std::async([]() { do_work(xy); });
    do_work(xy);

    f1.get();
    f2.get();
}

void do_work_sequential() {
    do_work(3);
    do_work(3);
    do_work(3);
}

int main() { do_work_sequential(); }