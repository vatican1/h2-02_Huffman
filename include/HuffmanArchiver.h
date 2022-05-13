#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

#include "HuffmanTreeNode.h"
#include "HuffmanTree.h"
#include "Options.h"

class HuffmanArchiver
{
public:
    std::ifstream in;
    std::ofstream out;
    HuffmanTree *tree;

    int headerSize = 0;
    int textSize = 0;
    int inputTextSize = 0;

    std::vector<My_Node *> *vect;

    struct MyOptions
    {
        std::string inputFile;
        std::string outputFile;
        CODE_OR_DECODE codeOrDecode_;
    } options;

    HuffmanArchiver(std::string fInName, std::string fOutName, CODE_OR_DECODE codeOrDecode);

    ~HuffmanArchiver();

    void create_vector();


    void write_header(std::vector<My_Node *> *vect, My_Node *root);

    int parse_header(std::vector<My_Node *> *vect);
    struct SymbolCode
    {
        uint32_t bits_;
        uint8_t len;
        friend bool operator<(const SymbolCode &lhs, const SymbolCode &rhs)
        {
            if (lhs.bits_ < rhs.bits_)
                return true;
            return false;
        }
    };

    void write_text(std::vector<My_Node *> *my_vector2);

    void parse_text(My_Node *root, int surplus);

    void printNode(My_Node *node);
};
