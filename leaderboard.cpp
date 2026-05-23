#include <bits/stdc++.h>
using namespace std;

struct Player {
    string name;
    int score;
};

struct BSTNode {
    Player data;
    BSTNode* left;
    BSTNode* right;
    BSTNode(Player p) : data(p), left(nullptr), right(nullptr) {}
};

string trim(string s) {
    s.erase(0, s.find_first_not_of(" \t\r\n"));
    s.erase(s.find_last_not_of(" \t\r\n") + 1);
    return s;
}

//using merge sort to sort the players by score for the leaderboard display
void merge(vector<Player>& arr, int lo, int mid, int hi) {
    vector<Player> left(arr.begin() + lo, arr.begin() + mid + 1);
    vector<Player> right(arr.begin() + mid + 1, arr.begin() + hi + 1);

    int i=0,j=0,k=lo;
    while(i < (int)left.size() && j < (int)right.size()) {
        if (left[i].score >= right[j].score)
            arr[k++] = left[i++];
        else
            arr[k++] = right[j++];
    }
    while(i < (int)left.size()) arr[k++]=left[i++];
    while(j < (int)right.size()) arr[k++]=right[j++];
}

void mergeSort(vector<Player>& arr, int lo, int hi) {
    if (lo >= hi) return;
    int mid = (lo + hi) / 2;
    mergeSort(arr, lo, mid);
    mergeSort(arr, mid + 1, hi);
    merge(arr, lo, mid, hi);
}
//BST class to manage the players, insert, delete, search, and display functions
class BST {
private:
    BSTNode* root;
    BSTNode* insert(BSTNode* node, Player p) {
        if(!node) return new BSTNode(p);
        if(p.name < node->data.name)
            node->left = insert(node->left, p);
        else if(p.name > node->data.name)
            node->right = insert(node->right, p);
        else {
            node->data.score = p.score;
            cout << "   Score updated for " << p.name << "\n";
        }
        return node;
    }

    BSTNode* search(BSTNode* node, string name) {
        if (!node || node->data.name == name) return node;
        if (name < node->data.name) return search(node->left, name);
        return search(node->right, name);
    }

    BSTNode* findMin(BSTNode* node) {
        while (node->left) node = node->left;
        return node;
    }

    BSTNode* deleteNode(BSTNode* node, string name) {
        if (!node) return nullptr;
        if (name < node->data.name)
            node->left = deleteNode(node->left, name);
        else if (name > node->data.name)
            node->right = deleteNode(node->right, name);
        else {
            if (!node->left) {
                BSTNode* temp = node->right;
                delete node;
                return temp;
            } else if (!node->right) {
                BSTNode* temp = node->left;
                delete node;
                return temp;
            }
            BSTNode* successor = findMin(node->right);
            node->data = successor->data;
            node->right = deleteNode(node->right, successor->data.name);
        }
        return node;
    }

    void inorder(BSTNode* node, vector<Player>& result) {
        if (!node) return;
        inorder(node->left, result);
        result.push_back(node->data);
        inorder(node->right, result);
    }

    void collectAll(BSTNode* node, vector<Player>& result) {
        if (!node) return;
        result.push_back(node->data);
        collectAll(node->left, result);
        collectAll(node->right, result);
    }

public:
    BST() : root(nullptr) {}

    void addPlayer(string name, int score) {
        Player p = {name, score};
        root = insert(root, p);
    }

    void removePlayer(string name) {
        if (!search(root, name)) {
            cout << "   Player '" << name << "' not found.\n";
            return;
        }
        root = deleteNode(root, name);
        cout << "   Player '" << name << "' removed.\n";
    }

    void searchPlayer(string name) {
        BSTNode* result = search(root, name);
        if (result) {
            cout << "\n  Found!\n";
            cout << "  Name  : " << result->data.name << "\n";
            cout << "  Score : " << result->data.score << "\n";
        } else {
            cout << "   Player '" << name << "' not found.\n";
        }
    }

    void showLeaderboard() {
        vector<Player> players;
        collectAll(root, players);

        if (players.empty()) { cout << "    No players yet.\n"; return; }

        mergeSort(players, 0, players.size() - 1);

        cout << "|           LEADERBOARD                 |\n";
        cout << "|  Rank  | Player           | Score          |\n";

        for (int i = 0; i < (int)players.size(); i++) {
            string medal = "";
            if (i == 0) medal = " GOLD";
            else if (i == 1) medal = " SILVER";
            else if (i == 2) medal = " BRONZE";

            cout <<  setw(3) << left << i + 1
                 <<  setw(16) << left << players[i].name
                 <<  setw(10) << left << players[i].score
                 << medal << setw(medal.empty() ? 4 : 1) << "" << "\n";
        }
    }

    void showAlphabetical() {
        vector<Player> players;
        inorder(root, players);

        if (players.empty()) { cout << "  No players yet.\n"; return; }

        cout << "\n Players (Alphabetical Order via BST Inorder):\n";
        for (auto& p : players)
            cout << "  " << setw(18) << left << p.name << " -> " << p.score << "\n";
    }

    void showTopK(int k) {
        vector<Player> players;
        collectAll(root, players);

        if (players.empty()) { cout << "   No players yet.\n"; return; }

        mergeSort(players, 0, players.size() - 1);

        k = min(k, (int)players.size());
        cout << "\n Top " << k << " Players:\n";
        for (int i = 0; i < k; i++)
            cout << "  " << i + 1 << ". " << setw(16) << left
                 << players[i].name << " -> " << players[i].score << "\n";
    }

    void showScoreRange(int lo, int hi) {
        vector<Player> players;
        collectAll(root, players);

        cout << "\nPlayers with score between " << lo << " and " << hi << ":\n";

        bool found=false;
        for (auto& p : players) {
            if (p.score >= lo && p.score <= hi) {
                cout << "  " << setw(16) << left << p.name << " -> " << p.score << "\n";
                found = true;
            }
        }
        if (!found) cout << "  No players found in this range.\n";
    }

    bool isEmpty() { return root == nullptr; }
};

//  Main Menu
int main() {
    BST leaderboard;
    cout << "|      Leaderboard System              |\n";
    cout << "|   DSA: BST + Merge Sort              |\n";

    int choice;
    do {
        cout << "\n    MENU     \n";
        cout << " 1. Add Player\n";
        cout << " 2. Remove Player\n";
        cout << " 3. Search Player\n";
        cout << " 4. Show Leaderboard (Merge Sort)\n";
        cout << " 5. Show Alphabetical List (BST Inorder)\n";
        cout << " 6. Show Top-K Players\n";
        cout << " 7. Players in Score Range\n";
        cout << " 0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if(choice == 1) {
            string name; int score;
            cout << "Enter player name  : "; cin.ignore(numeric_limits<streamsize>::max(), '\n'); getline(cin, name);
            name = trim(name);
            cout << "Enter score        : "; cin >> score;
            leaderboard.addPlayer(name, score);
            cout << "   Player added!\n";

        } else if(choice == 2) {
            string name;
            cout << "Enter player name to remove: "; cin.ignore(numeric_limits<streamsize>::max(), '\n'); getline(cin, name);
            name = trim(name);
            leaderboard.removePlayer(name);

        } else if(choice == 3) {
            string name;
            cout << "Enter player name to search: "; cin.ignore(numeric_limits<streamsize>::max(), '\n'); getline(cin, name);
            name = trim(name);
            leaderboard.searchPlayer(name);

        } else if(choice == 4) {
            leaderboard.showLeaderboard();

        } else if(choice == 5) {
            leaderboard.showAlphabetical();

        } else if(choice == 6) {
            int k;
            cout << "Enter K: "; cin >> k;
            leaderboard.showTopK(k);

        } else if(choice == 7) {
            int lo, hi;
            cout << "Enter min score: "; cin >> lo;
            cout << "Enter max score: "; cin >> hi;
            leaderboard.showScoreRange(lo, hi);

        } else if(choice == 0) {
            cout << "\n Exiting. Good luck!\n";
        } else {
            cout << "\n Invalid choice.\n";
        }

    } while (choice != 0);
    return 0;
}
