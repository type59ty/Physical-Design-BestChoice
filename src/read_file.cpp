#include <bits/stdc++.h>
#include <regex>
#include "read_file.h"
#include "ds.h"
using namespace std;

void ReadFile::read_file(string node_file, string net_file){
    ifstream IFS_node;
    ifstream IFS_net;
    IFS_node.open(node_file);
    IFS_net.open(net_file);
    string line = "";
    regex pat_net("NetDegree");
    regex pat_node("o[0-9]+");
    regex pat_ter("terminal");
    regex pat_nnodes("NumNodes");
    regex pat_nters("NumTerminals");
    regex pat_nnets("NumNets");
    regex pat_number_of_pins_on_this_net("NumPins");

    string dummy;
    string node_name;
    string net_name;
    int h,w;
    int num_pins;

    // read node
    while(getline(IFS_node,line)) {
        dummy.clear();
        istringstream token(line);
        if ((regex_search(line, pat_node)) and !(regex_search(line, pat_ter))) {
            Cell *cell = new Cell();//new cell
            token >> node_name >> h >> w;
            cell->node_name = node_name;
            cell->area = h*w;
            cells->Total_Size += cell->area;
            if (cell->area > cells->Max_Size) {
                cells->Max_Size = cell->area;
            }
            cells->CellArray.insert(pair<string, Cell* >(cell->node_name, cell ) ) ;
        } else if (regex_search(line, pat_nnodes)) {
            token >> dummy >> dummy >> cells->NumNodes;  
        } else if (regex_search(line, pat_nters)) {
            token >> dummy >> dummy >> cells->NumTerminals; 
        }
    }

    // read net
    while(getline(IFS_net,line)) {
        istringstream token(line);
        if (regex_search(line, pat_net)) {
            token >> dummy >> dummy >> num_pins >> net_name;
            Net *net = new Net;
            net->npins = num_pins;
            this->net_name = net_name;
            net->net_name = this->net_name;
            nets->NetArray[net->net_name] = net;
        } else if (regex_search(line, pat_node)) {
            token >> node_name;
            auto it = cells->CellArray.find(node_name);
            if (it != cells->CellArray.end()) {
                Cell *cell = cells->CellArray[node_name];//cell name
                cell->NetList.push_back(nets->NetArray[this->net_name]);
                vector<string> current_node;
                for (auto no : nets->NetArray[this->net_name]->CellList) {
                    current_node.push_back(no->node_name);
                }
                auto Nit = find(current_node.begin(), current_node.end(), node_name);
                if (Nit == current_node.end()) {
                    nets->NetArray[this->net_name]->CellList.push_back(cell); 
                }
            } else { // this node is a terminal
                continue;
            }
        } else if (regex_search(line, pat_nnets)) {
            token >> dummy >> dummy >> (nets->NumNets);
        } else if (regex_search(line, pat_number_of_pins_on_this_net)) {
            token >> dummy >> dummy >> (nets->NumPins);
        }
    }

    IFS_node.close();
    IFS_net.close();
}
