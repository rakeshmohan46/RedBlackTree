#include <iostream>
#include "REDBLACK.h"

using namespace std;

int main(void)
{
  Node * root = NULL;


  int choice = 1;
  while(choice != 0)
  {
    cout <<" 0 = Exit the program\n";
    cout <<" 1 = Insert a node\n";
    cout <<" 2 = Delete a node\n";
    cout <<" 3 = Print the tree\n";
    cout <<"Choose number: ";
    cin >>choice;
    switch(choice)
    {
      case 0:
        break;
      case 1:
        {
          int key;
          cout <<"Give the key of the node to insert:\n";
          cin >>key;
          rbInsert(root, new Node(key));
        }
        break;
      case 2:
        {
          int key;
          cout <<"Give the key of the node to delete:\n";
          cin >>key;
          Node * p = search(root, key);
          rbDelete(root, p);
        }
        break;
      case 3:
        printTree(root, 0);
        break;
    }
  }

  return 0;
}
