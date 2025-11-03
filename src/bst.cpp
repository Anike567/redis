#include <iostream>
#include <queue>
#include <stack>
using namespace std;

template<typename T>
class Node {
public:
    string key;
    T value;

    Node *left;
    Node *right;

    Node(string key, T value) {
        this->key = key;
        this->value = value;
        this->left = nullptr;
        this->right = nullptr;
    }
};

template<typename T>
class Map {
private:
    Node<T> *root;

public:
    Map() {
        root = nullptr;
    }
    
    void insert(string key, T value) {
        if (root == nullptr) {
            root = new Node<T>(key, value);
            return;
        }
        
        stack<Node<T> *> st;
        st.push(root);

        while (!st.empty()) {
            Node<T> *tp = st.top();
            st.pop();

            // If key exists, update value
            if (key == tp->key) {
                tp->value = value;
                return;
            }

            if (key < tp->key) {
                if (tp->left == nullptr) {
                    tp->left = new Node<T>(key, value);
                    return;
                }
                st.push(tp->left);
            } 
            else { 
                if (tp->right == nullptr) {
                    tp->right = new Node<T>(key, value);
                    return;
                }
                st.push(tp->right);
            }
        }
    }

    void print() {
        if (!root) return;

        queue<Node<T>*> q;
        q.push(root);

        while (!q.empty()) {
            Node<T>* tmp = q.front();
            q.pop();

            cout << tmp->key << " = " << tmp->value << endl;

            if (tmp->left) q.push(tmp->left);
            if (tmp->right) q.push(tmp->right);
        }
    }
};


int main(void){

    Map<string> mp;

    mp.insert("name", "aniket");
    mp.insert("age", "30");
    mp.print();
}