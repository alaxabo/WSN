//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include <src/BaseStation.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>
#include <vector>
#include <cstring>
#include <string.h>
#include <time.h>
#include <ctime>
#include <algorithm>

using namespace std;

Define_Module(BaseStation);

char color[32][15] =
        { "green", "gold", "blue", "red", "yellow", "brown", "cyan", "gray",
                "coral", "navy", "chocolate", "orange", "pink", "purple" };
BaseStation::BaseStation() {
    // TODO Auto-generated constructor stub
    //  this->firstDeadNode = 0;
    //   this->totalDeadNode = 0;
    this->totalEnergyLost = 0.0;
//    int tmp = this->currentRound / 20;
//    int mod = this->currentRound - 20 * tmp;
//    this->T = 0.05 / (1 - 0.05 * mod);

    this->clusterNumber = 4;
    srand(time(NULL));

}

BaseStation::~BaseStation() {
    // TODO Auto-generated destructor stub
}
void BaseStation::createConnection(cModule *c1, cModule *c2) {
    char name1[128];
    char name2[128];
    sprintf(name1, "output %d to input %d", c1->getId(), c2->getId());
    sprintf(name2, "input %d from output %d", c2->getId(), c1->getId());
    cGate *outGate = c1->addGate(name1, cGate::OUTPUT);
    cGate *inGate = c2->addGate(name2, cGate::INPUT);
    outGate->connectTo(inGate);
    outGate->getDisplayString().parse("ls=,0");
    sprintf(name1, "output %d to input %d", c2->getId(), c1->getId());
    sprintf(name2, "input %d from output %d", c1->getId(), c2->getId());
    outGate = c2->addGate(name1, cGate::OUTPUT);
    inGate = c1->addGate(name2, cGate::INPUT);
    outGate->connectTo(inGate);
    outGate->getDisplayString().parse("ls=,0");
}

void BaseStation::initNode() {

    std::ifstream Position("../xy.txt");
    //float temp[4][54];
    float temp[3][54];
    int j = 0;
    std::string s;
    cout << "Read position file." << endl;
    while (std::getline(Position, s)) {
        lib l;
        std::vector<string> str = l.splitString_C(s, " ");
        //for (int i = 0; i < 4; i++) {
        for (int i = 0; i < 3; i++) {
            temp[i][j] = strtof(str[i].c_str(), NULL);
            cout << temp[i][j] << " ";
        }
        cout << endl;
        j++;
    }
    cout << "Reading position file done." << endl;
    for (int i = 0; i < 54; i++) {
        posX[i + 2] = temp[1][i] * 2;
        posY[i + 2] = temp[2][i] * 2;
        //name[i + 2] = temp[3][i];

    }
    cout << "Position" << endl;
    for (int i = 2; i < simulation.getLastModuleId(); i++) {
        Sensor *s = (Sensor*) simulation.getModule(i);
        if (this->getId() != s->getId()) {
            //s->name = name[i];
            s->xpos = posX[i];
            s->ypos = posY[i];
            s->getDisplayString().setTagArg("p", 0, s->xpos);
            s->getDisplayString().setTagArg("p", 1, s->ypos);
            //s->getDisplayString().setTagArg("t", 0, s->name);
            cout << s->xpos << " " << s->ypos << endl;
            createConnection(this, s);

        }
    }
    double distance;
    for (int i = 2; i < simulation.getLastModuleId() - 1; i++) {
        Sensor *s1 = (Sensor*) simulation.getModule(i);
        if (this->getId() != s1->getId()) {
            for (int j = i + 1; j < simulation.getLastModuleId(); j++) {
                Sensor *s2 = (Sensor*) simulation.getModule(j);
                distance = this->getDistance(s1, s2);
                if (distance <= this->trRange)
                    createConnection(s1, s2);
            }
        }
    }
}

//Khoi tao clusters
void BaseStation::initialize() {
    this->xpos = par("xpos");
    this->ypos = par("ypos");
    this->getDisplayString().setTagArg("p", 0, this->xpos);
    this->getDisplayString().setTagArg("p", 1, this->ypos);
    this->k = par("k");
    this->clusterNumber = 4;
    this->trRange = par("trRange");
    this->xMax = par("xMax");
    this->yMax = par("yMax");
    initNode();
    int i;

    this->myClusters = (Clusters**) malloc(this->k * sizeof(Clusters*));

    for (i = 0; i < 4; i++) {
        this->myClusters[i] = new Clusters();
    }
    for (i = 2; i < simulation.getLastModuleId(); i++) {
        Sensor *s = (Sensor*) simulation.getModule(i);
        s->priority = 1;
        if (s->xpos <= this->xMax / 2 && s->ypos <= this->yMax / 2) {
            this->myClusters[0]->addNode(s->getId());
            s->getDisplayString().setTagArg("i", 1, color[0]);
        } else if (s->xpos <= this->xMax / 2 && s->ypos > this->yMax / 2) {
            this->myClusters[1]->addNode(s->getId());
            s->getDisplayString().setTagArg("i", 1, color[1]);
        } else if (s->xpos > this->xMax / 2 && s->ypos <= this->yMax / 2) {
            this->myClusters[2]->addNode(s->getId());
            s->getDisplayString().setTagArg("i", 1, color[2]);
        } else {
            this->myClusters[3]->addNode(s->getId());
            s->getDisplayString().setTagArg("i", 1, color[3]);
        }

    }

    for(int i = 0; i < this->clusterNumber; i++){
        this->myClusters[i]->color = color[i];
        this->findClusterHead(i);
    }

    this->currentRound = 0; // Ban dau la 1
    cout << "Init BS, Cluster." << endl;
    this->sendInitMessage();
}


void BaseStation::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        if (msg->getKind() == SELF_SMSG_INIT) {
            Message *myMsg = (Message*) msg;
            for (cModule::GateIterator i(this); !i.end(); i++) {
                cGate *gate = i();
                if (gate->getType() == cGate::OUTPUT) {
                    cGate *inGate = gate->getPathEndGate();
                    Sensor *s = (Sensor*) inGate->getOwnerModule();
                    if (s->getId() == myMsg->getDestination()) {
                        myMsg->setKind(SMSG_INIT);
                        send(myMsg, gate);
                        cout << "Send init msg to CH" << endl;
                    }
                }
            }
        }
    } else {
        if (msg->getKind() == DATA_TO_BS) {
            //Cap nhat du lieu
            this->count += 1;
            int countCH = 0;
            DataToBSMsg *myMsg = (DataToBSMsg*) msg;
            cout << "Data to BS from " << myMsg->getSource() - 2 << " : "
                    << endl;
            for (unsigned int i = 0; i < myMsg->getDataToBSArraySize(); i++) {
                DataMessage temp = myMsg->getDataToBS(i);
                DataList[temp.getSource() - 2].push_back(temp.getData());
            }

            cout << "Received data from " << this->count << " cluster." << endl;
            cout << "Cluster number " << this->clusterNumber << endl;
            cout << "Sender: " << msg->getSenderModuleId() << endl;
            this->printClusterStatus();
            if (this->count == this->liveCluster) {

                cout << "Chu ky: " << this->currentRound << endl;
                this->currentRound += 1;
                int countDeadNode = 0;
                //Kiem tra nut chet
                for (int i = 2; i < simulation.getLastModuleId(); i++) {
                    Sensor *s = (Sensor*) simulation.getModule(i);
                    EV << "Nang luong nut " << i - 2 << " la: "
                              << s->energy - s->energyLost << endl;
                    if ((s->energy - s->energyLost) <= 201000.0
                            || s->isDead == true) {
                        countDeadNode += 1;
                        s->isDead = true;

                        s->getDisplayString().setTagArg("i", 1, "black");
                        EV << "Nut " << s->getId() - 2 << " da chet: "
                                  << s->isDead << endl;
                        //Loai bo nut chet ra khoi cum
                        removeDeadNode(i);

                    }
                    this->totalEnergyLost += s->energyLost;
                    energyLostStats.collect(this->totalEnergyLost);
                }
                if (countDeadNode == 54){
                    cout << "Chet het roi ----------------" << endl;
                    endSimulation();
                }
                cout << "So nut chet o round " << currentRound << " la: "
                        << countDeadNode << endl;

                //Tinh toan so cum chu
                for (int i = 2; i < simulation.getLastModuleId(); i++) {
                    Sensor *s = (Sensor*) simulation.getModule(i);
                    if (s->isCH) {
                        countCH++;
                    }
                }

                cout << "Hien tai co: " << countDeadNode << " nut chet" << endl;
                cout << "Hien tai co: " << countCH << " nut da lam nut chu"
                        << endl;
                for (int i = 0; i < this->clusterNumber; i++) {
                    cout << "Cum " << i << " co tong so nut la "
                            << this->myClusters[i]->totalMembers << endl;
                }

                cout << "Check end simulation" << endl;

                if (this->currentRound != 0 && this->currentRound % 256 == 0) {
                    for (int i = 0; i < this->clusterNumber; i++) {
                        TreeNode *root = new TreeNode();
                        root->createNYT_TreeNode();
                        this->myClusters[i]->root = root;
                    }
                    cout << "Reclustering" << endl;
                    reClustering();
                    update_DataMsgLength();
                    printLimit();
                    distortion();
                    cout << "Finding Cluster Head" << endl;
                    for (unsigned int i = 0; i < 54; i++) {
                        this->DataList[i].clear();
                    }
                    cout << "Setup Cluster Head" << endl;
                    for (int i = 0; i < this->clusterNumber; i++) {
                        setUpClusterHead(i);
                    }
                    cout << "Send init message" << endl;
                    this->sendInitMessage();
                } else {

                    for (int i = 0; i < this->clusterNumber; i++) {
                        setUpClusterHead(i);
                    }
                    update_DataMsgLength();
                    int tmp = this->currentRound / 20;
                    int mod = this->currentRound - 20 * tmp;
                    this->T = 0.05 / (1 - 0.05 * mod);
                    cout << "Send init message 2" << endl;
                    this->sendInitMessage();
                }
            }
        }
    }
}

void BaseStation::printClusterStatus(){
    for(int i = 0; i < this->clusterNumber; i++){
        Clusters * cluster = this->myClusters[i];

        cout << "Cluster " << i << ": ";

        if(cluster->totalMembers == 0){
            cout << " is dead" << endl;
            continue;
        }

        for (int j = 0; j < cluster->totalMembers; j++){
            cout << " " << cluster->memberNodes[j];
        }

        cout << endl;
    }
}

void BaseStation::sendInitMessage() {
    this->count = 0;
    EV << this->T << endl;
    cout << "Remove node 5" << endl;
//    removeDeadNode(6);
//    simulation.getModule(6)->getDisplayString().setTagArg("i", 1, "black");
//    Sensor *ss = (Sensor*) simulation.getModule(6);
//    ss->isDead = true;
    int clusterNumber = this->clusterNumber;
    cout << "Cluster number: " << clusterNumber << endl;
    for (int i = 0; i < this->clusterNumber; i++) {

        cout << "Check cluster " << i << endl;
        cout << "Cluster member number: " << this->myClusters[i]->totalMembers << endl;
        int count = 0;
        for (int j = 0; j < this->myClusters[i]->totalMembers; j++) {
            Sensor *s = (Sensor *) simulation.getModule(
                    this->myClusters[i]->memberNodes[j]);
            if (s->isDead == false) {
                s->isCH = false;
                count++;
            }
        }
        if (count == 0) {
            cout << "Cluster " << i << " all dead." << endl;
            clusterNumber = clusterNumber - 1;
            this->myClusters[i]->clusterhead = -1;
        } else {
            Sensor *s = (Sensor *) simulation.getModule(
                    this->myClusters[i]->clusterhead);
            if (s->isDead == true) {
                findClusterHead(i);
            }
        }
    }
    this->liveCluster = clusterNumber;
    cout << "Cluster Number :" << this->clusterNumber << endl;

    cout << "Send self message" << endl;
    for (int i = 0; i < this->clusterNumber; i++) {
        if (this->myClusters[i]->clusterhead == -1)
            continue;
        Message *myMsg = new Message();
        myMsg->setKind(SELF_SMSG_INIT);
        myMsg->setDestination(this->myClusters[i]->clusterhead);
        myMsg->setContextPointer(this->myClusters[i]);
        myMsg->setCurrentRound(this->currentRound);
        scheduleAt(simTime() + 1000, myMsg); //ban dau la 1000
    }
}

//void BaseStation::update_DataMsgLength(){
//    lib ex;
//    std::ofstream testJentropy;
//    int round;
//    testJentropy.open("JEntropyGr1.txt", ios::app);
//    testJentropy << "Round: " << this->currentRound;
//    for(int i = 0; i < this->clusterNumber; i++) {
//        vector<vector<double>> data;
//        for (int j = 2; j < simulation.getLastModuleId(); j++){
//            Sensor *s = (Sensor*) simulation.getModule(j);
//            if (this->myClusters[i]->hasMember(s->getId())) data.push_back(this->DataList[j-2]);
//            round = s->currentRound;
//        }
//        double temp_je = ex.jEntropyGroup(data);
//        testJentropy << "Round: " << round << endl << i << "\t" << temp_je << endl;
//        for (int j = 2; j < simulation.getLastModuleId(); j++){
//                    double entropy = ex.entropy(this->DataList[j-2]);
//                    Sensor *s = (Sensor*) simulation.getModule(j);
//                    if (s->isCH && this->myClusters[i]->hasMember(s->getId())) s->DataMsg_Length = temp_je*1000;
//                    else {
//
//                        s->DataMsg_Length = entropy*1000;
//                        cout << entropy << "  " << "--";
//                    }
//                    if (this->myClusters[i]->hasMember(s->getId()))
//                        testJentropy << j-2 << "\t" << entropy << endl;
//                    cout << endl;
//
//                }
//       }
//    testJentropy.close();
//}

void BaseStation::update_DataMsgLength(){
    lib ex;
    std::ofstream file;
    int clusterHeadId;

    for(int i = 0; i < this->clusterNumber; i++) {
        vector<vector<double>> data;
        double sumEntropy = 0;

        Clusters * cluster = this->myClusters[i];

        for (int j = 0; j < cluster->totalMembers; j++){
            Sensor *s = (Sensor*) simulation.getModule(cluster->memberNodes[j]);

            if (s->isCH){
                clusterHeadId = s->getId();
            }

            data.push_back(this->DataList[s->getId() - 2]);

            double entropy = ex.entropy(this->DataList[s->getId() - 2]);
            if (entropy == 0){
                s->DataMsg_Length = 4000;
            }
            else{
                s->DataMsg_Length = entropy*100;
            }
            sumEntropy += entropy;
            cout << entropy << "  " << "--";

            std::stringstream ss;
            ss << "../Entropy/" << s->getId() - 1 << ".txt";
            string fileName = ss.str();
            cout << "file name: " << fileName << endl;

            ofstream file;
            if (this->currentRound == 1){
                file.open (fileName);
            } else {
                file.open (fileName, ios::app);
            }

            if(file.is_open()){
                file << entropy << endl;
                file.close();
            } else {
                cout << "Can not open file" << endl;
            }
            cout << endl;
        }

        double jEntropy = ex.jEntropyGroup(data);
        Sensor * s = (Sensor*) simulation.getModule(clusterHeadId);
        s->CHrecvDataMsg_Length = sumEntropy;
        if (jEntropy == 0){
            s->DataMsg_Length = 4000;
        }
        else{
            s->DataMsg_Length = jEntropy*100;
        }
    }
}

void BaseStation::printLimit(){
    lib ex;
    std::ofstream limitJentropy;
    if (this->currentRound == 256){
        limitJentropy.open("limitJentropy.txt");
        limitJentropy << "Round 256:" << endl;

    }
    else{
        limitJentropy.open("limitJentropy.txt", ios::app);
        limitJentropy << "Round " << this->currentRound << ":" << endl;
    }
    for (int i = 0; i < this->clusterNumber; i++){
        double rMax = 0;
        double rMin = 9999;
        double hMax = 0;
        double hMin = 9999;

        limitJentropy << "Group "<< i + 1 <<" Got Member: " << endl;

        for(int m = 0; m < this->myClusters[i]->totalMembers; m++){
            Sensor *sm = (Sensor *) simulation.getModule(this->myClusters[i]->memberNodes[m]);
            limitJentropy << sm->getId() - 2 << " ";
        }
        limitJentropy << endl;

        vector<vector<double>> data;

        for (int j = 0; j < this->myClusters[i]->totalMembers; j++){
            Sensor *s2 = (Sensor *) simulation.getModule(this->myClusters[i]->memberNodes[j]);
            double nodeH = ex.entropy(this->DataList[s2->getId() - 2]);
            if (hMax < nodeH)
                hMax = nodeH;
            if (hMin > nodeH)
                hMin = nodeH;
        }
        limitJentropy << "Group " << i + 1 << " Got Hmax: " << hMax << " And Hmin: " << hMin <<endl;
        for (int k = 0; k < this->myClusters[i]->totalMembers; k++){
            Sensor *s1 = (Sensor *) simulation.getModule(this->myClusters[i]->memberNodes[k]);
            for (int j = 0; j < this->myClusters[i]->totalMembers; j++) {
               Sensor *s2 = (Sensor *) simulation.getModule(this->myClusters[i]->memberNodes[j]);
               if (s1->getId() == s2->getId())
                   continue;
               double ecc = ex.EntropyCorrelationCoefficient(this->DataList[s1->getId() - 2], this->DataList[s2->getId()-2]);
               if (rMax < ecc)
                   rMax = ecc;
               if (rMin > ecc)
                   rMin = ecc;
            }
        }
        limitJentropy << "Group " << i + 1 << " Got Rmax: " << rMax << " And Rmin: " << rMin <<endl;

        int n = this->myClusters[i]->totalMembers;
        double bMin = 2 - rMax;
        double kMin = ex.findK(bMin, n);
        double bMax = 2 - rMin;
        double kMax =  ex.findK(bMax, n);

        limitJentropy << "Group " << i + 1 << " Got Down Limit: " << kMin * hMin << " And Up Limit: " << kMax * hMax << endl;

        for (int j = 0; j < this->myClusters[i]->totalMembers; j++){
            Sensor *s1 = (Sensor *) simulation.getModule(this->myClusters[i]->memberNodes[j]);
            data.push_back(this->DataList[s1->getId() - 2]);
        }
        double temp_je = ex.jEntropyGroup(data);
        limitJentropy << "Group " << i + 1 << " Got Joint Entropy: " << temp_je << endl;

    }
    limitJentropy.close();
}

void BaseStation::reClustering() {
    lib ex;
    int clusterCount = 0;
    int liveNodeNumber = 0;
    double baseECC = 0.7;
    double baseEntropy = 0;
    double incrementEntropy = 0.2;
    std::vector<double> entropyList;
    // Kiem tra nhung node chet
    cout << "Kiem tra nhung node chet" << endl;
    for (int i = 2; i < simulation.getLastModuleId(); i++) {
       Sensor *s = (Sensor*) simulation.getModule(i);

       if ((s->energy - s->energyLost) <= 201000.0 || s->isDead == true) {
           s->isDead = true;
           s->getDisplayString().setTagArg("i", 1, "black");
       }
       this->totalEnergyLost += s->energyLost;
       energyLostStats.collect(this->totalEnergyLost);
   }

    // Tinh toan entropy cua tung nut va tinh so nut con song
    cout << "Tinh toan entropy cua tung nut va tinh so nut con song" << endl;
    for (int i = 0; i < 54; i++) {
        Sensor *s = (Sensor *) simulation.getModule(i + 2);
        s->isCH = false;
        s->myCluster = NULL;

        if (!s->isDead) {
            double entropy = ex.entropy(this->DataList[i]);

            entropyList.push_back(entropy);
            s->entropy = entropy;
            //entropyList[s->getId()] = entropy;
            liveNodeNumber++;
        }
    }

    sort(entropyList.begin(), entropyList.end());

    while(baseEntropy < int(entropyList.size())){
        std::vector<int> nodeArray;

        // Add nhung nut co entropy trong khoang [baseEntropy, baseEntropy + deltaH] vao nhom
        nodeArray.clear();
        for(int i = 0; i< 54; i++){
           Sensor *s = (Sensor *) simulation.getModule(i + 2);
           if(!s->isDead &&
                   !s->myCluster &&
                   s->entropy >= entropyList[baseEntropy] &&
                   s->entropy <= entropyList[baseEntropy] + incrementEntropy){
               nodeArray.push_back(s->getId());
           }
        }

        // Neu so nut trong nhom nho hon 6 thi tang H0 va thu lai
        if(nodeArray.size() < 6){
            baseEntropy += 1;
            continue;
        }

        // Tao nhom moi
        this->myClusters[clusterCount] = new Clusters();
        Clusters * cluster = this->myClusters[clusterCount];

        // Phan nhom cho cac nut trong array
        while(baseECC >= 0.5) {
            // Tam thoi add cac nut chua co nhom vao nhom vua tao
            for (double id : nodeArray) {
               Sensor *s = (Sensor *) simulation.getModule(id);

               if(s->myCluster == NULL){
                  // Add cac nut
                  cluster->addNode(s->getId());
                  s->myCluster = cluster;
               }
            }

            if (cluster->totalMembers < 6){
                break;
            }

            while(1){
                std::map<int, int> countList;
                // Voi moi nut trong nhom, dem so nut co do tuong quan KHONG thoa man voi nut dang xet
                for(int i = 0; i < cluster->totalMembers; i++){
                    Sensor *s = (Sensor *) simulation.getModule(cluster->memberNodes[i]);

                    int count = 0;

                    for (int j = 0; j < cluster->totalMembers; j++) {
                        Sensor *s2 = (Sensor *) simulation.getModule(cluster->memberNodes[j]);
                        double ecc = ex.EntropyCorrelationCoefficient(this->DataList[s->getId() - 2], this->DataList[s2->getId()-2]);
                        if(ecc < baseECC){
                            count++;
                        }
                    }

                    countList[s->getId()] = count;
                }

                // Tim ra nut nao KHONG tuong quan nhat voi cac nut con lai trong nhom va loai nut do ra
                auto maxCount = std::max_element
                (
                    std::begin(countList), std::end(countList),
                    [] (const pair<int, int>& p1, const pair<int, int>& p2) {
                        return p1.second < p2.second;
                    }
                );
                cout << maxCount->first << " : " << maxCount->second << endl;

                // Khi tat ca cac nut trong nhom thoa man dieu kien
                if(maxCount->second == 0){
                    cout << "Tong so nut trong nhom: " << cluster->totalMembers << endl;
                    // Neu so nut nho hon 6 thi tang deltaH hoac giam p va thu lai
                    if(cluster->totalMembers < 6){
                        baseECC -= 0.1;
                    } else {
                        // Khi so nut lon hon 6 thi tao nhom tiep theo
                        baseECC = 0.7;
                        clusterCount++;
                        this->myClusters[clusterCount] = new Clusters();
                        cluster = this->myClusters[clusterCount];

                    }
                    break;
                } else {
                    // Kick nut KHONG thoa man dieu kien nhat
                    cout << "Removing node " << maxCount->first << "from cluster" << endl;
                    cluster->removeNode(maxCount->first);
                    Sensor * s = (Sensor *) simulation.getModule(maxCount->first);
                    s->myCluster = NULL;
                    continue;
                }
            }
        }

    if(cluster->totalMembers < 6){
                for (int i = 0; i < cluster->totalMembers; i++){
                    Sensor *s = (Sensor *) simulation.getModule(cluster->memberNodes[i]);
                    s->myCluster = NULL;
                }
            }

        baseEntropy++;
        baseECC = 0.7;
    }
    clusterCount = leach_Clustering(clusterCount);
    cout << "clusterCount is:--------------- " << clusterCount << endl;
    this->clusterNumber = clusterCount;

    for (int i = 2; i < simulation.getLastModuleId(); i++) {
        Sensor *s1 = (Sensor*) simulation.getModule(i);
        if (s1->myCluster == NULL) cout << s1->getId() << "  --  ";
    }
    cout << endl;


    for (int i = 0; i < clusterCount; i++){
        Clusters *cluster = this->myClusters[i];
        cluster->color = color[i];

//        this->setPriority(i);
//        for (int j = 0; j < cluster->totalMembers; j++){
//            Sensor *s = (Sensor *) simulation.getModule(cluster->memberNodes[j]);
//            cout << "Node " << i + 1 << "Priority " << s->priority << endl;
//        }

        this->findClusterHead(i);
    }

}

void BaseStation::distortion(){
    lib ex;
    for (int i = 0; i < this->clusterNumber; i++){
        double r0 = 9999;
        int repreNum;

        for (int k = 0; k < this->myClusters[i]->totalMembers; k++){
            Sensor *s1 = (Sensor *) simulation.getModule(this->myClusters[i]->memberNodes[k]);

            for (int j = 0; j < this->myClusters[i]->totalMembers; j++) {
               Sensor *s2 = (Sensor *) simulation.getModule(this->myClusters[i]->memberNodes[j]);
               if (s1->getId() == s2->getId())
                   continue;
               double ecc = ex.EntropyCorrelationCoefficient(this->DataList[s1->getId() - 2], this->DataList[s2->getId()-2]);
               if (r0 > ecc)
                   r0 = ecc;
            }
        }
        int n = this->myClusters[i]->totalMembers;
        double kN = ex.findK(2 - r0, n);

        cout << "Group " << i + 1 << ": " << endl;

        for (int m = 1; m <= this->myClusters[i]->totalMembers; m++){
            double kM = ex.findK(2 - r0, m);
            double D = 1 - (kM/kN);
            cout << "If m = " << m << "Then " << endl;
            cout << "D is: " << D << endl;
            if (D <= 0.15){
                repreNum = m;
                break;
            }
        }

        //print distortion
        if (i == 0){
            std::ofstream distortion;
            if (this->currentRound == 256){
                distortion.open("distortion.txt");
                distortion << "Round 256:" << endl;
                distortion << "Group " << i + 1 << " Got Member: ";
                for (int k = 0; k < this->myClusters[i]->totalMembers; k++){
                    Sensor *s = (Sensor *) simulation.getModule(this->myClusters[i]->memberNodes[k]);
                    distortion <<  s->getId() - 1 << " ";
                }
                distortion << "Node Energy Remain: " << endl;
                for (int k = 0; k < this->myClusters[i]->totalMembers; k++){
                    Sensor *s = (Sensor *) simulation.getModule(this->myClusters[i]->memberNodes[k]);
                    distortion <<  s->getId() - 1 << " ";
                    distortion << s->energy - s->energyLost << endl;
                }
                distortion << "" << endl;
                for (int m = 1; m <= this->myClusters[i]->totalMembers; m++){
                    double kM = ex.findK(2 - r0, m);
                    double D = 1 - (kM/kN);
                    distortion << "If m = " << m << "Then " << endl;
                    distortion << "D is: " << D << endl;
                }
                distortion << "representative nodes numbers: " << repreNum << endl;
                distortion.close();
            }
            for (int k = 0; k < this->myClusters[i]->totalMembers; k++){
                Sensor *s = (Sensor *) simulation.getModule(this->myClusters[i]->memberNodes[k]);
                s->repreNum = repreNum;
            }
        }
        else{
            for (int k = 0; k < this->myClusters[i]->totalMembers; k++){
                Sensor *s = (Sensor *) simulation.getModule(this->myClusters[i]->memberNodes[k]);
                s->repreNum = repreNum;
            }
        }
    }
}

int BaseStation::leach_Clustering(int clusterCount) {
    // double p = 0.07;
    // int mod = this->currentRound % 14;
    // this->T = 0.07 / (1 - 0.07 * mod);
    // cout << "Round: " << currentRound << " T: " << T << endl;
    int clusterC = clusterCount;
    int imax=0,jmax=0,count=0,max=-1;
    pair<double, int> tmp[100];
    double dis[100][100],temp[100];
    for (int i = 2; i < simulation.getLastModuleId(); i++){
        Sensor *s1 = (Sensor*) simulation.getModule(i);
        if (!s1->isDead && s1->myCluster == NULL){
            cout << s1->getId() << endl;
            count++;
        }
        for (int j = 2; j < simulation.getLastModuleId(); j++){

            Sensor *s2 = (Sensor*) simulation.getModule(j);
            if (!s1->isDead && !s2->isDead && s1->myCluster == NULL && s2->myCluster == NULL && i!=j){
                dis[i][j]= getDistance(s1, s2);
                if (dis[i][j] > max){
                    max = dis[i][j];
                    imax=i;
                    jmax=j;

                }
            } else dis[i][j]=-1;
        }
        }
    cout << imax << " " << jmax << " " << count << endl;
    if (count==0) return clusterC;
    for (int i = 2; i < simulation.getLastModuleId(); i++){
        tmp[i].first=dis[imax][i];
        tmp[i].second=i;
    }

    sort(tmp+2, tmp+simulation.getLastModuleId());
    for (int i = 2; i < simulation.getLastModuleId(); i++){
            cout << tmp[i].first << " " << tmp[i].second << endl;
        }
    for (int i = 2; i < simulation.getLastModuleId(); i++) cout << temp[i] << " ";
    cout << endl;

    this->myClusters[clusterC] = new Clusters();
    Clusters * cluster = this->myClusters[clusterC];

    this->myClusters[clusterC + 1] = new Clusters();
    Clusters * cluster1 = this->myClusters[clusterC + 1];

    Sensor *s = (Sensor *) simulation.getModule(imax);
    cluster->addNode(s->getId());
    s->myCluster = cluster;
    int c=1;
    for (int i = 2; i < simulation.getLastModuleId(); i++){
        if (tmp[i].first > 0 && tmp[i].second!=imax) {
            if (c < count/2){
                //Clusters * cluster = this->myClusters[clusterC];
                Sensor *s1 = (Sensor *) simulation.getModule(tmp[i].second);
                cluster->addNode(s1->getId());
                s1->myCluster = cluster;
                c++;
            } else {
                Sensor *s2 = (Sensor *) simulation.getModule(tmp[i].second);
                cluster1->addNode(s2->getId());
                s2->myCluster = cluster1;
                c++;
            }
        }
    }

    return clusterC+2;
}
//Chon cluster head dua tren muc nang luong
void BaseStation::findClusterHead(int index) {
    std::vector<double> ed;
//    std::vector<double> distance;
    for (int i = 2; i < simulation.getLastModuleId(); i++) {
        Sensor *s = (Sensor *) simulation.getModule(i);
        s->isCH = false;
    }
    for (int i = 0; i < this->myClusters[index]->totalMembers; i++) {

        Sensor *s1 = (Sensor*) simulation.getModule(
                this->myClusters[index]->memberNodes[i]);
        double sum = 0;
        for (int j = 0; j < this->myClusters[index]->totalMembers; j++) {
            Sensor *s2 = (Sensor*) simulation.getModule(
                    this->myClusters[index]->memberNodes[j]);
            sum += getDistance(s1, s2) * getDistance(s1, s2);
        }
        double mean = sum / (this->myClusters[index]->totalMembers);
        if (s1->isDead == false) {
            ed.push_back((s1->energy - s1->energyLost) / mean);
        }
    }
    double max_ed = *std::max_element(ed.begin(), ed.end());
    for (int i = 0; i < this->myClusters[index]->totalMembers; i++) {
        Sensor *s1 = (Sensor*) simulation.getModule(
                this->myClusters[index]->memberNodes[i]);
        double sum = 0;
        for (int j = 0; j < this->myClusters[index]->totalMembers; j++) {
            Sensor *s2 = (Sensor*) simulation.getModule(
                    this->myClusters[index]->memberNodes[j]);
            sum += getDistance(s1, s2) * getDistance(s1, s2);
        }
        double mean = sum / (this->myClusters[index]->totalMembers);

        double ed_s = (s1->energy - s1->energyLost) / mean;
//        cout << "Node " << s1->getId() - 2 << endl;
        if (ed_s == max_ed) {
            this->myClusters[index]->clusterhead = s1->getId();
            s1->isCH = true;
            s1->getDisplayString().setTagArg("i", 1, color[13]);
            cout << "Cluster " << index << " header : "
                    << this->myClusters[index]->clusterhead - 2 << endl;
            break;
        }
    }
}

void BaseStation::removeDeadNode(int index) {
    Sensor *s = (Sensor*) simulation.getModule(index);
    for (int i = 0; i < this->clusterNumber; i++) {
        for (int j = 0; j < this->myClusters[i]->totalMembers; j++) {
            if (this->myClusters[i]->memberNodes[j] == s->getId()) {
                this->myClusters[i]->removeNode(s->getId());
                break;
            }
        }
    }
}
void BaseStation::setUpClusterHead(int i) {
    if(this->myClusters[i]->totalMembers == 0){
        return ;
    }

    for (int j = 0; j < this->myClusters[i]->totalMembers; j++) {
        Sensor *s = (Sensor*) simulation.getModule(
                this->myClusters[i]->memberNodes[j]);

        s->isCH = false;
    }

    this->findClusterHead(i);
    cout << "Setup Cluster Head" << endl;
}
double BaseStation::getDistance(Sensor *s1, Sensor *s2) {
    double xlength = abs(s1->xpos - s2->xpos);
    double ylength = abs(s1->ypos - s2->ypos);
    double distance = sqrt(xlength * xlength + ylength * ylength);
    return distance / 4;
}

void BaseStation::setPriority(int groupNumber){
    lib ex;
    Clusters *cluster = this->myClusters[groupNumber];

    // Tinh toan do tuong quan du lieu cua cac nut trong nhom
    for (int i = 0; i < cluster->totalMembers; i++) {
       int count1 = 0;
       int count2 = 0;
       int count3 = 0;
       Sensor *s = (Sensor *) simulation.getModule(cluster->memberNodes[i]);
       s->priority = 1;
//       for (int j = 0; j < cluster->totalMembers; j++) {
//           Sensor *s2 = (Sensor *) simulation.getModule(cluster->memberNodes[j]);
//           double ecc = ex.EntropyCorrelationCoefficient(this->DataList[s->getId() - 2], this->DataList[s2->getId()-2]);
//           cout << "Node " << s->getId() << " va Node " << s2->getId() << " : " << ecc << endl;
//           this->EntropyCorrCoeff[i][j] = ecc;
//           if (ecc >= 0.7)
//               count1++;
//           if (0.6 <= ecc && ecc < 0.7)
//               count2++;
//           if (ecc < 0.6)
//               count3++;
//       }
//       if (count1 >= cluster->totalMembers / 2)
//           s->priority = 3;
//       else if (count2 >= cluster->totalMembers / 2)
//           s->priority = 2;
//       else
//           s->priority = 1;
    }
}



bool BaseStation::find_Node(Sensor *s, int n) {
    for (int i = 0; i < s->myCluster->totalMembers; i++) {
        if (s->myCluster->memberNodes[i] == n + 2) {
            return true;
        }
    }
    return false;
}


void BaseStation::finish() {
    cout << "Total Energy Lost: " << this->totalEnergyLost << endl;
    cout << "Mean Total Energy Lost: " << this->totalEnergyLost / 53 << endl;
    Sensor *s2 = (Sensor *) simulation.getModule(20);
    cout << "Init Energy:" << s2->energy << endl;
    cout << "Round: " << this->currentRound << endl;
    recordScalar("TotalEnergy: ", this->totalEnergyLost);
    int first_dead = 1000;
    std::ofstream deadNode;
    deadNode.open("DeadNode.txt");
    for (int i = 2; i < simulation.getLastModuleId(); i++) {
        Sensor *s = (Sensor *) simulation.getModule(i);
        if (s->isDead == false) cout << "--ndead " << s->getId();
        deadNode << i - 1 << " " << s->roundDead << endl;
        if (s->roundDead >= 0) first_dead = min(first_dead, s->roundDead );
    }
    deadNode.close();
    cout << "vong dau tien chet: " << first_dead << endl;
}
