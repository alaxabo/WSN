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

#include <src/Sensor.h>
#include "BaseStation.h"
#include "Definition.h"
#include <string.h>
#include <string>
#include <fstream>

using namespace std;

Define_Module(Sensor);



Sensor::Sensor() {
    // TODO Auto-generated constructor stub
    this->isDead = false;
    this->isCH = false;
    this->isLiveThisRound = true;
    this->totalMessage = 0;
    this->priority = 1;

}

Sensor::~Sensor() {
    // TODO Auto-generated destructor stub
}

void Sensor::initialize() {
    this->energy = 1000000000;
    this->energyLost = 0;
    this->timeslot = 100;
    this->connect = 0;
    this->DataMsg_Length=4000;
    HandleDataMessage s;
        this->messageList = s.createDataMsgList(this->getId()-1);
    if ((this->energy - this->energyLost) <= 201000.0||this->isDead == true) {
        this->isDead = true;
        this->roundDead = 0;
    }
}

void Sensor::handleMessage(cMessage *msg) {
    Enter_Method
    ("handleMessage(cMessage *msg)");
    if (msg->isSelfMessage()) {
        if (msg->getKind() == SELF_DATA_TO_CH) {
            double distance = this->getDistance(this, this->clusterhead);
            //this->energySend(DataMsg_Length, distance);
            this->energyLostIn_NonCH(DataMsg_Length, distance); //Nang luong tieu thu khi truyen tin toi CH cua non-CH
            DataMessage *temp = new DataMessage("Data to CH");
            temp->setKind(DATA_TO_CH);
            temp->setSource(this->getId());
            temp->setData(this->messageList.begin()->getData());
            sendMessage(temp, this->clusterheadId);
            if ((this->energy - this->energyLost) <= 201000.0||this->isDead == true) {
                this->isDead = true;
                this->roundDead = currentRound;
            }
            this->messageList.erase(this->messageList.begin());
        }
    }
    else {
        if (msg->getKind() == SMSG_INIT) {
            cout << "Handle SMSG_INIT message" << endl;
            //this->energyReceive(HeadMsgLength);

            //double clusterEnergy[this->myCluster->totalMembers];

            this->count = 0;
            this->connect = 0;
            Message *myMsg = (Message*) msg;
            this->myCluster = (Clusters*) myMsg->getContextPointer();
            this->currentRound = myMsg->getCurrentRound();
            this->notConnect.clear();

            double * clusterMembersEnergy = (double *) malloc ((this->myCluster->totalMembers)*sizeof(double));

            for(int i = 0; i < this->myCluster->totalMembers; i++){
               Sensor *s = (Sensor *)simulation.getModule(this->myCluster->memberNodes[i]);
               clusterMembersEnergy[i] = s->energy - s->energyLost;
            }

            std::sort(clusterMembersEnergy, clusterMembersEnergy + this->myCluster->totalMembers);

            for(int i = 0; i < this->myCluster->totalMembers; i++){
                Sensor *s = (Sensor *)simulation.getModule(this->myCluster->memberNodes[i]);
                //cout << "Priority node " << s->getId()-2 << ": " << s->priority << endl;
                s->random = rand();
                if(s->getId() != this->getId()){
                    cout << "Id: " << s->getId() << " Priority: " << s->priority << endl;
                    if(s->priority == 1){
                        this->connect += 1;
                        s->getDisplayString().setTagArg("i",1,this->myCluster->color);
                        //this->getDisplayString().setTagArg("i",1,"brown");
                        //cout << s->random << " Node " << s->getId()-2 << " connect to header " << this->getId()-2 <<" in round " << this->currentRound << " with priority " << s->priority << endl;
                        s->isLiveThisRound = true;
                    }
                    else if(s->priority == 2){
                        int length = sizeof(clusterMembersEnergy)/sizeof(double);
                        double threshold = clusterMembersEnergy[length/3];
                        double energy = s->energy - s->energyLost;

                        if(energy < threshold){
                            this->connect += 0;
                            this->notConnect.push_back(s->getId());
                            s->getDisplayString().setTagArg("i",1,"gray");
                            s->isLiveThisRound = false;
                        } else {
                            this->connect += 1;
                            s->getDisplayString().setTagArg("i",1,this->myCluster->color);
                            s->isLiveThisRound = true;
                        }

//                        if(s->random % 3 == 0){
//                            this->connect += 0;
//                            this->notConnect.push_back(s->getId());
//                            s->getDisplayString().setTagArg("i",1,"gray");
//                            //cout << s->random << " Node " << s->getId()-2 << " not connect to header " << this->getId()-2 <<" in round " << this->currentRound <<  " with priority " << s->priority << endl;
//                            s->isLiveThisRound = false;
//                        }
//                        else{
//                            this->connect += 1;
//                            s->getDisplayString().setTagArg("i",1,this->myCluster->color);
//                            //cout << s->random << " Node " << s->getId()-2 << " connect to header " << this->getId()-2 <<" in round " << this->currentRound <<  " with priority " << s->priority << endl;
//                            s->isLiveThisRound = true;
//                        }
                    }
                    else if(s->priority == 3){
                       int length = sizeof(clusterMembersEnergy)/sizeof(double);
                       double threshold = clusterMembersEnergy[length/2];
                       double energy = s->energy - s->energyLost;

                       if(energy < threshold){
                           this->connect += 0;
                           this->notConnect.push_back(s->getId());
                           s->getDisplayString().setTagArg("i",1,"gray");
                           s->isLiveThisRound = false;
                       } else {
                           this->connect += 1;
                           s->getDisplayString().setTagArg("i",1,this->myCluster->color);
                           s->isLiveThisRound = true;
                       }

//                        if(s->random % 2 == 0){
//                            this->connect += 0;
//                            this->notConnect.push_back(s->getId());
//                            s->getDisplayString().setTagArg("i",1,"gray");
//                            //cout << s->random << " Node " << s->getId()-2 << " not connect to header " << this->getId()-2 <<" in round " << this->currentRound <<  " with priority " << s->priority <<endl;
//                            s->isLiveThisRound = false;
//                        }
//                        else{
//                            this->connect += 1;
//                            s->getDisplayString().setTagArg("i",1,this->myCluster->color);
//                            //cout << s->random << " Node " << s->getId()-2 << " connect to header " << this->getId()-2 <<" in round " << this->currentRound <<  " with priority " << s->priority << endl;
//                            s->isLiveThisRound = true;
//                        }
                    }
                }
            }
            cout << this->connect << " nodes connect to header " << this->getId() << endl;
            this->setupCluster();
            DataMessage *tmp = new DataMessage();
            if(this->connect == 0){
            //EV << "Nhan duoc " << this->count << "/" << this->myCluster->totalMembers << " goi tin" << endl;
                //Send data to BS
                cout << "Create data message of CH node" << endl;
                DataMessage *temp1 = new DataMessage("Data to CH");
                int bsID = simulation.getLastModuleId();
                DataToBSMsg *temp = new DataToBSMsg("Data to BS");
                temp1->setSource(this->getId());
                temp1->setData(this->messageList.begin()->getData());
                temp1->setKind(DATA_TO_CH);
                temp1->setCurrentRound(this->currentRound);
                this->messageListToBS.push_back(*temp1);
                this->messageList.erase(this->messageList.begin());
                //cout << "Disconnect nodes of header :" << this->getId() << endl;
//                for(int i = 0; i < this->notConnect.size(); i++){
//                    cout << this->notConnect[i]-2 << endl;
//                }
                for(int i = 0; i < this->notConnect.size(); i++){
                    int a = this->notConnect[i];
                    Sensor *s1 = (Sensor *)simulation.getModule(a);
                    Sensor *s2 = (Sensor *)simulation.getModule(s1->max_corr_node+2);
                    DataMessage *temp2 = new DataMessage();
                    //cout << "Data from disconnect node " << s1->getId()-2 << " is data of node " << s2->getId()-2 << endl;
                    temp2->setSource(s1->getId());
                    temp2->setData(s2->messageList.begin()->getData());
                    temp2->setKind(DATA_TO_CH);
                    //cout << temp2->getData() << endl;
                    this->messageListToBS.push_back(*temp2);
                }
                temp->setSource(this->getId());
                temp->setKind(DATA_TO_BS);
                temp->setDataToBSArraySize(this->messageListToBS.size());
                temp->setCurrentRound(this->currentRound);
                for(int i = 0; i < this->messageListToBS.size(); i++){
                    temp->setDataToBS(i,this->messageListToBS[i]);
                }

                DataToBSEncodeMsg* encodeMsg = new DataToBSEncodeMsg("Data To BS");
                string code;
                std::vector<float> curData;
                HuffmanTree *tree = new HuffmanTree();
                encodeMsg->setSourceArraySize(this->messageListToBS.size());
                for(unsigned int i = 0; i < this->messageListToBS.size(); i++){
                    encodeMsg->setSource(i,this->messageListToBS[i].getSource());
                    curData.push_back((float)this->messageListToBS[i].getData());
                }
                if(this->currentRound % ROUND_TO_RECLUSTER == 0){
                    this->previousData = curData.front();
                }

                code = tree->encoder(curData, this->myCluster->root, this->previousData);
                encodeMsg->setCode(code.c_str());

                encodeMsg->setKind(DATA_TO_BS);

                int size = code.size() + sizeof(curData);

                if(this->currentRound % 256 == 0){
                    ofstream file;
                    file.open("Compression.txt", ios::app);
                    file << this->currentRound << "\t" << this->getId() -1 << "\t"
                            << this->myCluster->totalMembers << "\t" << size << endl;
                    /*file << "Round: " << this->currentRound << endl;
                    file << "HEAD: " << this->getId() -1 << endl;
                    file << "So thanh vien: " << this->myCluster->totalMembers << endl;
                    file << "Size: " << size << endl;*/
                }

                this->previousData = curData.back();


                this->messageListToBS.clear();

                BaseStation *BS = (BaseStation*) simulation.getModule(bsID);
                double xlength = abs(this->xpos - BS->xpos);
                double ylength = abs(this->ypos - BS->ypos);
                double distanceToBS = sqrt(xlength * xlength + ylength * ylength);
                //this->energySend(DataMsg_Length, distance);DataMsg_Length * (this->connect+1)
                this->energyLostIn_CH(DataMsg_Length*this->connect, distanceToBS, size);
                if ((this->energy - this->energyLost) <= 201000.0||this->isDead == true) {
                    this->isDead = true;
                    this->roundDead = currentRound;
                }
                sendMessage(temp, bsID);
            }

        } else if (msg->getKind() == TDMA_INIT) {
            //this->energyReceive(HeadMsgLength);
            this->clusterheadId = msg->getSenderModuleId();
            this->clusterhead = (Sensor*) msg->getSenderModule();
            int frame = ((TDMASchedule*) msg)->getFrame();
            this->currentRound = ((TDMASchedule*) msg)->getCurrentRound();
            cMessage *cmsg = new cMessage("DataToCH");
            cmsg->setKind(SELF_DATA_TO_CH);
            scheduleAt(simTime() + frame * this->timeslot, cmsg);
        } else if (msg->getKind() == DATA_TO_CH) {
            //this->energyReceive(DataMsg_Length);
            this->count += 1;
            DataMessage *temp1 = (DataMessage*) msg;
            this->messageListToBS.push_back(*temp1);
            if(count == this->connect){
            cout << "Nhan duoc " << this->count << "/" << this->myCluster->totalMembers << " goi tin" << endl;
                //Send data to BS
                int bsID = simulation.getLastModuleId();
                DataToBSMsg* temp = new DataToBSMsg("Data to BS");
                temp1->setSource(this->getId());
                temp1->setData(this->messageList.begin()->getData());
                temp1->setKind(DATA_TO_CH);
                this->messageListToBS.push_back(*temp1);
                this->messageList.erase(this->messageList.begin());
                cout << "Disconnect nodes of header :" << this->getId() << endl;
                for(int i = 0; i < this->notConnect.size(); i++){
                    cout << this->notConnect[i]-2 << endl;
                }
                for(int i = 0; i < this->notConnect.size(); i++){
                    int a = this->notConnect[i];
                    Sensor *s1 = (Sensor *)simulation.getModule(a);
                    Sensor *s2 = (Sensor *)simulation.getModule(s1->max_corr_node+2);
                    DataMessage *temp2 = (DataMessage*) msg;
                    //cout << "Data from disconnect node " << s1->getId()-2 << " is data of node " << s2->getId()-2 << endl;
                    temp2->setSource(s1->getId());
                    temp2->setData(s2->messageList.begin()->getData());
                    temp2->setKind(DATA_TO_CH);
                    //cout << temp2->getData() << endl;
                    this->messageListToBS.push_back(*temp2);
                }

                temp->setSource(this->getId());
                temp->setKind(DATA_TO_BS);
                temp->setDataToBSArraySize(this->messageListToBS.size());
                temp->setCurrentRound(this->currentRound);

                DataToBSEncodeMsg* encodeMsg = new DataToBSEncodeMsg("Data To BS");
                string code;
                std::vector<float> curData;
                HuffmanTree *tree = new HuffmanTree();
                encodeMsg->setSourceArraySize(this->messageListToBS.size());
                for(unsigned int i = 0; i < this->messageListToBS.size(); i++){
                    encodeMsg->setSource(i,this->messageListToBS[i].getSource());
                    curData.push_back((float)this->messageListToBS[i].getData());
                }
                if(this->currentRound % ROUND_TO_RECLUSTER == 0){
                    this->previousData = curData.front();
                }
                code = tree->encoder(curData, this->myCluster->root, this->previousData);

                int size = code.size() + sizeof(curData);

                if(this->currentRound % 256 == 0){
                    ofstream file;
                    file.open("Compression.txt", ios::app);
                    file << this->currentRound << "\t" << this->getId() -1 << "\t"
                            << this->myCluster->totalMembers << "\t" << size << endl;
                    /*file << "Round: " << this->currentRound << endl;
                    file << "HEAD: " << this->getId() -1 << endl;
                    file << "So thanh vien: " << this->myCluster->totalMembers << endl;
                    file << "Size: " << size << endl;*/
                }

                encodeMsg->setCode(code.c_str());
                this->previousData = curData.back();
                encodeMsg->setKind(DATA_TO_BS);
                for(int i = 0; i < this->messageListToBS.size(); i++){
                    temp->setDataToBS(i,this->messageListToBS[i]);

                }
                this->messageListToBS.clear();
                BaseStation *BS = (BaseStation*) simulation.getModule(bsID);
                double xlength = abs(this->xpos - BS->xpos);
                double ylength = abs(this->ypos - BS->ypos);
                double distanceToBS = sqrt(xlength * xlength + ylength * ylength);
                //this->energySend(DataMsg_Length, distance);DataMsg_Length * (this->connect+1)
                this->energyLostIn_CH(DataMsg_Length*this->connect, distanceToBS, size);
                if ((this->energy - this->energyLost) <= 201000.0||this->isDead == true) {
                    this->isDead = true;
                    this->roundDead = currentRound;
                }

                sendMessage(temp, bsID);

            }
        }
    }

}

void Sensor::setupCluster() {
    Enter_Method
    ("setupCluster()");
    int frame = 0;
    this->count = 0;
    this->bubble("I'm cluster head");
    cout << "Setup Cluster TDMA message" << endl;
    for (cModule::GateIterator i(this); !i.end(); i++) {
        cGate *gate = i();
        if (gate->getType() == cGate::OUTPUT) {
            cGate *inGate = gate->getPathEndGate();
            Sensor *s = (Sensor*) inGate->getOwnerModule();
            for (int j = 0; j < this->myCluster->totalMembers; j++) {
                if (s->getId() == this->myCluster->memberNodes[j]) {
                    if(s->isLiveThisRound == true){
                        //Send TDMA Schedule
                        s->bubble("I'm active node");
                        frame += 1;
                        //double distance = this->getDistance(this, s);
                        //this->energySend(HeadMsgLength, distance);
                        TDMASchedule *tdmaMsg = new TDMASchedule();
                        tdmaMsg->setKind(TDMA_INIT);
                        tdmaMsg->setFrame(frame);
                        tdmaMsg->setCurrentRound(this->currentRound);
                        send(tdmaMsg, gate);
                    }
                }
            }
        }
    }
}

void Sensor::sendMessage(cMessage *cmsg, int destination) {
    for (cModule::GateIterator i(this); !i.end(); i++) {
        cGate *gate = i();
        if (gate->getType() == cGate::OUTPUT) {
            cGate *inGate = gate->getPathEndGate();
            cModule *c = inGate->getOwnerModule();
            int id = c->getId();
            if (destination == id) {
                send(cmsg, gate);
                this->totalMessage += 1;
                break;
            }
        }
    }
}

void Sensor::energyLostIn_CH(double l_receive, double dToBS, int l_send) {
    Enter_Method_Silent
    ("energyLostIn_CH");
    // l bit
    double lostE = l_receive * Eelec + l_send * Efs * (dToBS * dToBS) + l_send * Eelec; // = energy dissipated in the CH
    this->energyLost += lostE;

}


void Sensor::energyLostIn_NonCH(double l, double dToCH) {
    Enter_Method_Silent
    ("energyLostIn_NonCH");
    // l bit
    double lostE = l * Eelec + l * Efs * (dToCH * dToCH); // = energy dissipated in the non-CH
    this->energyLost += lostE;

}

double Sensor::getDistance(Sensor *s1, Sensor *s2) {
    double xlength = abs(s1->xpos - s2->xpos);
    double ylength = abs(s1->ypos - s2->ypos);
    double distance = sqrt(xlength * xlength + ylength * ylength);
    return distance/4;
}

