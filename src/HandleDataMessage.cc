/*
 * DataMessage.cc
 *
 *  Created on: Nov 18, 2015
 *      Author: Administrator
 */

#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>

#include "HandleDataMessage.h"
using namespace std;

std::vector<DataMessage> HandleDataMessage::createDataMsgList(int node){
    std::stringstream ss;
    ss << "../Temperature/" << node << ".txt";
    std::string str = ss.str();
    std::vector<DataMessage> myVector;
    std::ifstream infile;
    std::string line;
    infile.open(str);
    if(infile.is_open())
        cout << "File" << str << "opened" << endl;
    else
        cout << "Cannot open file " << str << endl;
    while(std::getline(infile, line ))
    {
        DataMessage msg;
        double data = strtod(line.c_str(),NULL);
        msg.setData(data);
        msg.setSource(node+1);
        myVector.push_back(msg);
    }
    infile.close();
    cout << "Done file " << node << endl;

    return myVector;
}

void HandleDataMessage::saveDataMsg(DataMessage msg, int node){
    std::stringstream ss;
    ss << "OutFile" << msg.getCurrentRound() << node << ".txt\\";
    std::string str = ss.str();
    std::ofstream outfile(str);
    outfile << msg.getData() << std::endl;
    outfile.close();
}


