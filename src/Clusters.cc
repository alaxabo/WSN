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

#include <src/Clusters.h>

Clusters::Clusters() {
    // TODO Auto-generated constructor stub
    this->totalMembers = 0;
   // this->totalActiveSleep = 0;
    this->root = new TreeNode();
    root->createNYT_TreeNode();
}

Clusters::~Clusters() {
    // TODO Auto-generated destructor stub
}

void Clusters::addNode(int id)
{
    this->memberNodes[this->totalMembers] = id;
    this->totalMembers += 1;
}

void Clusters::removeNode(int id)
{
    int i, j;
    for(i = 0; i < this->totalMembers; i++)
    {
        cout << "Node: " << this->memberNodes[i] << endl;
        if(this->memberNodes[i] == id)
        {
            for(j = i+1; j < this->totalMembers; j++)
            {
                this->memberNodes[j-1] = this->memberNodes[j];
            }
            this->totalMembers -= 1;
            cout << "Removed" << endl;
            break;
        }
    }
}

void Clusters::clearTree_Step(TreeNode *tree){
    if(tree == NULL) return;
    clearTree_Step(tree->l_child);
    clearTree_Step(tree->r_child);
    free(tree);
}

void Clusters::clearTree(){
    clearTree_Step(this->root->l_child);
    clearTree_Step(this->root->r_child);
    this->root->createNYT_TreeNode();
}

bool Clusters::hasMember(int id){
    for(int i; i < this->totalMembers; i++){
        if(this->memberNodes[i] == id){
            return true;
        }
    }
    return false;
}

void Clusters::removeAllNode(){
    this->totalMembers = 0;
}
