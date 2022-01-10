#include "avl-tree.h";
#include <cmath>
#include <stack>
#include <iostream>

using namespace std;

typedef BinarySearchTree::DataType DataType;
typedef BinarySearchTree::Node Node;

int AVLTree::depthBelow(Node *n)
{
    if(n == nullptr){
        return -1;
    }else{
        int depthRight = depthBelow(n->right);
        int depthLeft = depthBelow(n->left);

        if (depthRight > depthLeft){
            return depthRight + 1;
        }else{
            return depthLeft + 1;
        }
    }
}

bool AVLTree::singleLeftRotation(Node *parentNode, bool isLeftChild)
{
    Node* current = nullptr;

    if(parentNode == nullptr){
        current = getRootNode();
    }else if(isLeftChild){
        current = parentNode->left;
    }else{
        current = parentNode->right;
    }

    Node* S = current->right;
    Node* B = S->left;
    S->left = current;
    current->right = B;

    if(parentNode == nullptr){
        Node** root = getRootNodeAddress();
        *root = S;
    }else if(isLeftChild){
        parentNode->left = S;
    }else{
        parentNode->right = S;
    }
    return true;
}

bool AVLTree::singleRightRotation(Node *parentNode, bool isLeftChild)
{
    Node* current = nullptr;

    if(parentNode == nullptr){
        current = getRootNode();
    }else if(isLeftChild){
        current = parentNode->left;
    }else{
        current = parentNode->right;
    }

    Node* S = current->left;
    Node* B = S->right;
    S->right = current;
    current->left = B;

    if(parentNode == nullptr){
        Node** root = getRootNodeAddress();
        *root = S;
    }else if(isLeftChild){
        parentNode->left = S;
    }else{
        parentNode->right = S;
    }
    return true;
}

bool AVLTree::leftRightRotation(Node *parentNode, bool isLeftChild)
{
    if(parentNode == nullptr){
        Node* current = getRootNode();
        singleLeftRotation(current, true);
        singleRightRotation(parentNode, isLeftChild);
    }else if(isLeftChild){
        singleLeftRotation(parentNode->left, true);
        singleRightRotation(parentNode, isLeftChild);
    }else{
        singleLeftRotation(parentNode->right, true);
        singleRightRotation(parentNode, isLeftChild);
    }
    return true;
}

bool AVLTree::rightLeftRotation(Node *parentNode, bool isLeftChild)
{
    if(parentNode == nullptr){
        Node* current = getRootNode();
        singleRightRotation(current, false);
        singleLeftRotation(parentNode, isLeftChild);
    }else if(isLeftChild){
        singleRightRotation(parentNode -> left, false);
        singleLeftRotation(parentNode, isLeftChild);
    }else{
        singleRightRotation(parentNode -> right, false);
        singleLeftRotation(parentNode, isLeftChild);
    }
    return true;
}

std::stack<BinarySearchTree::Node*> *AVLTree::pathToNodeStack(DataType val)
{
    std::stack<BinarySearchTree::Node*> *node_path = new std::stack<BinarySearchTree::Node*>();
    Node* nodeA = getRootNode();
    Node* nodeB = nullptr;
    node_path->push(nodeB);

    if(nodeA->val == val){
        return node_path;
    }

    while(nodeA->val != val){
        node_path->push(nodeA);
        if (nodeA->val < val){
            nodeA = nodeA->right;
        }else{
            nodeA = nodeA->left;
        }
    }
    return node_path;
}

bool AVLTree::updateNodeBalance(std::stack<BinarySearchTree::Node*> *pathToNode, DataType val)
{
    while(pathToNode->top() != nullptr){
        Node* tempNode = pathToNode->top();
        pathToNode->pop();
        Node* parentNode = pathToNode->top();

        int balance = depthBelow(tempNode->left) - depthBelow(tempNode->right);
        bool isLeftChild;

        if(parentNode != nullptr && parentNode -> left == tempNode)
            isLeftChild = true;
        else
            isLeftChild = false;

        if(balance >= 2){
            int balanceLeft = depthBelow(tempNode->left->left) - depthBelow(tempNode->left->right);
            if(balanceLeft > 0){
                singleRightRotation(parentNode, isLeftChild);
            }else{
                leftRightRotation(parentNode, isLeftChild);
            }
        }else if(balance <= -2){
            int balanceRight = depthBelow(tempNode->right->left) - depthBelow(tempNode->right->right);
            if(balanceRight > 0){
                rightLeftRotation(parentNode, isLeftChild);
            }else{
                singleLeftRotation(parentNode, isLeftChild);
            }
        }
    }
    return true;
}


bool AVLTree::insert(DataType val)
{
    BinarySearchTree::insert(val);
    stack<BinarySearchTree::Node*> *pathToNode = pathToNodeStack(val);
    bool balance = updateNodeBalance(pathToNode, val);
    delete pathToNode;
    return balance;
}

bool AVLTree::remove(DataType val)
{
    if(BinarySearchTree::size() == 2){ //idk it breaks when there are only 2 nodes left and you delete one
        Node* temp = BinarySearchTree::getRootNode();
        if (temp->val == val){
                //Node* root = *BinarySearchTree::getRootNodeAddress();
            if (temp->left == nullptr){
                *BinarySearchTree::getRootNodeAddress() = temp->right;
            } else {
                *BinarySearchTree::getRootNodeAddress() = temp->left;
            }
            return true;
        }
        temp->left = nullptr;
        temp->right = nullptr;
        return true;
    }

    stack<BinarySearchTree::Node*> *pathToNode = pathToNodeStack(val);
    BinarySearchTree::remove(val);
    bool balance;

    if(pathToNode->top() == nullptr)
    {
        delete pathToNode;
        return true;
    }
    balance = updateNodeBalance(pathToNode, val);
    delete pathToNode;
    return balance;
}