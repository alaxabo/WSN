/*
 * HuffmanTree.h
 *
 *  Created on: Mar 8, 2016
 *      Author: Administrator
 */

#ifndef HUFFMANTREE_H_
#define HUFFMANTREE_H_

#include "TreeNode.h"
#include <iostream>
#include <iomanip>
#include <string.h>
#include <sstream>
#include <tgmath.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>

using namespace std;

class HuffmanTree {
public:
    TreeNode *tree;
    HuffmanTree();
    virtual ~HuffmanTree();
    void initialize(TreeNode *tree);
    TreeNode* search(TreeNode *tree,TreeNode *node);
    TreeNode* search_nyt(TreeNode *tree);
    void buildCode(TreeNode *tree);
    void printCode(TreeNode *tree);
    std::string ConvertToBinary(int n);
    int ConvertToDecima(std::string BinaryCode);
    std::string ConvertToBCD(float diff);
    float BCDtoDecima(std::string BCDcode);
    std::string traverse(TreeNode *tree, float diff);
    std::string suffixCode(float diff);
    void addNode(TreeNode *tree, float diff);
    int reBalance_Step(TreeNode *tree);
    void reBalance(TreeNode *tree);
    void printTree(TreeNode *tree);
    float* createDiffArr(std::vector<float> currentData, float previousData);
    string encoder(std::vector<float> Data, TreeNode *root, float previousData);
    std::vector<float> decoder(string code, TreeNode *root, float preData);

};

#endif /* HUFFMANTREE_H_ */
