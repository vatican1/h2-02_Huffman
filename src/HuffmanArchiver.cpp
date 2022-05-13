#include "HuffmanArchiver.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <map>

using namespace std;

HuffmanArchiver::HuffmanArchiver(string fInName, string fOutName, CODE_OR_DECODE codeOrDecode)
{

    options.codeOrDecode_ = codeOrDecode;
    options.inputFile = fInName;
    options.outputFile = fOutName;

    in = ifstream(options.inputFile, ios::binary);
    out = ofstream(options.outputFile, ios::binary);
    vect = new vector<My_Node *>;

    if (!(in.is_open()) || !(out.is_open()))
    {
        // TODO throw
        //cout << "ERROR!" << endl;
        return;
    }

    if (codeOrDecode == CODE_OR_DECODE::TEXT_TO_CODE)
    {
        create_vector(); // это архивер

        tree = new HuffmanTree(*vect, CODE_OR_DECODE::TEXT_TO_CODE);

        write_header(&tree->leavesVector_, tree->root_);
        write_text(&tree->leavesVector_);
        // tree->printVect(); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }
    if (codeOrDecode == CODE_OR_DECODE::CODE_TO_TEXT)
    {
        int surplus = parse_header(vect);

        tree = new HuffmanTree(*vect, CODE_OR_DECODE::CODE_TO_TEXT);
        // tree->printVect();//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        parse_text(tree->root_, surplus);
    }

    cout << inputTextSize << endl
         << textSize << endl
         << headerSize << endl;
}

HuffmanArchiver::~HuffmanArchiver()
{
    in.close();
    out.close();
    delete vect;
    delete tree;
}

void HuffmanArchiver::create_vector()
{
    uint32_t symbols_freq[256]{0};

    char char_now;
    while (in.read(&char_now, 1))
    {
        symbols_freq[(u_char)char_now]++;
        inputTextSize += 1;
    }

    for (int i = 0; i < 256; i++)
    {
        if (symbols_freq[i] != 0)
            vect->push_back(new My_Node((u_char)i, symbols_freq[i]));
    }
}

void HuffmanArchiver::write_header(vector<My_Node *> *vect, My_Node *root)
{

    size_t vect_size = vect->size();
    out.write((char *)&vect_size, sizeof(size_t));
    out.write((char *)&root->amount, sizeof(size_t));
    headerSize += sizeof(size_t) * 2;

    for (auto i : *vect)
    {
        out.write((char *)&i->my_code.len, sizeof(i->my_code.len));
        out.write((char *)&i->my_code.bits_, sizeof(i->my_code.bits_));
        out.write((char *)&i->symbol, sizeof(i->symbol));
        out.write((char *)&i->amount, sizeof(i->amount));
        headerSize += sizeof(i->my_code.len) + sizeof(i->my_code.bits_) + sizeof(i->symbol) + sizeof(i->amount);
    }
}

int HuffmanArchiver::parse_header(vector<My_Node *> *vect)
{
    in.seekg(0);
    size_t vect_size;
    size_t surplus;
    in.read((char *)(&vect_size), sizeof(size_t));
    in.read((char *)(&surplus), sizeof(size_t));
    headerSize += sizeof(size_t) * 2;

    for (size_t i = 0; i < vect_size; i++)
    {
        uint8_t len;
        uint32_t bits_;
        u_char symbol;
        uint32_t amount;
        in.read((char *)&len, sizeof(uint8_t));
        in.read((char *)&bits_, sizeof(uint32_t));
        in.read((char *)&symbol, sizeof(u_char));
        in.read((char *)&amount, sizeof(uint32_t));
        vect->push_back(new My_Node(bits_, len, symbol, amount));
        headerSize += sizeof(len) + sizeof(bits_) + sizeof(symbol) + sizeof(amount);
    }
    return surplus;
}

int getIBitInCode(int i, uint32_t bits_)
{
    if (unsigned(((1 << (31 - i)) & bits_)) > 0)
        return 1;
    else
        return 0;
}

int getIBitIndecode(int i, u_char bits_)
{
    if (unsigned(((1 << (7 - i)) & bits_)) > 0)
        return 1;
    else
        return 0;
}


// будем заполнять по 1 байту
void HuffmanArchiver::write_text(vector<My_Node *> *my_vector2)
{
    map<u_char, SymbolCode> symbol_code;

    for (auto i : *my_vector2)
    {
        symbol_code.insert(make_pair(i->symbol, SymbolCode{i->my_code.bits_, i->my_code.len}));
    }

    in.clear();
    in.seekg(0);

    char letter;
    u_char bits_to_write = 0;
    int pos_to_write = 0;

    while (in.read(&letter, 1))
    {
        SymbolCode code = symbol_code[letter];
        for (int i = 0; i < code.len; i++)
        {
            if (pos_to_write == 8)
            {
                //cout << bitset<8>(bits_to_write) << endl;
                out.write((char *)&bits_to_write, sizeof(u_char));
                textSize += sizeof(char);
                pos_to_write = 0;
                bits_to_write = 0;
            }
            bits_to_write |= getIBitInCode(i, code.bits_) << (7 - pos_to_write);
            pos_to_write++;
        }
    }

    //cout << endl
        //  << bitset<8>(bits_to_write) << endl;
    out.write((char *)&bits_to_write, sizeof(char));
    textSize += sizeof(char);
}

void HuffmanArchiver::parse_text(My_Node *root, int surplus)
{
    u_char bits_;

    My_Node *vertex = root;

    out.clear();
    out.seekp(0);
    int k = 0;

    //cout << surplus << endl;

    while (in.read((char *)&bits_, sizeof(u_char)))
    {
        inputTextSize += sizeof(u_char);
        for (int i = 0; i < 8; i++)
        {
            if (getIBitIndecode(i, bits_) == 1)
            {
                if (vect->size() > 1)
                {
                    vertex = vertex->right;
                    //cout << 1 << flush;
                }
            }
            if (getIBitIndecode(i, bits_) == 0)
            {
                if (vect->size() > 1)
                {
                    vertex = vertex->left;
                    //cout << 0 << flush;
                }
            }

            if (vertex->right == nullptr || vertex->left == nullptr)
            {
                //cout << "|" << bitset<8>(bits_) << "|";
                //cout << vertex->symbol << endl;
                k++;
                textSize += sizeof(char);
                out.write((char *)&vertex->symbol, sizeof(char));
                if (surplus == k)
                    return;
                vertex = root;
            }
        }
        //cout << "->";
    }
}