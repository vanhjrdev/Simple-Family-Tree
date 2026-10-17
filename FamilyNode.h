#pragma once
#include "FamilyTree.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

struct Couple {
    string name;
    Couple* next;

    Couple(string n) {
        name = n;
        next = nullptr;
    }
};
class FamilyTree;
struct FamilyPathNode;

class FamilyNode : public FamilyTree{
    friend class FamilyTree;
    friend void saveNode(ofstream& os, FamilyNode* node);
    friend FamilyNode* loadNode(ifstream& is);
    friend void displayNode(FamilyNode* node, string prefix, bool isLast);
    friend string relationWord(FamilyNode* child);
private:
    string fullName;
    string parentRelation;
    int birthYear;
    int deathYear;

    Couple* coupleList;
    FamilyNode* firstChild;
    FamilyNode* nextSibling;
    static FamilyPathNode* dfsHelper(FamilyNode* node, const string& name, FamilyPathNode* pathSoFar);
public:

    FamilyNode* getRoot();
    FamilyNode* findNode(const string& name);
    FamilyNode();
    FamilyNode(string name, string relation, int bY, int dY);
    bool update(string name, string relation, int bY, int dY);
    bool addChild(FamilyNode* child);
    FamilyNode* addCouple(string name);
    bool deleteCouple(string name);
    bool deleteChild(FamilyNode* child);
    bool deleteNode(FamilyNode* node);
    FamilyPathNode* findPathBFS(const string& name);
    FamilyPathNode* findPathDFS(const string& name);
    FamilyNode* findAncestor(const string& name);
    void printRelationships(const string& name);

    ~FamilyNode();
};