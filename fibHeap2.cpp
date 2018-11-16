#include <bits/stdc++.h>
#include <cmath> 
#include <cstdlib> 
#include <iostream> 
#include <malloc.h> 
#include <map>
#include <ostream>
#include <fstream>
#include <sstream>  
#include <string>
#include <math.h>   
#include <vector>
using namespace std; 

struct node { 
    node* parent; // Parent pointer 
    node* child; // Child pointer 
    node* left; // Pointer to the node on the left 
    node* right; // Pointer to the node on the right 
    int key; // Value of the node 
    int degree; // Degree of the node 
    bool mark; // Black or white mark of the node
}; 

//pointer to max node
struct node* maxNode = NULL;

//number of nodes
int numNodes = 0; 

//method to insert nodes
node* insert_node(int val) 
{ 
    struct node* new_node = (struct node*)malloc(sizeof(struct node)); 
    new_node->key = val; 
    new_node->degree = 0; 
    new_node->mark = false; 
    new_node->parent = NULL; 
    new_node->child = NULL; 
    new_node->left = new_node; 
    new_node->right = new_node; 
    if (maxNode != NULL) { 
        (maxNode->left)->right = new_node; 
        new_node->right = maxNode; 
        new_node->left = maxNode->left; 
        maxNode->left = new_node; 
        if (new_node->key > maxNode->key) 
            maxNode = new_node; 
    } 
    else { 
        maxNode = new_node; 
    }
    numNodes++;
    return new_node;
} 

//add child to parent
void meld_child(struct node* Node2, struct node* Node1) 
{ 
    (Node2->left)->right = Node2->right; 
    (Node2->right)->left = Node2->left; 
    if (Node1->right == Node1) 
        maxNode = Node1; 
    Node2->left = Node2; 
    Node2->right = Node2; 
    Node2->parent = Node1; 
    if (Node1->child == NULL) 
        Node1->child = Node2; 
    Node2->right = Node1->child; 
    Node2->left = (Node1->child)->left;
    ((Node1->child)->left)->right = Node2; 
    (Node1->child)->left = Node2; 
    if (Node2->key > (Node1->child)->key) 
        Node1->child = Node2; 
    Node1->degree++; 
}

//consolidate the fib heap
void Consolidate() 
{ 
    int deg; 
    float temp = (log(numNodes)) / (log(2)); 
    int temp2 = temp;
    struct node* arr[temp2]; 
    for (int i = 0; i <= temp2; i++) 
        arr[i] = NULL; 
    node* ptr1 = maxNode; 
    node* ptr2; 
    node* ptr3;
    do {
        deg = ptr1->degree; 
        while (arr[deg] != NULL && arr[deg] != ptr1) { 
            ptr2 = arr[deg]; 
            if (ptr1->key < ptr2->key) { 
                ptr3 = ptr1; 
                ptr1 = ptr2; 
                ptr2 = ptr3; 
            } 
            if (ptr2 == maxNode) 
                maxNode = ptr1; 
            meld_child(ptr2, ptr1); 
            if (ptr1->right == ptr1) 
                maxNode = ptr1; 
            arr[deg] = NULL; 
            deg++; 
        } 
        arr[deg] = ptr1; 
        ptr1 = ptr1->right; 
    } while (ptr1 != maxNode); 
    maxNode = NULL; 
    for (int j = 0; j <= temp2; j++) { 
        if (arr[j] != NULL) { 
            arr[j]->left = arr[j]; 
            arr[j]->right = arr[j]; 
            if (maxNode != NULL) { 
                (maxNode->left)->right = arr[j]; 
                arr[j]->right = maxNode; 
                arr[j]->left = maxNode->left; 
                maxNode->left = arr[j]; 
                if (arr[j]->key > maxNode->key) 
                    maxNode = arr[j]; 
            } 
            else { 
                maxNode = arr[j]; 
            } 
            if (maxNode == NULL) 
                maxNode = arr[j]; 
            else if (arr[j]->key > maxNode->key) 
                maxNode = arr[j]; 
        } 
    } 
} 

//find key corresponding to max node
tuple<string, int> findMapKey(map<string, node*> wordMap, node* max_node)
{
    tuple<string, int> tup;
    for(auto const& x : wordMap)
    {
        if(x.second == max_node)
        {
            tup = make_tuple(x.first, x.second->key);
            break;
        }
    }
    return tup;
}

//method to extract max code
tuple<string, int> extract_max(map<string, node*> wordMap) 
{ 
    tuple<string, int> ExtractedPair = findMapKey(wordMap, maxNode);
    if (maxNode == NULL) 
        cout<<endl; 
    else { 
        node* temp = maxNode; 
        node* n; 
        n = temp; 
        node* x = NULL; 
        if (temp->child != NULL) { 
  
            x = temp->child; 
            do { 
                n = x->right; 
                (maxNode->left)->right = x; 
                x->right = maxNode; 
                x->left = maxNode->left; 
                maxNode->left = x; 
                if (x->key > maxNode->key) 
                    maxNode = x; 
                x->parent = NULL; 
                x = n; 
            } while (n != temp->child); 
        } 
        (temp->left)->right = temp->right; 
        (temp->right)->left = temp->left; 
        maxNode = temp->right; 
        
        if (temp == temp->right && temp->child == NULL) 
            maxNode = NULL; 
        else { 
            maxNode = temp->right; 
            Consolidate(); 
        } 
        numNodes--; 
    } 
    return ExtractedPair;
} 

//method to perform cut
void Cut(struct node* n, struct node* temp) 
{ 
    if (n == n->right) 
        temp->child = NULL; 
  
    (n->left)->right = n->right; 
    (n->right)->left = n->left; 
    if (n == temp->child) 
        temp->child = n->right; 
  
    temp->degree = temp->degree - 1; 
    n->right = n; 
    n->left = n; 
    (maxNode->left)->right = n; 
    n->right = maxNode; 
    n->left = maxNode->left; 
    maxNode->left = n; 
    n->parent = NULL; 
    n->mark = true; 
} 

//recursive method to perform cascade cut
void Cascade_cut(struct node* n) 
{ 
    node* ptr = n->parent; 
    if (ptr != NULL) { 
        if (n->mark == false) { 
            n->mark = true; 
        } 
        else { 
            Cut(n, ptr); 
            Cascade_cut(ptr); 
        } 
    } 
} 

//method to increase given node key
void Increase_key(struct node* n, int val)     
{ 
    n->key = val; 
  
    struct node* temp = n->parent; 
    if (temp != NULL && n->key > temp->key) { 
        Cut(n, temp); 
        Cascade_cut(temp); 
    } 
    if (n->key > maxNode->key) 
        maxNode = n; 
}

//driver function
int main() 
{
    map<string, node*> wordMap;
    ifstream infile("file_name");
    ofstream fout("output_file");

    int x, q, i;
    vector<tuple<string, int>> v;
    string line, key, nodeVal;
    //read lines of input
    while(infile)
    {
        v.clear();
        getline(infile, line);
        if(line == "")
            continue;
        line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);
        if(line.front() == '$')
        {
            stringstream ss (line);
            getline(ss, key, ' ');
            getline(ss, nodeVal);
            key = key.substr(1, key.size() - 1);
            if(key == "")
            {
                continue;
            }
            if(nodeVal == "")
            {
                continue;
            }
            x = stoi(nodeVal);
            if(wordMap.count(key) > 0)
                Increase_key(wordMap.find(key)->second, (x + wordMap.find(key)->second->key));
            else
                wordMap[key] = insert_node(x);
        }
        else
        {
            try
            {
                transform(line.begin(), line.end(), line.begin(), ::tolower); 
                if(line.compare("stop") == 0)
                    break;
                q = stoi(line);
                for(i = 0; i < q; i++) 
                {
                    v.push_back(extract_max(wordMap));
                }
                for(i = 0; i < q; i++)
                {
                    fout << get<0>(v[i]) <<",";
                    wordMap[get<0>(v[i])] = insert_node(get<1>(v[i]));
                }
                fout << endl;
            }
            catch(const invalid_argument& ia)
            {
                continue;
            }
        }
    }
    fout.close();
    infile.close();
    return 0;
} 
