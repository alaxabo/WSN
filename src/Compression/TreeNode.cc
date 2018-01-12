/*
 * TreeNode->cpp
 *
 *  Created on: Mar 8, 2016
 *      Author: Administrator
 */

#include "TreeNode.h"
using namespace std;
TreeNode::TreeNode() {
    // TODO Auto-generated constructor stub
    //this->group.group = -1;
    this->flag = COMP_NODE;
    this->weight = -1;
}

TreeNode::~TreeNode() {
    // TODO Auto-generated destructor stub
}

void TreeNode::createNYT_TreeNode(){
    this->flag = NYT_NODE;
    this->weight = 0;
    this->l_child = 0;
    this->r_child = 0;
    //this->huffmanCode = 0;
    this->group.group = -1;
}

void TreeNode::createNRM_TreeNode(float diff){
    this->flag = NRM_NODE;
//    //cout << "Flag" << NRM_NODE << endl;
    this->weight = 1;
//    //cout << "Weight" << this->weight << endl;
    this->l_child = 0;
    this->r_child = 0;
//    //cout << "Children" << this->l_child << this->r_child << endl;
    this->group.fromDiffToGroup(diff);
    //this->huffmanCode = 0;
    //    //cout << "Group" << this->group.group << endl;
}

//int main(){
//    TreeNode *n = new TreeNode();
//    n->createNRM_TreeNode(0.3);
//    //cout << "Node " << n->group.group << endl;
//}

