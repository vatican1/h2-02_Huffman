#include "HuffmanTreeNode.h"

#include <bitset>
#include <cstdint>
#include <iostream>

uint32_t My_Node::getAmount()
{
    return amount;
}

My_Node::My_Node(u_char symbol, uint32_t amount) : symbol(symbol), amount(amount)
{
    right = nullptr;
    left = nullptr;
};

My_Node::My_Node(My_Node *new_left, My_Node *new_right)
{
    left = new_left;
    right = new_right;
    amount = left->amount + right->amount;
    my_code.bits_ = (1 << 31) >> 31;
}

My_Node::My_Node(uint32_t bits__, uint8_t len_, u_char symbol_, uint32_t amount_)
{
    my_code.bits_ = bits__;
    my_code.len = len_;
    symbol = symbol_;
    amount = amount_;
    right = nullptr;
    left = nullptr;
};

void My_Node::setCode(Child child, uint32_t prevCode, uint8_t deep)
{
    my_code.len = deep + 1;
    my_code.bits_ = prevCode;
    if (child == Child::Right)
        my_code.bits_ |= 1 << (31 - deep);
}

void My_Node::printNode(My_Node *node)
{
    cout << " symbol - " << node->symbol << " amount - " << node->amount << " len - " << unsigned(node->my_code.len) << " bits - " << bitset<32>(node->my_code.bits_) << endl;
}
