/*
 * Group.h
 *
 *  Created on: Mar 8, 2016
 *      Author: Administrator
 */

#ifndef GROUP_H_
#define GROUP_H_

#include <vector>
#include "Definition.h"
#include <iostream>
#include <math.h>
class Group {

public:
    int group;
    std::vector<float> difference;
    Group();
    virtual ~Group();
    int check(float diff, float *group, int size);
    float absFloat(float diff);
    void fromDiffToGroup(float diff);
    float getDataByIndex(int index, Group g);
};

#endif /* GROUP_H_ */
