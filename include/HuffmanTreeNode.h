#pragma once


#include <bitset>
#include <cstdint>
#include <iostream>

using namespace std;

class My_Node
{
public:
    My_Node *right;
    My_Node *left;

    u_char symbol;
    uint32_t amount;

    enum class Child
    {
        Left,
        Right
    };

    struct code_t
    {
        uint32_t bits_;
        uint8_t len;
    } my_code;

    uint32_t getAmount();

    My_Node(u_char symbol, uint32_t amount);

    My_Node(My_Node *new_left, My_Node *new_right);

    My_Node(uint32_t bits__, uint8_t len_, u_char symbol_, uint32_t amount_);

    void setCode(Child child, uint32_t prevCode, uint8_t deep);


    friend bool operator<(const My_Node &lhs, const My_Node &rhs)
    {
        if (lhs.amount < rhs.amount)
            return true;
        return false;
    }

    static void printNode(My_Node *node);
};