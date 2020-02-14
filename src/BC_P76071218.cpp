#include "read_file.h"
#include "bc.h"
#include "time.h"
using namespace std;

int main(int argc, char **argv ){
    Cells *cells = new Cells();     
    Nets  *nets  = new Nets;  
    ReadFile *readFile = new ReadFile(cells, nets);
    clock_t start,end;

    string node_file  = argv[1];
    string net_file   = argv[2];
    string cls_file   = argv[3];
    string cnet_file  = argv[4];

    cout << "Reading files...\n";
    readFile->read_file(node_file, net_file);
    start = clock();
    BC *bestchoice = new BC(cells, nets);
    bestchoice->cluster();
    end = clock();
    double cpu_time = ((double)end-(double)start) / CLOCKS_PER_SEC;
    bestchoice->output(cpu_time, cls_file, cnet_file);
    cout << "Finish! All files are generated.\n\n\n";
    return 0;
}
