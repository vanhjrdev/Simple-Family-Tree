#include "FamilyTree.h"
#include "FamilyNode.h"
#include <chrono>
#include <cstdlib>
#include <ctime>
using namespace std;

void printMenu() {
    cout << " MENU FAMILY TREE\n";
    cout << "1. Load data\n";
    cout << "2. Hien thi toan bo cay gia pha\n";
    cout << "3. Them thanh vien\n";
    cout << "4. Them vo/chong cho mot nguoi\n";
    cout << "5. Sua thong tin thanh vien\n";
    cout << "6. Xoa thanh vien\n";
    cout << "7. Xem moi quan he cua 1 nguoi\n";
    cout << "8. Test hieu nang\n";
    cout << "0. Thoat\n";
    cout << "==============================================\n";
    cout << "Hay chon chuc nang: ";
}

int main() {
    FamilyNode fn;
    int choice;
    string name, relation, parentName, coupleName, filename;
    int bY, dY;
    fn.loadData("DemoData_Nhom1.txt");

    do {
        printMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            choice = -1;
        }
        cin.ignore();
        switch (choice) {
            case 1: {
                cout << "Nhap ten file muon load: ";
                getline(cin, filename);
                fn.loadData(filename);
                cout << "Da load xong\n";
                break;
            }
            case 2:
                cout << "\n--- CAY GIA PHA ---\n";
                fn.displayData();
                break;
            case 3: {
                cout << "Nhap ten cha/me de them con: ";
                getline(cin, parentName);
                FamilyNode* parent = fn.getRoot()->findNode(parentName);
                if (parent != nullptr) {
                    cout << "Nhap ten nguoi con: ";
                    getline(cin, name);
                    cout << "Quan he (vd: Con trai, Con gai): ";
                    getline(cin, relation);
                    cout << "Nam sinh: ";
                    cin >> bY;
                    cout << "Nam mat (nhap 0 neu van song): ";
                    cin >> dY;
                    cin.ignore();
                    parent->addChild(new FamilyNode(name, relation, bY, dY));
                    cout << "Them con thanh cong!\n";
                } else {
                    cout << "Khong tim thay '" << parentName;
                }
                break;
            }
            case 4: {
                cout << "Nhap ten thanh vien muon them vo/chong: ";
                getline(cin, name);
                FamilyNode* node = fn.getRoot()->findNode(name);
                if (node != nullptr) {
                    cout << "Nhap ten vo/chong: ";
                    getline(cin, coupleName);
                    node->addCouple(coupleName);
                    cout << "Da them\n";
                } else {
                    cout << "Ten nay khong ton tai\n";
                }
                break;
            }
            case 5: {
                cout << "Nhap ten thanh vien can sua thong tin: ";
                getline(cin, name);
                FamilyNode* node = fn.getRoot()->findNode(name);
                if (node != nullptr) {
                    string newName, newRel;
                    int newBY, newDY;
                    cout << "Nhap ten moi: ";
                    getline(cin, newName);
                    cout << "Quan he moi: ";
                    getline(cin, newRel);
                    cout << "Nam sinh moi: ";
                    cin >> newBY;
                    cout << "Nam mat moi (0 neu con song): ";
                    cin >> newDY;
                    cin.ignore();
                    node->update(newName, newRel, newBY, newDY);
                    cout << "Update thanh cong\n";
                } else {
                    cout << "Ten '" << name << "' ko ton tai! \n";
                }
                break;
            }
            case 6: {
                cout << "Nhap ten thanh vien can xoa: ";
                getline(cin, name);
                FamilyNode* node = fn.getRoot()->findNode(name);
                if (node != nullptr) {
                    if (fn.FamilyTree::deleteNode(node)) {
                        cout << "Da xoa thanh cong!\n";
                    } else {
                        cout << "Xoa that bai\n";
                    }
                } else {
                    cout << "Nguoi nay khong ton tai!\n";
                }
                break;
            }
            case 7: {
                cout << "Nhap ten nguoi muon Xem moi quan he: ";
                getline(cin, name);
                cout << "\n--- Thong Tin ---\n";
                fn.printRelationships(name);
                break;
            }
            case 8: {
                cout << "\n=== DO HIEU NANG TIM KIEM ===\n";
                cout << "Nhap ten thanh vien: ";
                getline(cin, name);
                auto start = chrono::high_resolution_clock::now();
                FamilyNode* result = fn.getRoot()->findNode(name);
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

                if (result != nullptr) {
                    cout << "-> Ket qua: Da tim ra\n";
                } else {
                    cout << "-> Ket qua: Khong tim thay (da quet toan bo cay).\n";
                }
                cout << "=> Runtime: " << duration.count() << " microseconds (us).\n";
                break;
            }
            case 0:
            fn.saveData(filename);
                break;
            default:
                cout << "Nhap sai, vui long nhap lai\n";
        }
    } while (choice != 0);

    return 0;
}