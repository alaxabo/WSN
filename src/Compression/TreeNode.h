/*
 * TreeNode.h
 *
 *  Created on: Mar 8, 2016
 *      Author: Administrator
 */

#ifndef TREENODE_H_
#define TREENODE_H_

#include "Definition.h"
#include "Group.h"
#include <iostream>
#include <string.h>
class TreeNode {
protected:
public:
    Group group;
    int weight;
    int flag;
    TreeNode *l_child;
    TreeNode *r_child;
    std::string huffmanCode;
    TreeNode();
    virtual ~TreeNode();
    void createNYT_TreeNode();
    void createNRM_TreeNode(float diff);
    void updateWeight();
};

#endif /* TREENODE_H_ */
