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

#ifndef BASESTATION_H_
#define BASESTATION_H_

#include <omnetpp.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include "Sensor.h"
#include "Clusters.h"
#include "Definition.h"
#include "lib.h"
#include "HandleDataMessage.h"
#include "Message_m.h"

#define maxNode 56

class BaseStation: public cSimpleModule {
public:
    BaseStation();
    virtual ~BaseStation();
    int xpos, ypos;
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    void initNode();
    void resetAllCluster();
    Clusters **myClusters;
  protected:
    int posX[maxNode];
    int posY[maxNode];
    int k;
    int count;

    int currentRound;
    double trRange;
    int xMax;
    int yMax;
    int clusterNumber;
    int liveCluster;
    int totalMessage;
    double T;
    double totalEnergyLost;
    cLongHistogram energyLostStats;
    double EntropyCorrCoeff[54][54];
    double AvgEntropyCorrCoeff[54];
    int groupNumber;
    void createConnection(cModule *c1, cModule *c2);
    void sendInitMessage();
    void findClusterHead(int index);
    double getDistance(Sensor *s1, Sensor *s2);
    void removeDeadNode(int index);
    void removeCluster(int index);
    void setUpClusterHead(int i);
    void finish();
    void reClustering();
    std::vector<double> DataList[54];
    void setPriority(int groupNumber);
    bool find_Node(Sensor *s, int n);
    void printClusterStatus();
};

#endif /* BASESTATION_H_ */
