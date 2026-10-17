#include "FamilyTree.h"
#include "FamilyNode.h"
#include <iostream>

using namespace std;

FamilyTree::FamilyTree() {
    root = nullptr;
}

FamilyTree::~FamilyTree() {
    delete root;
}

void saveNode(ofstream& os, FamilyNode* node) {
    if (node == nullptr) {
        os << "#" << '\n';
        return;
    }

    os << "@" << '\n';
    os << node->fullName << '\n';
    os << node->parentRelation << '\n';
    os << node->birthYear << '\n';
    os << node->deathYear << '\n';
    int coupleCount = 0;
    Couple* c = node->coupleList;
    while (c != nullptr) {
        coupleCount++;
        c = c->next;
    }
    os << coupleCount << '\n';
    c = node->coupleList;
    while (c != nullptr) {
        os << c->name << '\n';
        c = c->next;
    }
    saveNode(os, node->firstChild);
    saveNode(os, node->nextSibling);
}

void FamilyTree::saveData(const std::string& filename) {
    ofstream os(filename);
    saveNode(os, root);
    os.close();
}

FamilyNode* loadNode(ifstream& is) {
    string marker;
    getline(is, marker);

    if (marker != "@") {
        return nullptr;
    }
    string fullName, relation;
    getline(is, fullName);
    getline(is, relation);
    int bY, dY;
    is >> bY;
    is >> dY;
    is.ignore();
    FamilyNode* node = new FamilyNode(fullName, relation, bY, dY);
    int coupleCount;
    is >> coupleCount;
    is.ignore();
    Couple* tail = nullptr;
    for (int i = 0; i < coupleCount; i++) {
        string cname;
        getline(is, cname);
        Couple* newCouple = new Couple(cname);
        if (tail == nullptr) {
            node->coupleList = newCouple;
        } else {
            tail->next = newCouple;
        }
        tail = newCouple;
    }
    node->firstChild = loadNode(is);
    node->nextSibling = loadNode(is);

    return node;
}

void displayNode(FamilyNode* node, string prefix, bool isLast) {
    if (node == nullptr) return;

    // Vẽ cành cho node hiện tại
    cout << prefix;
    cout << (isLast ? "`-- " : "|-- ");

    cout << node->fullName << " (" << node->parentRelation << ") "
         << node->birthYear << " - "
         << (node->deathYear == 0 ? "con song" : to_string(node->deathYear));

    if (node->coupleList != nullptr) {
        cout << " | Vo/Chong: ";
        Couple* c = node->coupleList;
        while (c != nullptr) {
            cout << c->name;
            c = c->next;
            if (c != nullptr) cout << ", ";
        }
    }
    cout << endl;
    string newPrefix = prefix + (isLast ? "    " : "|   ");
    FamilyNode* child = node->firstChild;
    while (child != nullptr) {
        bool last = (child->nextSibling == nullptr);
        displayNode(child, newPrefix, last);
        child = child->nextSibling;
    }
}

void FamilyTree::displayData() const {
    if (root == nullptr) {
        cout << "Cay gia pha dang trong." << endl;
        return;
    }
    cout << root->fullName << " (" << root->parentRelation << ") "
         << root->birthYear << " - "
         << (root->deathYear == 0 ? "con song" : to_string(root->deathYear));

    if (root->coupleList != nullptr) {
        cout << " | Vo/Chong: ";
        Couple* c = root->coupleList;
        while (c != nullptr) {
            cout << c->name;
            c = c->next;
            if (c != nullptr) cout << ", ";
        }
    }
    cout << endl;
    FamilyNode* child = root->firstChild;
    while (child != nullptr) {
        bool isLast = (child->nextSibling == nullptr);
        displayNode(child, "", isLast);
        child = child->nextSibling;
    }
}

bool FamilyTree::deleteNode(FamilyNode* node)
{
    if (root == nullptr || node == nullptr)
        return false;
    if (root == node)
    {
        FamilyNode* oldRoot = root;
        if (oldRoot->firstChild == nullptr)
        {
            root = nullptr;
            delete oldRoot;
        }
        else
        {
            FamilyNode* newRoot = oldRoot->firstChild;
            FamilyNode* sibling = newRoot->nextSibling;
            newRoot->nextSibling = nullptr;
            FamilyNode* last = newRoot;
            while (last->nextSibling != nullptr)
                last = last->nextSibling;
            last->nextSibling = sibling;
            root = newRoot;
            oldRoot->firstChild = nullptr;
            oldRoot->nextSibling = nullptr;
            delete oldRoot;
        }
        return true;
    }
    bool result = root->deleteNode(node);
    return result;
}

void FamilyTree::loadData(const std::string& filename) {
    ifstream is(filename);
    if (!is.is_open()) {
        cout << "Khong mo duoc file de doc: " << filename << endl;
        return;
    }
    delete root;
    root = loadNode(is);

    is.close();
}
