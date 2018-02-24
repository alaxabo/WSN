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

#ifndef SENSOR_H_
#define SENSOR_H_

#include <omnetpp.h>
#include <Message_m.h>
#include "Definition.h"
#include "Clusters.h"
#include "Compression/HuffmanTree.h"
using namespace std;

class Sensor: public cSimpleModule
{
    public:
        Sensor();
        virtual ~Sensor();
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
        void setupPhase();
        void setupCluster();
        void sendMessage(cMessage *cmsg, int destination);
        void energyLostIn_CH(double l_receive, double dToBS, int l_send);
        void energyLostIn_NonCH(double l, double dToCH);
        double getDistance(Sensor *s1, Sensor *s2);

        Clusters *myCluster;
        double entropy;
        int name;
        int xpos;
        int ypos;
        int totalMessage;
        double energy;
        int count;
        int currentRound;
        int clusterheadId;
        Sensor *clusterhead;
        int timeslot;
        bool isDead;
        bool isCH;
        bool isLiveThisRound;
        int priority;
        int random;
        int number_Msg_To_CH;
        double distance_To_BS;
        std::vector<DataMessage> messageList;
        std::vector<DataMessage> messageListToBS;
        int max_corr_node;
        int connect;
        vector<int> notConnect;
        double energyLost;
        int roundDead;
        float previousData;
        double DataMsg_Length;
};

#endif /* SENSOR_H_ */
