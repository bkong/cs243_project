#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {
    typedef vector<string> LINES;
    LINES l;
    char line[256];
    ifstream in("data/filerev.txt");
    while (in.getline(line, 256)) {
        l.push_back(line);
    }
    in.close();
    for (LINES::reverse_iterator j = l.rbegin(); j != l.rend(); j++) {
        cout << (*j) << endl;
    }
}

