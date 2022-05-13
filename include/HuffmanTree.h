#pragma once
#include "HuffmanTreeNode.h"
#include "Options.h"



#include <vector>
#include <algorithm>
#include <bitset>
#include <cstdint>


using namespace std;

class HuffmanTree
{
public:
    My_Node *root_;
    vector<My_Node *> leavesVector_;
    CODE_OR_DECODE codeOrDecode_;

    HuffmanTree();
    void printVect();

    HuffmanTree(vector<My_Node *> leavesVector, CODE_OR_DECODE codeOrDecode);
    ~HuffmanTree();

    void dfs(My_Node *root);

    My_Node *MakeTree();

    void linearInsert(My_Node *node, vector<My_Node *> *vect);
    void deleteVertex(My_Node *vertex);
};
