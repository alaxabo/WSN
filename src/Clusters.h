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

#ifndef CLUSTERS_H_
#define CLUSTERS_H_

#include "Compression/HuffmanTree.h"
#include "lib.h"

class Clusters {
public:
    Clusters();
    virtual ~Clusters();
    int memberNodes[64];
    int totalMembers;
    int clusterhead;
    char* color;
    TreeNode *root;

//    int iActiveSleep[64][64];
//    int totalActiveSleep;
    void addNode(int id);
    void removeNode(int id);
    void clearTree_Step(TreeNode *tree);
    void clearTree();
    bool hasMember(int id);
    void removeAllNode();
    double ClusterJentropy();
};

#endif /* CLUSTERS_H_ */
