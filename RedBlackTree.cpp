#include "RedBlackTree.h"
void printVec(vector<char> v)
{
	for(int i = 0; i<v.size(); i++)
	{
		cout << v[i] << " ";
	}
	cout << endl;
}
void printVec(vector<int> v)
{
	for(int i = 0; i<v.size(); i++)
	{
		cout << v[i] << " ";
	}
	cout << endl;
}
// template <class T1, class T2>
// void RedBlackTree<T1, T2>::printPreorder(NodeT<T1, T2>* node)
// {
//     if (node!= nullptr)
//     {
//         cout << "Key: " << node->key << " Val: " << node->val << " Colour:" << node->isBlack;
//         if(node->parent == nullptr)
//         {
//             cout << " Parent: nullptr" << endl;
//         }
//         else
//         {
//             cout << " Parent: " << node->parent->key << endl;
//         }
//         printPreorder(node->left);
//         printPreorder(node->right);
//     }
    
// }

void simpleTest()
{
	// Int Tree Tests	
	RedBlackTree<int, char> rb1;
	if (rb1.insert(6, 'f')) cout << "inserted 42" << endl;
	rb1.insert(10, 'j');
	rb1.insert(2, 'b');
	RedBlackTree<int, char> rb2(rb1);
 
	if (rb1.remove(2)) cout << "removed 2" << endl;
	if (rb1.search(6)) cout << "found 6" << endl;
 
	vector<char> v1 = rb1.search(21, 8); //should contain j
	vector<char> v2 = rb1.values(); //should contain {f, j}
	vector<int> v3 = rb1.keys(); //should contain {6, 10}
	printVec(v1);
	printVec(v2);
	printVec(v3);
	cout << "size = " << rb1.size() << endl; //should be 2
	NodeT<int, char>* pRoot = rb1.getRoot(); //BAD design - for our testing only
}
int main(void)
{
	simpleTest();
    return 0;
}