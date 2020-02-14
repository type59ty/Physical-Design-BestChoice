#ifndef DS
#define DS
#include <bits/stdc++.h>
#define uint unsigned int
using namespace std;
class Cell;
class Net;

class Cell{
public:
    Cell():cluster_name(20, ""), node_name(""), area(0), Valid(true) {};
    ~Cell();
    vector<string > cluster_name;
    string node_name;
    uint area;
    bool Valid;
    vector<Net*> NetList;
};
class Cells{
public:
    Cells(): NumNodes(0), NumTerminals(0), Max_Size(0), Total_Size(0){};
    ~Cells();
    int NumNodes;
    int NumTerminals;
    uint Max_Size;
    uint Total_Size;
    vector<string > TermiArray;
    map<string, Cell* > CellArray;
};

class Net{
public:
    int npins;
    string net_name;
    vector<Cell *> CellList;
    uint A, B;
};
class Nets{
public:
    int NumNets; 
    int NumPins; 
    map<string, Net* > NetArray;
};
#endif
