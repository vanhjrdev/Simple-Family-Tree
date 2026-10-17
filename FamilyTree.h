#pragma once
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

class FamilyNode;

class FamilyTree {
protected:
    FamilyNode* root;

public:
    FamilyTree();
    virtual ~FamilyTree();

    virtual void loadData(const string& filename);
    virtual void saveData(const string& filename);
    virtual void displayData() const;
    bool deleteNode(FamilyNode* node);
};