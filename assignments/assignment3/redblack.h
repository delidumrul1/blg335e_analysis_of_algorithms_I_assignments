#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
using namespace std;

//This node actually represents the processes.
class Node{
    public:
        string name;
        int arrival_time;
        int burst_time;
        string status; // it contains "Completed" or "Incomplete".

        int key;
        Node* p; //This is for parent.
        Node* left;
        Node* right;
        string color;
        Node();
};
Node::Node(){
    key=0;
    color="black";
}


class RBT{
    public:
        Node* root;
        Node* nil;
        RBT();
        Node* minimum(Node* x);
        void left_rotate(Node* x);
        void right_rotate(Node* x);
        
        void insert(Node* z);    
        void insert_fixup(Node* z);

        void transplant(Node* u, Node* v);
        void delete_(Node* z);
        void delete_fixup(Node* x);

        Node* tree_search(Node* x, int k);
        void inorder_tree_walk(ostream& strea,Node* x);
};



RBT::RBT(){
    nil=new Node;
    nil->color="black";
    nil->left=nil;
    nil->right=nil;
    root=nil; 
}

Node* RBT::minimum(Node* x){
    while(x->left!=nil)
        x=x->left;
    return x;    
}
void RBT::left_rotate(Node* x){
    Node* y=x->right;
    x->right=y->left;
    if(y->left!=nil)
        y->left->p=x;
    y->p=x->p;
    if(x->p==nil)
        root=y;
    else if(x==x->p->left)
        x->p->left=y;
    else
        x->p->right=y;
    y->left=x;
    x->p=y;
}

void RBT::right_rotate(Node* x){
    Node* y=x->left;
    x->left=y->right;
    if(y->right!=nil)
        y->right->p=x;
    y->p=x->p;
    if(x->p==nil)
        root=y;
    else if(x==x->p->right)
        x->p->right=y;
    else
        x->p->left=y;
    y->right=x;
    x->p=y;
}

void RBT::insert(Node* z){
    Node* y=nil;
    Node* x=root;
    while(x!=nil){
        y=x;
        if(z->key < x->key)
            x=x->left;
        else
            x=x->right;
    }
    z->p=y;
    if(y==nil)
        root=z;
    else if(z->key<y->key)
        y->left=z;
    else
        y->right=z;
    z->left=nil;
    z->right=nil;
    z->color="red";
    insert_fixup(z);

    
}

void RBT::insert_fixup(Node* z){
    Node* y;
    while(z->p->color=="red"){
        if(z->p==z->p->p->left){
            y=z->p->p->right;
            if(y->color=="red"){
                z->p->color="black";
                y->color="black";
                z->p->p->color="red";
                z=z->p->p;
            }
            else{
                if(z==z->p->right){
                    z=z->p;
                    left_rotate(z);
                }
                z->p->color="black";
                z->p->p->color="red";
                right_rotate(z->p->p);
            }
        }
        else{
            y=z->p->p->left;
            if(y->color=="red"){
                z->p->color="black";
                y->color="black";
                z->p->p->color="red";
                z=z->p->p;
            }
            else{	
                if(z==z->p->left){
                    z=z->p;
                    right_rotate(z);
                }
                z->p->color="black";
                z->p->p->color="red";
                left_rotate(z->p->p);
            }
        }
    }			
	root->color="black";
}


void RBT::transplant(Node* u, Node* v){
    if(u->p==nil)
        root=v;
    else if(u==u->p->left)
        u->p->left=v;
    else
        u->p->right=v;
    v->p=u->p;
}

void RBT::delete_(Node* z){
    Node* y=z;
    Node* x;
    string y_original_color=y->color;
    if(z->left==nil){
        x=z->right;
        transplant(z,z->right);
    }
    else if(z->right==nil){
        x=z->left;
        transplant(z,z->left);
    }
    else{ 
        y=minimum(z->right);
        y_original_color=y->color;
        x=y->right;
        if(y->p==z)
            x->p=y;
        else{
            transplant(y,y->right);
            y->right=z->right;
            y->right->p=y;
        }
        transplant(z,y);
        y->left=z->left;
        y->left->p=y;
        y->color=z->color;
    }
    //delete z; simdilik delete yapmayalim z'yi.
    if(y_original_color=="black")
        delete_fixup(x);
}


void RBT::delete_fixup(Node* x){
    Node* w;
    while(x!=root and x->color=="black"){
        
        if(x==x->p->left){
            w=x->p->right;
            if(w->color=="red"){
                w->color="black";
                x->p->color="red";
                left_rotate(x->p);
                w=x->p->right;
            }
            if(w->left->color=="black" and w->right->color=="black"){
                w->color="red";
                x=x->p;
            }
            else{
                if(w->right->color=="black"){
                    w->left->color="black";
                    w->color="red";
                    right_rotate(w);
                    w=x->p->right;
                }
                w->color=x->p->color;
                x->p->color="black";
                w->right->color="black";
                left_rotate(x->p);
                x=root;
            }
        
        }
        else{
            w=x->p->left;
            if(w->color=="red"){
                w->color="black";
                x->p->color="red";
                right_rotate(x->p);
                w=x->p->left;
            }
            if(w->right->color=="black" and w->left->color=="black"){
                w->color="red";
                x=x->p;
            }
            else{
                if(w->left->color=="black"){
                    w->right->color="black";
                    w->color="red";
                    left_rotate(w);
                    w=x->p->left;
                }
                w->color=x->p->color;
                x->p->color="black";
                w->left->color="black";
                right_rotate(x->p);
                x=root;	
            }
        }
    }
    x->color="black";
}

Node* RBT::tree_search(Node* x, int k){
    while(x!=nil && k!=x->key){
        if(k<x->key)
            x=x->left;
        else
            x=x->right;
    }
    return x;
}   


void RBT::inorder_tree_walk(ostream& strea, Node* x){
    if(x!=nil){
        inorder_tree_walk(strea,x->left);
        strea <<  x->name << ":" << x->key << "-" << x->color << ";";
        inorder_tree_walk(strea,x->right);
    }
}


/*
I'm testing my rbt tree according to exercies in clrs.
int main() {
    RBT rbt;

    Node* n;
    n=new Node;
    n->key=41;
    rbt.insert(n);

    Node* n1=new Node;
    n1->key=38;
    rbt.insert(n1);


    Node* n2=new Node;
    n2->key=31;
    rbt.insert(n2);


    Node* n3=new Node;
    n3->key=12;
    rbt.insert(n3);

    Node* n4=new Node;
    n4->key=19;
    rbt.insert(n4);

    Node* n5=new Node;
    n5->key=8;
    rbt.insert(n5);

    rbt.delete_(n5);
    rbt.delete_(n3);
    rbt.delete_(n4);
    rbt.delete_(n2);
    rbt.delete_(n1);
    rbt.delete_(n);
    rbt.inorder_tree_walk(rbt.root);
    
    int i;
  
}

*/


/*
.: ⡴⠑⡄⠀⠀⠀⠀⠀⠀⠀⣀⣀⣤⣤⣤⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ 
⠸⡇⠀⠿⡀⠀⠀⠀⣀⡴⢿⣿⣿⣿⣿⣿⣿⣿⣷⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠑⢄⣠⠾⠁⣀⣄⡈⠙⣿⣿⣿⣿⣿⣿⣿⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⢀⡀⠁⠀⠀⠈⠙⠛⠂⠈⣿⣿⣿⣿⣿⠿⡿⢿⣆⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⢀⡾⣁⣀⠀⠴⠂⠙⣗⡀⠀⢻⣿⣿⠭⢤⣴⣦⣤⣹⠀⠀⠀⢀⢴⣶⣆ 
⠀⠀⢀⣾⣿⣿⣿⣷⣮⣽⣾⣿⣥⣴⣿⣿⡿⢂⠔⢚⡿⢿⣿⣦⣴⣾⠁⠸⣼⡿ 
⠀⢀⡞⠁⠙⠻⠿⠟⠉⠀⠛⢹⣿⣿⣿⣿⣿⣌⢤⣼⣿⣾⣿⡟⠉⠀⠀⠀⠀⠀ 
⠀⣾⣷⣶⠇⠀⠀⣤⣄⣀⡀⠈⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀ 
⠀⠉⠈⠉⠀⠀⢦⡈⢻⣿⣿⣿⣶⣶⣶⣶⣤⣽⡹⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⠀⠉⠲⣽⡻⢿⣿⣿⣿⣿⣿⣿⣷⣜⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣷⣶⣮⣭⣽⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⣀⣀⣈⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⠀⠹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠻⠿⠿⠿⠿⠛⠉
*/