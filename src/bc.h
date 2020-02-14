#ifndef BC_H
#define BC_H
#include "ds.h"
#include <bits/stdc++.h>
#define uint unsigned int
using namespace std;

class Choice{
    public:
    Choice():first(0), second(0), D(0.0){};
    ~Choice();
    Cell *first;
    Cell *second;
    double D;
};

struct cmp{
    bool operator()(Choice const *a, Choice const *b ){
        return a->D < b->D;
    }
}; 

class BC {
public:
    BC();
    BC(Cells *cells, Nets *nets){
        this->cells = cells;
        this->nets = nets;
    };
    ~BC();
    void cluster();
    void compute_nbr(Cell *tmpCellU); // compute scores of nbr of cell u
    void output(double, string, string);
    Cells *cells;
    Nets  *nets;
    priority_queue<Choice*, vector<Choice*>, cmp> PQ; 
    uint A_total, Nc, Level;
    vector<string> clusters_File;
    vector<string> cnets_File;
    vector< vector<string> > tot_cnets;
    string cnet;  
}; 
#endif
