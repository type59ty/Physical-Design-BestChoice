#include "bc.h"

void BC::output(double cpu_time, string cls_file, string cnet_file) {
    //string path = "output/";
    string path = "BC-HW/BC_P76071218/";
    ofstream OFS_cls;
    OFS_cls.open(path+cls_file);
    ofstream OFS_cnet;
    OFS_cnet.open(path+cnet_file);
    OFS_cls << "Time: " << cpu_time << " s" <<endl;
    OFS_cls << endl;
    for (auto line : clusters_File) {
        OFS_cls << line << endl;
    }
    for (auto cnets : tot_cnets) {
        for (auto line : cnets) {
            OFS_cnet << line << endl;
        }
    }
    OFS_cls.close();
    OFS_cnet.close();
}

void BC::compute_nbr(Cell *u){ // compute d of cell u's nbr 
    map<string, double > score_v;
    for (vector<int>::size_type i = 0 ; i < u->NetList.size() ; ++i) {
        Net *tmpNet = u->NetList[i];
        for (vector<int>::size_type j = 0 ; j < tmpNet->CellList.size() ; ++j) {
            Cell *tmpCellV = tmpNet->CellList[j];
            if (tmpCellV->node_name != u->node_name ) {
                if (!score_v.empty() ){
                    map<string, double >::iterator NIt = score_v.find(tmpCellV->node_name);
                    if ( NIt != score_v.end() ) {
                        NIt->second += 1.0 / double(tmpNet->CellList.size() - 1 );
                    } else {
                        score_v.insert(pair<string, double>(tmpCellV->node_name, 1.0 / double(tmpNet->CellList.size() - 1 ))) ;
                    }            
                } else {
                    score_v.insert(pair<string, double>(tmpCellV->node_name, 1.0 / double(tmpNet->CellList.size() - 1 )));
                }
            }
        }
    }
    if (!score_v.empty()) {
        string tmpCellname = "";
        double max_D = 0.0;
        double v_D = 0.0;
        for (auto NIt=score_v.begin(); NIt != score_v.end(); ++NIt) {
            map<string, Cell* >::iterator cellV = cells->CellArray.find(NIt->first);
            v_D = (NIt->second) / (double(u->area) + double(cellV->second->area));
            if(v_D > max_D){
                tmpCellname = cellV->second->node_name;
                max_D = v_D;
            }
        }
        map<string, Cell* >::iterator cellV = cells->CellArray.find(tmpCellname);//find max V
        Choice *choosen = new Choice();
        choosen->first = u;
        choosen->second = cellV->second;
        choosen->D = max_D;
        PQ.push(choosen); 
    }
}

void BC::cluster() {
    A_total = cells->Total_Size;
    cout << "A_total: " << A_total;
    //# of clustered blocks in level-1;
    Nc = cells->NumNodes - cells->NumTerminals; 
    uint pre_Nc;
    //Nc = cells->NumNodes; 
    cnet = "n";
    uint Nmax = 6000;
    Level = 0;
    
    for(auto it = cells->CellArray.begin(); it != cells->CellArray.end(); ++it){
        Cell *u = it->second;
        compute_nbr(u);
    }
    cout << "" << endl;
    cout << "Clustered blocks (Nc): " << Nc << endl;
    cout << "Nmax: " << Nmax << endl;
    uint counter = 0;
    string last = "";
    while (Nc > Nmax) {
        pre_Nc = Nc;
        uint levelNode = pre_Nc * 0.5;
        cout << "Level: " << Level << endl; 
        while (!PQ.empty()) {
            if (Nc == levelNode || Nc < Nmax) break;
            Choice *choosen = PQ.top();
            PQ.pop();
            if (choosen->first->Valid == false) {
                choosen->first->Valid = true;
                compute_nbr(choosen->first);
            } else if (cells->CellArray.find(choosen->first->node_name)  != cells->CellArray.end() &&
                       cells->CellArray.find(choosen->second->node_name) != cells->CellArray.end()) {       
                Cell *clusterCell = new Cell();
                uint Ac = 0;
                Ac = choosen->first->area + choosen->second->area;
                if (Ac <= 3 * (double)A_total / (double)pre_Nc) {
                    --Nc; 
                    clusterCell->node_name = choosen->first->node_name +" "+ choosen->second->node_name;
                    clusterCell->area = Ac; 
                    map<string, Net* > ClusterNetArray;
                    for (auto ne : choosen->first->NetList) {
                        Net *tmpNet = ne;
                        if (ClusterNetArray.find(tmpNet->net_name) == ClusterNetArray.end()) {
                            ClusterNetArray.insert(pair<string, Net* >(tmpNet->net_name, tmpNet));
                        }
                    }
                    if (choosen->first->node_name != choosen->second->node_name){
                        for (auto ne : choosen->second->NetList) {
                            Net *tmpNet = ne;
                            if (ClusterNetArray.find(tmpNet->net_name) == ClusterNetArray.end()){
                                ClusterNetArray.insert(pair<string, Net* >(tmpNet->net_name, tmpNet));
                            }
                        }        
                    }
                    vector<Net *> tmp;
                    for (auto it = ClusterNetArray.begin() ; it != ClusterNetArray.end() ; ++it) {
                        Net *tmpNet = it->second; // cells in the clustered net
                        bool flag = false;
                        while (!flag) {
                            flag = true;
                            for (vector<Cell *>::iterator j = tmpNet->CellList.begin() ; j != tmpNet->CellList.end() ; ++j) {
                                if (( (*j)->node_name == choosen->first->node_name) || ( (*j)->node_name == choosen->second->node_name) ){
                                    tmpNet->CellList.erase(j); 
                                    flag = false;
                                    break;              
                                } else
                                    (*j)->Valid = false;
                            }            
                        }
                        if (tmpNet->CellList.empty())
                             tmp.push_back(tmpNet);
                        else
                            tmpNet->CellList.push_back(clusterCell);
                    }     
                    for (vector<int>::size_type i = 0 ; i < tmp.size() ; ++i) {
                        ClusterNetArray.erase(tmp[i]->net_name);
                        nets->NetArray.erase(tmp[i]->net_name);
                        nets->NumPins -= tmp[i]->npins;
                    }          
                    for (map<string, Net* >::iterator it = ClusterNetArray.begin() ; it != ClusterNetArray.end() ; ++it)
                        clusterCell->NetList.push_back(it->second);
                    cells->CellArray.erase(choosen->first->node_name);
                    cells->CellArray.erase(choosen->second->node_name);     
                    if (Level > 0) {
                        clusterCell->cluster_name[Level-1] = choosen->first->cluster_name[Level-1] + " " + choosen->second->cluster_name[Level-1];
                    }
                    cells->CellArray.insert(pair<string, Cell* >(clusterCell->node_name, clusterCell));
                    compute_nbr(clusterCell);
                    ++counter;    

                }   
            }
        }
        if (PQ.empty()) {
            for(auto it=cells->CellArray.begin(); it != cells->CellArray.end(); ++it){
                Cell *u = it->second;
                compute_nbr(u);
            }
        }
        cout << "Clustered blocks: " << Nc << endl;
        uint numCluster = 0;
        cnet.insert(0,"c");
        clusters_File.push_back("Level : " + to_string(Level));
        for (auto it = cells->CellArray.begin() ; it != cells->CellArray.end() ; ++it){
            ++numCluster;
            it->second->cluster_name[Level] = "Cluster" + to_string(Level) +"_" + to_string(numCluster);
            if (Level > 0)
                clusters_File.push_back(it->second->cluster_name[Level] + " " + it->second->cluster_name[Level - 1]);
            else
                clusters_File.push_back(it->second->cluster_name[Level] + " " + it->second->node_name);
        }
        uint numNet = 0;
        int cnt=0;
        cnets_File.push_back("Level : " + to_string(Level));
        cnets_File.push_back("NumNets : " + to_string(cnt));
        cnets_File.push_back("NumPins : " + to_string(nets->NumPins) );
        for (auto it = nets->NetArray.begin() ; it != nets->NetArray.end(); ++it){
            if (it->second->CellList.size() > 1) {
                cnets_File.push_back("NetDegree : " + to_string(it->second->CellList.size()) + "\t" + cnet + to_string(numNet) );
                ++numNet;
                for (vector<int>::size_type i = 0; i < it->second->CellList.size() ; ++i) {
                    cnets_File.push_back("\t" + it->second->CellList[i]->cluster_name[Level] );
                }
                ++cnt;
            }
        }
        cnets_File[1] = "NumNets : " + to_string(cnt);
        cnets_File.push_back("");
        cnets_File.push_back("");
        clusters_File.push_back("");
        clusters_File.push_back("");
        tot_cnets.push_back(cnets_File);
        cnets_File.clear();
        ++Level;
    }
}

