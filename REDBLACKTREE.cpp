#include <cstdlib>
#include <iostream>
#include "REDBLACK.h"

using namespace std;

Node * nil = new Node(0,BLACK);

//Constructor
Node::Node(int key, COLOR color){
  key(key),
  color(color),
  left(NULL),
  right(NULL),
  parent(NULL)
}

 Node::Node(int key){
 key(key),
 color(RED),
 left(NULL),
 right(NULL),
 parent(NULL)
 }

//Destructor

Node::~Node()
{
  if(left != NULL)
    left->parent = NULL;
  if(right != NULL)
    right->parent = NULL;
  delete left;
  delete right;
}

void insert(Node * & T, Node * pNode)
{
  //if(pNode == NULL)
    //return;   //nothing to insert

  if( T == NULL )  //Create a null tree
  {
    T = pNode;
    T->left = T->right = T->parent = NULL;
    return;
  }

  Node * p = T;
  Node * q = NULL;

  while(p != NULL)
  {
    if(pNode->getKey() < p->getKey())
    {
      q = p;
      p = p->left;
    }
    else
    {
      q = p;
      p = p->right;
    }
  }

  //q points to the node after which pNode has to be inserted
  if(pNode->getKey() < q->getKey())
  {
    q->left = pNode;
    pNode->parent = q;
  }
  else
  {
    q->right = pNode;
    pNode->parent = q;
  }
}

//
void printTree(Node * root, int counter)
{
  if(root == NULL)
    return;
  counter += 5;
  printTree(root->right, counter);
  for(int i = 0; i<=counter; i++)
    cout <<" ";
  char color = root->getColor() == RED ? 'r':'b';
  cout <<"("<< root->getKey() <<"|"<< color <<")\n";
  printTree(root->left, counter);
}

void leftRot(Node * & root, Node * x)
{
  if(x->right == NULL)
    return;
  Node * y = x->right;
  x->right = y->left;
  if(y->left != NULL)
    y->left->parent = x;
  y->parent = x->parent;
  if(x->parent == NULL)
    root = y;
  else
  {
    if(x == x->parent->left)  //x is the left child of its parent
      x->parent->left = y;
    else					            //y is the right child of its parent
      x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}


void rightRot(Node * & root, Node * x)
{
  if(x->left == NULL)
    return;
  Node * y = x->left;
  x->left = y->right;
  if(y->right != NULL)
    y->right->parent = x;
  y->parent = x->parent;
  if(x->parent == NULL)
    root = y;
  else
  {
    if(x == x->parent->left)
      x->parent->left = y;
    else
      x->parent->right = y;
  }
  y->right = x;
  x->parent = y;
}

void rbInsert(Node * & root, Node * x)
{
  if(x == NULL)
    return;   //nothing to insert

  insert(root, x);
  x->setColor(RED);

  if(x->parent == NULL || x->parent->parent == NULL)  //we have inserted the root its child
  {
    root->setColor(BLACK);		//by convention the root is always BLACK
    return;
  }

  //the inserted node x is neither the root, nor any of its children
  while( x != root && x->parent->getColor() == RED )
  {
    if(x->parent == x->parent->parent->left) //x->parent is a left child
    {
      Node * y = x->parent->parent->right;  //y = the uncle of x
      COLOR coY;
      if(y == NULL)
        coY = BLACK;
      else
        coY = y->getColor();
      if(coY == RED)    //Insert fixup Case 1
      {
        x->parent->setColor(BLACK);
        y->setColor(BLACK);
        y->parent->setColor(RED);
        x = x->parent->parent;
      }
      else  //color of y is BLACK (it could be NULL)
      {
        if(x == x->parent->right)  //Case 2
        {
          x = x->parent;
          leftRot(root, x);
        }
      //Case 3
        x->parent->setColor(BLACK);
        x->parent->parent->setColor(RED);
        rightRot(root, x->parent->parent);
      }
    }
    else  //symmetric case when x->parent is a right child
    {
      Node * y = x->parent->parent->left;			//y = the uncle of x
      COLOR coY;
      if(y == NULL)
        coY = BLACK;
      else
        coY = y->getColor();
      if(coY == RED)
      {
        y->parent->setColor(RED);  //y cannot be null since it is RED
        y->setColor(BLACK);
        x->parent->setColor(BLACK);
        x = x->parent->parent;
      }
      else   //color of Y is BLACK;
      {
        if(x == x->parent->left)  //case 2
        {
          x = x->parent;
          rightRot(root, x);
        }
        //case 3
        x->parent->parent->setColor(RED);
        x->parent->setColor(BLACK);
        leftRot(root,x->parent->parent);
      }
    }
  }
  root->setColor(BLACK);
}

Node * successor(Node * root)
{
  if(root == NULL)
    return NULL;
  if(root->right == NULL)
    return NULL;
  Node * p = root->right;
  while(p->left != NULL)
    p = p->left;
  return p;
}

void rbDelete(Node * & root, Node * z)
{
  if(z == NULL)
    return;			//nothing to delete
  if(root == NULL)
    return;
  if(z == root && z->left == NULL && z->right == NULL)
  {
    z->parent = NULL;
    delete z;
    root = NULL;
    return;
  }
  //there are at least 2 nodes in the tree
  Node * y = NULL;
  Node * x = NULL;
  if(z->left == NULL || z->right == NULL)
    y = z;
  else
    y = successor(z);
  if(y->left != NULL)
    x = y->left;
  else
    x = y->right;
  if(x != NULL)
    x->parent = y->parent;
  else
  {
    x = nil;
    x->parent = y->parent;
  }
  if(y->parent == NULL)
    root = x;

  else
    if(y == y->parent ->left)
      y->parent->left = x;
    else
      y->parent->right = x;
  if(y != z)
    z->setKey(y->getKey());
  COLOR colY = y->getColor();
  //we delete y
  y->left = y->right = y->parent = NULL;
  delete y;
  if(colY == BLACK)	//we might have violated property 4 or 3
  {
    if(x->getColor() == RED)
      x->setColor(BLACK);
    else
      rbFix(root,x);
  }
  else  //we don't need to fix, we delete x if it is a sentinel
    if(x == nil)
    {
      if(x == x->parent->left)
        x->parent->left = NULL;
      else
        x->parent->right = NULL;
      x->parent = x->left = x->right = NULL;
      x->setColor(BLACK);
    }
}


COLOR colorOf(Node * p)
{
  if(p != NULL)
    return p->getColor();
  else
    return BLACK;
}

void removeSentinel(Node * p)
{
  if(p == nil)
  {
    if(p == p->parent->left)
      p->parent->left = NULL;
    else
      p->parent->right = NULL;
    p->parent = p->left = p->right = NULL;
    p->setColor(BLACK);     
  }
}

void rbFix(Node * & root, Node * x)
{
  Node * w = NULL;
  while( x != root && x->getColor() == BLACK )
  {
    if(x == x->parent->left)   //x is a left child
    {
      w = x->parent->right;
      if(w != NULL)
      {
        if(w->getColor() == RED)  //Case1  (brother of x is RED)
        {
          w->parent->setColor(RED);  //w cannot be NULL, since it is RED
          w->setColor(BLACK);
          leftRot(root, x->parent);
          w = x->parent->right;
        }
        else //w is BLACK => Cases 2(brother of x is black and both of w's child are black)
        //, 3(brother of x is black,w's left child is red and w's right child is black)
        //or 4(brother of x, w is black and w's right child is red)
        {
          if(colorOf(w->left) == BLACK && colorOf(w->right) == BLACK)  //case 2
          {
            w->setColor(RED);
            Node * p = x;
            x = x->parent;
            x->setColor(BLACK);
            removeSentinel(p);
          }
          else  //not both children of w are BLACK => case3 or case4
          {
            if(colorOf(w->right) == BLACK)  //i.e. w->left == RED => Case 3
            {
              w->setColor(RED);
              w->left->setColor(BLACK); //w->left is RED hence it cannot be NULL
              rightRot(root, w);
            }
              else //case4   w->right == RED
            {
              w->setColor(x->parent->getColor());
              x->parent->setColor(BLACK);
              w->right->setColor(BLACK);
              leftRot(root, x->parent);
              Node * p = x;
              x = root;  //we have finished
              removeSentinel(p);
            }
          }
        }
      }
      else      //w is NULL
      {
        //set x->parent BLACK, pass x up to its parent and remove the sentinel if necessary
        x->parent->setColor(BLACK);
        Node * p = x;
        x = x->parent;
        removeSentinel(p);
      }
    }
    else  //symmetric case if x is a right child
    {
      w = x->parent->left;
      if(w != NULL)
      {
        if(w->getColor() == RED) //Case 1
        {
          w->parent->setColor(RED);  //w cannot be NULL, since it is RED
          w->setColor(BLACK);
          rightRot(root, x->parent);
          w = x->parent->left;
        }
        else  //color of w is BLACK => case 2, 3 or 4
        {
          if(colorOf(w->left) == BLACK && colorOf(w->right) == BLACK)  //case 2
          {
            w->setColor(RED);
            Node * p = x;
            x = x->parent;
            x->setColor(BLACK);
            removeSentinel(p);
          }
          else  //not both children of w are BLACK => case3 or case4
          {
            if(colorOf(w->left) == BLACK)  //i.e. w->right == RED => Case 3
            {
              w->setColor(RED);
              w->right->setColor(BLACK); //w->left is RED hence it cannot be NULL
              leftRot(root, w);
            }
              else //case4   w->left == RED
            {
              w->setColor(x->parent->getColor());
              x->parent->setColor(BLACK);
              w->left->setColor(BLACK);
              rightRot(root, x->parent);
              Node * p = x;
              x = root;  //we have finished
              removeSentinel(p);
            }
          }
        }
      }
      else   //w is NULL
      {
        Node * p = x;
        x = x->parent;
        x->setColor(BLACK);
        removeSentinel(p);
      }
    }
  }
  root->setColor(BLACK);
}

Node * search(Node * root, int keyToSearch)
{
  if(root == NULL)
    return NULL;
  Node * p = root;
  while( p != NULL)  //infinite loop
  {
    int key = p->getKey();
    if(key == keyToSearch)
      return p;
    if(keyToSearch < key)
      p = p->left;
    else
      p = p->right;
  }
  return NULL;
}
