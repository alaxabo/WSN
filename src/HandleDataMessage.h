/*
 * DataMessage.h
 *
 *  Created on: Nov 18, 2015
 *      Author: Administrator
 */

#ifndef DATAMESSAGE_H_
#define DATAMESSAGE_H_

#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* strtof */
#include "Message_m.h"
#include <sstream>
#include <vector>
using namespace std;

class HandleDataMessage {
    public:
    std::vector<DataMessage> createDataMsgList(int node);
    void saveDataMsg(DataMessage msg, int node);
};

#endif /* DATAMESSAGE_H_ */
