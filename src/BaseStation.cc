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
    float temp[3][54];
    int j = 0;
    std::string s;
    cout << "Read position file." << endl;
    while (std::getline(Position, s)) {
        lib l;
        std::vector<string> str = l.splitString_C(s, " ");
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

    }
    cout << "Position" << endl;
    for (int i = 2; i < simulation.getLastModuleId(); i++) {
        Sensor *s = (Sensor*) simulation.getModule(i);
        if (this->getId() != s->getId()) {
            s->xpos = posX[i];
            s->ypos = posY[i];
            s->getDisplayString().setTagArg("p", 0, s->xpos);
            s->getDisplayString().setTagArg("p", 1, s->ypos);
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

void BaseStation::reClustering() {
    lib ex;
    int nodesInGroup = 0;
    int clusterCount = 0;
    int liveNodeNumber = 0;
    double baseECC = 0.7;
    double baseEntropy = 100;
    double maxEntropy = 0;
    double incrementEntropy = 0.2;
    std::map<int, double> entropyList;

    // Kiem tra nhung node chet
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
    for (int i = 0; i < 54; i++) {
        Sensor *s = (Sensor *) simulation.getModule(i + 2);
        s->isCH = false;
        s->myCluster = NULL;

        if (!s->isDead) {
            double entropy = ex.entropy(this->DataList[i]);
            maxEntropy = max(entropy, maxEntropy);
            baseEntropy = min(entropy, baseEntropy);

            entropyList[s->getId()] = entropy;
            liveNodeNumber++;
        }
    }

    cout << "Live nodes: " << liveNodeNumber << endl;
    cout << "base entropy: " << baseEntropy << endl;

    while(nodesInGroup < liveNodeNumber){
        this->myClusters[clusterCount] = new Clusters();
        Clusters * cluster = this->myClusters[clusterCount];

        // Add nhung nut co entropy trong khoang [baseEntropy, baseEntropy + deltaH] vao nhom
        for(int i = 0; i< 54; i++){
           Sensor *s = (Sensor *) simulation.getModule(i + 2);
           if(!s->isDead &&
                   !s->myCluster &&
                   entropyList[s->getId()] >= baseEntropy &&
                   entropyList[s->getId()] <= baseEntropy + incrementEntropy){
               cluster->addNode(s->getId());
               s->myCluster = cluster;
               nodesInGroup++;
           }
        }

        if(incrementEntropy > maxEntropy - baseEntropy){
            clusterCount++;
            break;
        }

        if (nodesInGroup >=  liveNodeNumber){
           clusterCount++;
           break;
        }

        // Neu so nut trong nhom nho hon 6 thi tang deltaH va thu lai
        if(cluster->totalMembers < 6){
            cout << "Max: " << maxEntropy << " Min: " << baseEntropy << "Incre: " << incrementEntropy << endl;
            incrementEntropy += 0.2;
            // Loai cac nut ra khoi nhom
            for (int i = 0; i < cluster->totalMembers; i++){
                Sensor *s = (Sensor *) simulation.getModule(cluster->memberNodes[i]);
                s->myCluster = NULL;
            }
            nodesInGroup -= cluster->totalMembers;
            cluster->removeAllNode();
            continue;
        }



        while(1) {
            std::map<int, int> countList;
            std::map<int, std::vector<double>> eccList;
            cout << "Total members: " << this->myClusters[clusterCount]->totalMembers << endl;
            cout << "Node in group: " << nodesInGroup << endl;
            cout << "Cluster count: " << clusterCount << endl;

            // Tinh toan do tuong quan du lieu cua cac nut trong nhom
            for (int i = 0; i < cluster->totalMembers; i++) {
               Sensor *s = (Sensor *) simulation.getModule(cluster->memberNodes[i]);
               for (int j = 0; j < cluster->totalMembers; j++) {
                   Sensor *s2 = (Sensor *) simulation.getModule(cluster->memberNodes[j]);
                   double ecc = ex.EntropyCorrelationCoefficient(this->DataList[s->getId() - 2], this->DataList[s2->getId()-2]);
                   cout << "Node " << s->getId() << " va Node " << s2->getId() << " : " << ecc << endl;
                   eccList[s->getId()].push_back(ecc);
               }
           }

            // Voi moi nut trong nhom, dem so nut co do tuong quan KHONG thoa man voi nut dang xet
            for(int i = 0; i < cluster->totalMembers; i++){
                Sensor *s = (Sensor *) simulation.getModule(cluster->memberNodes[i]);

                cout << "Id: " << s->getId();
               cout << " Is dead: " << s->isDead;
               cout << " Cluster: " << s->myCluster;
               cout << " Entropy " << entropyList[s->getId()] << endl;

                /*int count = std::count(std::begin(eccList[s->getId()]),
                        std::end(eccList[s->getId()]), baseECC);*/

                int count = 0;

                for (int i = 0; i < eccList[s->getId()].size(); i++){
                    if(eccList[s->getId()][i] < baseECC){
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
                    if(baseECC > 0.5){
                        baseECC -= 0.1;
                    } else {
                        incrementEntropy += 0.2;
                    }

                    // Loai cac nut ra khoi nhom
                    for (int i = 0; i < cluster->totalMembers; i++){
                        Sensor *s = (Sensor *) simulation.getModule(cluster->memberNodes[i]);
                        s->myCluster = NULL;
                    }
                    nodesInGroup -= cluster->totalMembers;
                    cluster->removeAllNode();
                } else {
                    // Khi so nut lon hon 6 thi tao nhom tiep theo
                    baseECC = 0.7;
                    clusterCount++;
                }
                break;
            } else {
                // Kick nut KHONG thoa man dieu kien nhat
                cout << "Removing node " << maxCount->first << "from cluster" << endl;
                cluster->removeNode(maxCount->first);
                Sensor * s = (Sensor *) simulation.getModule(maxCount->first);
                s->myCluster = NULL;
                nodesInGroup--;

                continue;
            }
        }
        cout << "node in group " << nodesInGroup << endl;
    }

    this->clusterNumber = clusterCount;

    for (int i = 0; i < clusterCount; i++){
        Clusters *cluster = this->myClusters[i];
        cluster->color = color[i];

//        if (i == 0){
//            for (int j = 0; j < cluster->totalMembers; j++){
//                Sensor *s = (Sensor *) simulation.getModule(cluster->memberNodes[j]);
//                s->getDisplayString().setTagArg("i", 1, cluster->color);
//                s->priority = 3;
//            }
//        }
//        else
        {
            this->setPriority(i);
            for (int j = 0; j < cluster->totalMembers; j++){
                Sensor *s = (Sensor *) simulation.getModule(cluster->memberNodes[j]);
                cout << "Node " << i + 1 << "Priority " << s->priority << endl;
            }
        }
        this->findClusterHead(i);
    }
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
       for (int j = 0; j < cluster->totalMembers; j++) {
           Sensor *s2 = (Sensor *) simulation.getModule(cluster->memberNodes[j]);
           double ecc = ex.EntropyCorrelationCoefficient(this->DataList[s->getId() - 2], this->DataList[s2->getId()-2]);
           cout << "Node " << s->getId() << " va Node " << s2->getId() << " : " << ecc << endl;
           this->EntropyCorrCoeff[i][j] = ecc;
           if (ecc >= 0.7)
               count1++;
           if (0.6 <= ecc && ecc < 0.7)
               count2++;
           if (ecc < 0.6)
               count3++;
       }
       if (count1 >= cluster->totalMembers / 2)
           s->priority = 3;
       if (count2 >= cluster->totalMembers / 2)
           s->priority = 2;
       if (count3 >= cluster->totalMembers / 2)
           s->priority = 1;
    }

    //Tinh do tuong quan trung binh cua cac node trong nhom
//    for (int i = 0; i < cluster->totalMembers; i++) {
//        this->AvgEntropyCorrCoeff[i] = 0;
//        for (int j = 0; j < cluster->totalMembers; j++){
//            if (i == j)
//                continue;
//            this->AvgEntropyCorrCoeff[i] += this->EntropyCorrCoeff[i][j];
//        }
//        this->AvgEntropyCorrCoeff[i] = this->AvgEntropyCorrCoeff[i]/cluster->totalMembers;
//    }

//    //Set Priority
//    for (int i = 0; i < cluster->totalMembers; i++) {
//       int count = 0;
//       Sensor *s1 = (Sensor *) simulation.getModule(cluster->memberNodes[i]);
//       for (int j = 0; j < cluster->totalMembers; j++){
//           //Sensor *s2 = (Sensor *) simulation.getModule(cluster->memberNodes[j]);
//           if (i == j)
//               continue;
//           if (this->EntropyCorrCoeff[i][j] >= this->AvgEntropyCorrCoeff[i])
//               count++;
//       }
//       if (count >= 0.8 * cluster->totalMembers)
//           s1->priority = 3;
//       if (0.5 * cluster->totalMembers <= count && count < 0.8 * cluster->totalMembers)
//           s1->priority = 2;
//       if (0.5 * cluster->totalMembers > count)
//           s1->priority = 1;
//    }

}

/*void BaseStation::setPriority() {
    lib ex;
//    cout << "Reset priority" << endl;
    //reset do uu tien cua cac sensor
    for (int i = 2; i < simulation.getLastModuleId(); i++) {
        Sensor *s = (Sensor*) simulation.getModule(i);
        s->priority = 1;
    }
//    cout << "Calculate entropy correlation coefficient" << endl;
    //tinh tung cap do tuong quan

    for (int i = 2; i < simulation.getLastModuleId(); i++) {
        for (int j = 2; j < simulation.getLastModuleId(); j++) {
            Sensor *s1 = (Sensor *) simulation.getModule(i);
            Sensor *s2 = (Sensor *) simulation.getModule(j);
            if (s1->isDead == true || s2->isDead == true)
                continue;
//            cout << "Entropy " << i-2 << ": " <<ex.entropy(this->DataList[i-2]) << endl;
//            cout << "Entropy " << j-2 << ": " << ex.entropy(this->DataList[j-2]) << endl;
//            cout << "Joint entropy " << i-2 << "," << j-2 << ": " << ex.jEntropy(this->DataList[i-2], this->DataList[j-2]) << endl;
//            cout << "Correlation coefficient " << i-2 << "," << j-2 << ": " << ex.EntropyCorrelationCoefficient(this->DataList[i-2],this->DataList[j-2]) << endl;
            this->EntropyCorrCoeff[i - 2][j - 2] =
                    ex.EntropyCorrelationCoefficient(this->DataList[i - 2],
                            this->DataList[j - 2]);
        }
    }

    cout << "Calculate Avg coeff" << endl;
    //tinh do tuong quan trung binh cua cac node
    for (int n = 0; n < 4; n++) {
        cout << "Cluster : " << n << endl;
        for (int i = 0; i < this->myClusters[n]->totalMembers; i++) {
            cout << "Node : " << this->myClusters[n]->memberNodes[i] - 2
                    << endl;
            this->AvgEntropyCorrCoeff[this->myClusters[n]->memberNodes[i] - 2] =
                    0;
            for (int j = 0; j < this->myClusters[n]->totalMembers; j++) {
                if (j == i)
                    continue;
                this->AvgEntropyCorrCoeff[this->myClusters[n]->memberNodes[i]
                        - 2] +=
                        this->EntropyCorrCoeff[this->myClusters[n]->memberNodes[i]
                                - 2][this->myClusters[n]->memberNodes[j] - 2];
                cout << "EntropyCorrCoeff : "
                        << this->myClusters[n]->memberNodes[i] - 2 << " "
                        << this->myClusters[n]->memberNodes[j] - 2
                        << this->EntropyCorrCoeff[this->myClusters[n]->memberNodes[i]
                                - 2][this->myClusters[n]->memberNodes[j] - 2]
                        << endl;
            }
            this->AvgEntropyCorrCoeff[this->myClusters[n]->memberNodes[i] - 2] =
                    this->AvgEntropyCorrCoeff[this->myClusters[n]->memberNodes[i]
                            - 2] / this->myClusters[n]->totalMembers;
            cout << "Avg Correlation Coefficient: "
                    << AvgEntropyCorrCoeff[this->myClusters[n]->memberNodes[i]
                            - 2] << endl;
        }
    }
    cout << "Calculate and set priority" << endl;
    //tinh do uu tien cua tung node
    cout << "thiet lap myCluster cua tung node" << endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < this->myClusters[i]->totalMembers; j++) {
            Sensor *s = (Sensor *) simulation.getModule(
                    this->myClusters[i]->memberNodes[j]);
            s->myCluster = this->myClusters[i];
        }
    }

    for (int i = 2; i < simulation.getLastModuleId(); i++) {
        int count = 0;
        Sensor *s = (Sensor*) simulation.getModule(i);
        cout << "Set priority nodeID: " << i << endl;
//            cout << "My cluster:" << s->myCluster->clusterhead << " " << s->myCluster->totalMembers << endl;
        if (s->isDead == true || i == 6) {
            cout << "Node dead" << endl;
            s->priority = 0;
            continue;
        } else {
            for (int j = 0; j < s->myCluster->totalMembers; j++) {
                Sensor *s2 = (Sensor*) simulation.getModule(
                        s->myCluster->memberNodes[j]);
                if (s->isDead == true || s2->isDead == true
                        || i == s->myCluster->memberNodes[j]) {
                    cout << "Continue" << endl;
                    continue;
                }
                cout << "Compare" << endl;
                if (this->EntropyCorrCoeff[i - 2][s->myCluster->memberNodes[j]
                        - 2] >= this->AvgEntropyCorrCoeff[i - 2]) {
                    cout << "Compare corr between " << i - 2 << " and "
                            << s->myCluster->memberNodes[j] - 2 << ": "
                            << EntropyCorrCoeff[i - 2][s->myCluster->memberNodes[j]
                                    - 2] << " , " << AvgEntropyCorrCoeff[i - 2]
                            << endl;
                    count += 1;
                    cout << "Count priority at this step: " << count << endl;
                }
            }
            if (count >= (4 * s->myCluster->totalMembers) / 5) {
                s->priority = 3;
            }
            if (s->myCluster->totalMembers / 2 <= count
                    && count < (4 * s->myCluster->totalMembers) / 5)
                s->priority = 2;
            if (count < s->myCluster->totalMembers / 2)
                s->priority = 1;
            cout << "Priority: " << s->priority << endl;
        }

    }

    cout << "Set max correlation coefficient node." << endl;
    for (int i = 2; i < simulation.getLastModuleId(); i++) {
        Sensor *s1 = (Sensor *) simulation.getModule(i);
        cout << "Max corr node: " << s1->getId() - 2 << endl;
        double max_corr = 0;
        for (int j = 0; j < 54; j++) {
            Sensor *s2 = (Sensor *) simulation.getModule(j + 2);
            if (j == i - 2 || s2->isDead == true || s1->isDead == true)
                continue;
            else {

                if (max_corr < EntropyCorrCoeff[i - 2][j]) {
                    if (find_Node(s1, j)) {
                        max_corr = EntropyCorrCoeff[i - 2][j];
                        s1->max_corr_node = j;
                    }
                }
            }
        }
        cout << s1->max_corr_node << endl;
    }
}*/

bool BaseStation::find_Node(Sensor *s, int n) {
    for (int i = 0; i < s->myCluster->totalMembers; i++) {
        if (s->myCluster->memberNodes[i] == n + 2) {
            return true;
        }
    }
    return false;
}
//double BaseStation::CalculateAvgCoeff(double *EntropyCorrCoeff){
//    double result = 0;
//    int count = 0;
//    std::pair<pair<int, int>, double> temp;
//    for(unsigned_int i = 0; i < EntropyCorrCoeff.size(); i++){
//        temp = EntropyCorrCoeff[i];
//        if(temp.first.first == n){
//            result += temp.second;
//            count++;
//        }
//    }
//    return result/count;
//}

void BaseStation::finish() {
    cout << "Total Energy Lost: " << this->totalEnergyLost << endl;
    cout << "Mean Total Energy Lost: " << this->totalEnergyLost / 53 << endl;
    Sensor *s2 = (Sensor *) simulation.getModule(20);
    cout << "Init Energy:" << s2->energy << endl;
    cout << "Round: " << this->currentRound << endl;
    recordScalar("TotalEnergy: ", this->totalEnergyLost);

    std::ofstream deadNode;
    deadNode.open("DeadNode.txt");
    for (int i = 2; i < simulation.getLastModuleId(); i++) {
        Sensor *s = (Sensor *) simulation.getModule(i);
        deadNode << i - 1 << " " << s->roundDead << endl;
    }
    deadNode.close();
}

