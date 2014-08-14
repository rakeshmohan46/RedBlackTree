#ifndef RBTREE_H
#define RBTREE_H

using namespace std;
#include <cstdlib>
#include <iostream>
//#include <stdio>

enum COLOR { RED, BLACK, DB_BLACK };

class Node
{
  int key;
  COLOR color;
public:
  Node(int,COLOR);
  Node(int);
  ~Node();


  int getKey() const {
    return key; }
  void setKey(int key) {
    this->key = key; }
  COLOR getColor() const {
    return color; }
  void setColor(COLOR c) {
    color = c; }

  Node * left;
  Node * right;
  Node * parent;

};

extern Node * nil;

void insert(Node * & , Node * );
void printTree(Node *, int);
void leftRot(Node * &, Node *);
void rightRot(Node * &, Node *);
void rbInsert(Node * &, Node *);
Node * successor(Node *);
void rbDelete(Node * & , Node * );
void rbFix(Node * & , Node * );
Node * search(Node *, int);

#endif
