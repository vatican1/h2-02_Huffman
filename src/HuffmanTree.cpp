#include "HuffmanTree.h"
#include "HuffmanTreeNode.h"

#include <vector>
#include <algorithm>
#include <bitset>
#include <cstdint>

HuffmanTree::HuffmanTree() {}

void HuffmanTree::printVect()
{
    for (auto i : leavesVector_)
    {
        My_Node::printNode(i);
    }
}

HuffmanTree::HuffmanTree(vector<My_Node *> leavesVector, CODE_OR_DECODE codeOrDecode) : root_(nullptr), leavesVector_(leavesVector)
{
    codeOrDecode_ = codeOrDecode;
    if (codeOrDecode_ == CODE_OR_DECODE::TEXT_TO_CODE)
    {
        if (leavesVector_.size() > 1)
        {
            sort(leavesVector_.begin(), leavesVector_.end(), [](My_Node *first, My_Node *seccond)
                 { return first->getAmount() > seccond->getAmount(); });

            root_ = MakeTree();
        }
        else
        {
            root_ = leavesVector_.back();
        }

        root_->my_code.bits_ = 0;
        root_->my_code.len = 0;
        dfs(root_);
    }

    if (codeOrDecode_ == CODE_OR_DECODE::CODE_TO_TEXT)
    {
        if (leavesVector_.size() > 1)
        {
            root_ = MakeTree();
        }
        else
        {
            root_ = leavesVector_.back();
        }
    }
}

HuffmanTree::~HuffmanTree()
{
    deleteVertex(root_);
}

void HuffmanTree::dfs(My_Node *root)
{
    if (!(root->left == nullptr || root->right == nullptr))
    {
        root->left->setCode(My_Node::Child::Left, root->my_code.bits_, root->my_code.len);
        root->right->setCode(My_Node::Child::Right, root->my_code.bits_, root->my_code.len);
        dfs(root->left);
        dfs(root->right);
    }
}

My_Node *HuffmanTree::MakeTree()
{
    vector<My_Node *> leavesVector_tmp = leavesVector_;

    while (leavesVector_tmp.size() != 1)
    {
        My_Node *right = leavesVector_tmp.back();
        leavesVector_tmp.pop_back();
        My_Node *left = leavesVector_tmp.back();
        leavesVector_tmp.pop_back();
        My_Node *node = new My_Node(left, right);
        linearInsert(node, &leavesVector_tmp);
    }

    return leavesVector_tmp.back();
}

void HuffmanTree::linearInsert(My_Node *node, vector<My_Node *> *vect)
{

    for (auto i = vect->begin(); i <= vect->end(); i++)
    {
        if (i == vect->end() || (*i)->amount < node->amount)
        {
            vect->insert(i, node);
            break;
        }
    }
}

void HuffmanTree::deleteVertex(My_Node *vertex)
{
    if (!(vertex->left == nullptr || vertex->right == nullptr))
    {
        deleteVertex(vertex->left);
        deleteVertex(vertex->right);
    }
    delete vertex;
}