#ifndef RFILE
#define RFILE
#include <bits/stdc++.h>
#include "ds.h"
using namespace std;

class ReadFile{
public:
    ReadFile();
    ReadFile(Cells *cells, Nets *nets){
        this->cells = cells;
        this->nets = nets;
    };
    ~ReadFile();
    void read_file(string node_file, string net_file);
    Cells *cells;
    Nets  *nets;
    string net_name; 
};
#endif
