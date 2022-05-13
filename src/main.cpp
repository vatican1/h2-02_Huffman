#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <bitset>
#include <cstdint>
#include <map>
#include <cstring>

#include "HuffmanTreeNode.h"
#include "HuffmanTree.h"
#include "HuffmanArchiver.h"
#include "Options.h"

using namespace std;

struct OPT
{
    string finName;
    string foutName;
    CODE_OR_DECODE options;
};

OPT parse(int argc, char **argv)
{
    OPT opt = OPT();
    if (argc != 6)
    {
        // TODO throw
    }
    for (int i = 0; i != argc; i++)
    {

        if (strcmp(argv[i], "-c") == 0)
        {
            opt.options = CODE_OR_DECODE::TEXT_TO_CODE;
        }

        if (strcmp(argv[i], "-u") == 0)
        {
            opt.options = CODE_OR_DECODE::CODE_TO_TEXT;
        }

        if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "---files") == 0)
        {

            opt.finName = +argv[++i];
        }

        if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "-output") == 0)
        {
            opt.foutName += argv[++i];
        }
    }

    return opt;
}

bool compareTrees(My_Node *root1, My_Node *root2, bool isRoot)
{
    if ((root1->amount != root2->amount) ||
        (root1->right != root2->right) ||
        (root1->left != root2->left))
        return false;

    if (!(root1->left == nullptr || root1->right == nullptr))
    {
        compareTrees(root1->left, root2->left, false);
        compareTrees(root1->right, root2->right, false);
    }

    if (isRoot)
        return true;

    return false;
}

bool compareFiles(string fname1, string fname2)
{
    ifstream in1(fname2, ios::binary);
    ifstream in2(fname1, ios::binary);

    char firstFileChar, seccondFileChar;

    while (in1.read(&firstFileChar, sizeof(char)) && in2.read(&seccondFileChar, sizeof(char)))
    {
        if (firstFileChar != seccondFileChar)
        {
            in1.close();
            in2.close();
            return false;
        }
    }

    in2.read(&seccondFileChar, sizeof(char));
    if (in1.eof() != in2.eof())
    {
        in1.close();
        in2.close();
        return false;
    }

    in1.close();
    in2.close();

    return true;
}

int main(int argc, char **argv)
{

    OPT opt = parse(argc, argv);

    // cout << opt.finName << endl
    //      << opt.foutName << endl;

    HuffmanArchiver *arc = new HuffmanArchiver(opt.finName, opt.foutName, opt.options);

    delete arc;

    // cout << compareFiles(opt.finName, opt.foutName) << endl;

    return 0;
}
