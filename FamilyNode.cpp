#include "FamilyNode.h"
#include <queue>

struct FamilyPathNode {
    FamilyNode* node;
    FamilyPathNode* next;
    FamilyPathNode(FamilyNode* n) : node(n), next(nullptr) {}
};
static FamilyPathNode* appendToPath(FamilyPathNode* head, FamilyNode* node) {
    FamilyPathNode* newHead = nullptr;
    FamilyPathNode* tail = nullptr;
    for (FamilyPathNode* p = head; p != nullptr; p = p->next) {
        FamilyPathNode* copy = new FamilyPathNode(p->node);
        if (newHead == nullptr) newHead = copy;
        else tail->next = copy;
        tail = copy;
    }
    FamilyPathNode* newNode = new FamilyPathNode(node);
    if (newHead == nullptr) newHead = newNode;
    else tail->next = newNode;
    return newHead;
}

static void freePath(FamilyPathNode* head) {
    while (head != nullptr) {
        FamilyPathNode* tmp = head;
        head = head->next;
        delete tmp;
    }
}

static FamilyNode* lastNodeOf(FamilyPathNode* head) {
    FamilyNode* result = nullptr;
    for (FamilyPathNode* p = head; p != nullptr; p = p->next)
        result = p->node;
    return result;
}

FamilyPathNode* FamilyNode::findPathBFS(const string& name) {
    FamilyNode* start = getRoot();
    if (start == nullptr) return nullptr;

    queue<FamilyPathNode*> q;
    q.push(appendToPath(nullptr, start));

    while (!q.empty()) {
        FamilyPathNode* path = q.front();
        q.pop();
        FamilyNode* current = lastNodeOf(path);

        if (current->fullName == name) {
            while (!q.empty()) { freePath(q.front()); q.pop(); }
            return path;
        }

        FamilyNode* child = current->firstChild;
        while (child != nullptr) {
            q.push(appendToPath(path, child));
            child = child->nextSibling;
        }
        freePath(path);
    }
    return nullptr;
}

FamilyPathNode* FamilyNode::dfsHelper(FamilyNode* node, const string& name, FamilyPathNode* pathSoFar) {
    if (node == nullptr) return nullptr;

    FamilyPathNode* currentPath = appendToPath(pathSoFar, node);
    if (node->fullName == name) return currentPath;

    FamilyNode* child = node->firstChild;
    while (child != nullptr) {
        FamilyPathNode* result = dfsHelper(child, name, currentPath);
        if (result != nullptr) {
            freePath(currentPath);
            return result;
        }
        child = child->nextSibling;
    }
    freePath(currentPath);
    return nullptr;
}

FamilyPathNode* FamilyNode::findPathDFS(const string& name) {
    return dfsHelper(getRoot(), name, nullptr);
}
FamilyNode* FamilyNode::findAncestor(const string& name) {
    FamilyPathNode* path = findPathDFS(name);
    if (path == nullptr) return nullptr;

    FamilyNode* ancestor = path->node;
    freePath(path);
    return ancestor;
}

string relationWord(FamilyNode* child) {
    if (child->parentRelation.empty())
        return "con";
    return child->parentRelation;
}

void FamilyNode::printRelationships(const string& name) {
    FamilyPathNode* path = findPathDFS(name);

    if (path == nullptr) {
        cout << "Khong tim thay nguoi ten: " << name << endl;
        return;
    }

    int n = 0;
    FamilyNode* target = nullptr;
    for (FamilyPathNode* p = path; p != nullptr; p = p->next) {
        n++;
        target = p->node;
    }

    cout << "[QUAN HE] " << target->fullName << endl;
    cout << "Cu to: " << path->node->fullName << endl;

    cout << "Duong di DFS: ";
    for (FamilyPathNode* p = path; p != nullptr; p = p->next) {
        cout << p->node->fullName;
        if (p->next != nullptr) cout << " -> ";
    }
    cout << endl;

    cout << "Duong di BFS: ";
    FamilyPathNode* bfsPath = findPathBFS(name);
    for (FamilyPathNode* p = bfsPath; p != nullptr; p = p->next) {
        cout << p->node->fullName;
        if (p->next != nullptr) cout << " -> ";
    }
    cout << endl;
    freePath(bfsPath);

    cout << "Quan he doc: ";
    FamilyNode* parent = nullptr;
    if (n == 1) {
        cout << target->fullName << " la cu to cua gia pha." << endl;
    } else {
        FamilyNode* prev = path->node;
        FamilyPathNode* cur = path->next;
        while (cur != nullptr) {
            cout << cur->node->fullName << " la " << relationWord(cur->node)
                 << " cua " << prev->fullName;
            if (cur->next != nullptr) cout << "; ";
            prev = cur->node;
            cur = cur->next;
        }
        parent = prev;
        cout << endl;
        cout << "Cha/me truc tiep: " << parent->fullName << endl;

        cout << "To tien phia tren: ";
        if (n <= 2) {
            cout << "khong co";
        } else {
            bool first = true;
            for (FamilyPathNode* p = path; p != nullptr && p->node != parent; p = p->next) {
                if (!first) cout << ", ";
                cout << p->node->fullName;
                first = false;
            }
        }
        cout << endl;

        cout << "Anh/chi/em: ";
        bool hasSibling = false;
        FamilyNode* sibling = parent->firstChild;
        while (sibling != nullptr) {
            if (sibling != target) {
                if (hasSibling) cout << ", ";
                cout << sibling->fullName;
                hasSibling = true;
            }
            sibling = sibling->nextSibling;
        }
        if (!hasSibling) cout << "khong co";
        cout << endl;
    }

    cout << "Vo/chong: ";
    Couple* c = target->coupleList;
    if (c == nullptr) cout << "khong co";
    while (c != nullptr) {
        cout << c->name;
        c = c->next;
        if (c != nullptr) cout << ", ";
    }
    cout << endl;

    cout << "Con: ";
    bool hasChild = false;
    FamilyNode* child = target->firstChild;
    while (child != nullptr) {
        if (hasChild) cout << ", ";
        cout << child->fullName;
        hasChild = true;
        child = child->nextSibling;
    }
    if (!hasChild) cout << "khong co";
    cout << endl;

    freePath(path);
}

FamilyNode *FamilyNode::getRoot()
{
    return root;
}

FamilyNode *FamilyNode::findNode(const string &name)
{
     if (fullName == name)
        return this;
    FamilyNode* temp = firstChild;
    while (temp != nullptr)
    {
        FamilyNode* result = temp->findNode(name);
        if (result != nullptr)
            return result;
        temp = temp->nextSibling;
    }
    return nullptr;
}

FamilyNode::FamilyNode()
{
    fullName = "";
    parentRelation = "";
    birthYear = 0;
    deathYear = 0;
    coupleList = nullptr;
    firstChild = nullptr;
    nextSibling = nullptr;
}

FamilyNode::FamilyNode(string name, string relation, int bY, int dY)
{
    fullName = name;
    parentRelation = relation;
    birthYear = bY;
    deathYear = dY;
    coupleList = nullptr;
    firstChild = nullptr;
    nextSibling = nullptr;
}

bool FamilyNode::update(string name, string relation, int bY, int dY)
{
    if (name.empty())
        return false;
    fullName = name;
    parentRelation = relation;
    birthYear = bY;
    deathYear = dY;
    return true;
}

bool FamilyNode::addChild(FamilyNode* child)
{
    if (child == nullptr)
        return false;
    if (firstChild == nullptr) {
        firstChild = child;
    }
    else {
        FamilyNode* temp = firstChild;
        while (temp->nextSibling != nullptr)
            temp = temp->nextSibling;
        temp->nextSibling = child;
    }
    return true;
}

FamilyNode* FamilyNode::addCouple(string name)
{
    if (name.empty())
        return nullptr;
    Couple* newCouple = new Couple(name);
    if (coupleList == nullptr) {
        coupleList = newCouple;
    }
    else {
        Couple* temp = coupleList;
        while (temp->next != nullptr)
            temp = temp->next;
        temp->next = newCouple;
    }
    return this;
}

bool FamilyNode::deleteCouple(string name)
{
    if (coupleList == nullptr)
        return false;

    if (coupleList->name == name) {
        Couple* temp = coupleList;
        coupleList = coupleList->next;
        delete temp;
        return true;
    }
    Couple* temp = coupleList;
    while (temp->next != nullptr && temp->next->name != name)
        temp = temp->next;
    if (temp->next == nullptr)
        return false;

    Couple* nodeToDelete = temp->next;
    temp->next = nodeToDelete->next;
    delete nodeToDelete;

    return true;
}
bool FamilyNode::deleteChild(FamilyNode* child)
{
    if (child == nullptr || firstChild == nullptr)
        return false;
    FamilyNode* prev = nullptr;
    FamilyNode* current = firstChild;
    while (current != nullptr && current != child)
    {
        prev = current;
        current = current->nextSibling;
    }
    if (current == nullptr)
        return false;
    if (prev == nullptr)
    {
        firstChild = current->nextSibling;
    }
    else
    {
        prev->nextSibling = current->nextSibling;
    }
    current->nextSibling = nullptr;
    delete current;
    return true;
}

bool FamilyNode::deleteNode(FamilyNode* node)
{
    if (node == nullptr)
        return false;

    FamilyNode* temp = firstChild;
    while (temp != nullptr)
    {
        if (temp == node)
            return deleteChild(node);
        if (temp->deleteNode(node))
            return true;
        temp = temp->nextSibling;
    }
    return false;
}
FamilyNode::~FamilyNode()
{
    Couple* c = coupleList;
    while (c != nullptr) {
        Couple* temp = c;
        c = c->next;
        delete temp;
    }
    FamilyNode* child = firstChild;
    while (child != nullptr) {
        FamilyNode* temp = child;
        child = child->nextSibling;
        temp->nextSibling = nullptr;
        delete temp;
    }
    firstChild = nullptr;
    nextSibling = nullptr;
}