/*
 * group.cpp
 *
 *  Created on: Mar 8, 2016
 *      Author: Administrator
 */

#include "Group.h"
using namespace std;

float GROUP0 = 0.0;
float GROUP1[1] = {0.1};
float GROUP2[2] = {0.2,0.3};
float GROUP3[4] = {0.4,0.5,0.6,0.7};
float GROUP4[8] = {0.8,0.9,1,1.1,1.2,1.3,1.4,1.5};
float GROUP5[16] = {1.6,1.7,1.8,1.9,2,2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,3,3.1};
float GROUP6[32] = {3.2,3.3,3.4,3.5,3.6,3.7,3.8,3.9,4,4.1,4.2,4.3,4.4,4.5,4.6,4.7,
                    4.8,4.9,5,5.1,5.2,5.3,5.4,5.5,5.6,5.7,5.8,5.9,6,6.1,6.2,6.3};
float GROUP7[64] =  {
                    6.4,6.5,6.6,6.7,6.8,6.9,
                    7.0,7.1,7.2,7.3,7.4,7.5,7.6,7.7,7.8,7.9,
                    8.0,8.1,8.2,8.3,8.4,8.5,8.6,8.7,8.8,8.9,
                    9.0,9.1,9.2,9.3,9.4,9.5,9.6,9.7,9.8,9.9,
                    10.0,10.1,10.2,10.3,10.4,10.5,10.6,10.7,10.8,10.9,
                    11.0,11.1,11.2,11.3,11.4,11.5,11.6,11.7,11.8,11.9,
                    12.0,12.1,12.2,12.3,12.4,12.5,12.6,12.7
                    };
Group::Group() {
    // TODO Auto-generated constructor stub

}

Group::~Group() {
    // TODO Auto-generated destructor stub
}
int Group::check(float diff, float *group, int size){
    for(int i = 0; i < size; i++){
        if(diff == group[i]) return i;
    }
    return -1;
}

float Group::absFloat(float diff){
    if(diff >= 0)
        return diff;
    else
        return -diff;
}

void Group::fromDiffToGroup(float diff) {
    float Diff = absFloat(diff);
    if(Diff == 0.0){
        this->group = GROUP_0;
        this->difference.push_back(0.0);
    }
    if(Diff == (float)0.1){
        this->group = GROUP_1;
        this->difference.push_back(0.1);
    }
    if(check(Diff, GROUP2, 2) != -1){
        this->group = GROUP_2;
        for(int i = 0; i < 2; i++){
            this->difference.push_back(GROUP2[i]);
        }
    }
    if(check(Diff, GROUP3, 4) != -1){
        this->group = GROUP_3;
        for(int i = 0; i < 4; i++){
            this->difference.push_back(GROUP3[i]);
        }
    }
    if(check(Diff, GROUP4, 8) != -1){
        this->group = GROUP_4;
        for(int i = 0; i < 8; i++){
            this->difference.push_back(GROUP4[i]);
        }
    }
    if(check(Diff, GROUP5, 16) != -1){
        this->group = GROUP_5;
        for(int i = 0; i < 16; i++){
            this->difference.push_back(GROUP5[i]);
        }
    }
    if(check(Diff, GROUP6, 32) != -1){
        this->group = GROUP_6;
        for(int i = 0; i < 32; i++){
            this->difference.push_back(GROUP6[i]);
        }
    }
    if(check(Diff, GROUP7, 64) != -1){
        this->group = GROUP_7;
        for(int i = 0; i < 64; i++){
            this->difference.push_back(GROUP7[i]);
        }
    }
}

float Group::getDataByIndex(int index, Group g){
    if(index >= g.difference.size()){
        return g.difference[index - g.difference.size()];
    }
    else{
        return -g.difference[g.difference.size() - index - 1];
    }
}
//int main(){
//    Group *g = new Group();
//    g->fromDiffToGroup(1.8);
//    cout << g->group << endl;
//    int index = 11;
//    float data = g->getDataByIndex(index,*g);
//    cout << data << endl;
//}
