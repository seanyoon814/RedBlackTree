#include <iostream>
#include <vector>

using std::vector;
using std::string;
using std::endl;
using std::cout;
//node class
template <class T1, class T2>
class NodeT{
    public:
        //public variables
        T1 key;
        T2 val;
        NodeT* left;
        NodeT* right;
        NodeT* parent;
        bool isBlack;
        //constructor, sets all node pointers to a nullptr, and initially sets the node to red
        //input: key template value, value template value, 
        //output: new nodeT object
        NodeT(T1 k, T2 v)
        {
            key = k;
            val = v;
            left = nullptr;
            right = nullptr;
            parent = nullptr;
            isBlack = false;
        }
};
//redblack tree class
template <class T1, class T2>
class RedBlackTree{
    public:
        //constructor
        RedBlackTree();
        //copy constructor
        RedBlackTree(const RedBlackTree<T1, T2> &obj);
        //assignment operator
        RedBlackTree & operator=(const RedBlackTree<T1, T2> &obj);
        //destructor
        ~RedBlackTree();
        //insert method
        bool insert(T1 key, T2 val);
        //remove method
        bool remove(T1 key);
        //search method
        bool search(T1 key) const;
        //search method: vector
        vector<T2> search(T1 val1, T1 val2);
        //values method
        vector<T2> values();
        //keys method
        vector<T1> keys();
        //size method
        int size() const;
        //root method
        NodeT<T1, T2>* getRoot() const;
        // void printPreorder(NodeT* n);
    private:
        //node for the root
        NodeT<T1, T2>* root;
        //current size variable
        int currentSize;
        //from slides
        //left rotation method
        void leftRotate(NodeT<T1, T2>* x);
        //symmetric to leftRotate()
        //right rotation method
        void rightRotate(NodeT<T1, T2>* x);
        //check for duplicates method
        bool checkDuplicates(NodeT<T1, T2>* n, T1 key);
        //https://www.geeksforgeeks.org/binary-search-tree-set-1-search-and-insertion/
        //binary search tree insert method
        NodeT<T1, T2>* bstInsert(NodeT<T1, T2>* n, T1 key, T2 val);
        //search method
        NodeT<T1, T2>* search(NodeT<T1, T2>* n, T1 key);
        //red black tree fix method (after deletion)
        void rbFix(NodeT<T1, T2>* x, NodeT<T1, T2>* parent, bool isRightChild);
        //predecessor method
        NodeT<T1, T2>* predecessor(NodeT<T1, T2>* n);
        //pass by reference
        //key order method
        void findOrderByKeys(NodeT<T1, T2>* n, vector<T1> &v);
        //val order method
        void findOrderByVals(NodeT<T1, T2>* n, vector<T2> &v);
        //clear nodes method (destructor)
        void clearNodes(NodeT<T1, T2>* n);
        //copynodes method
        NodeT<T1, T2>* copyNodes(NodeT<T1, T2>* n, NodeT<T1, T2>* parent);
};
////////////////////////
//PUBLIC METHODS
////////////////////////

//default constructor
//input: none
//output: new redblacktree obj, with root set to nullptr and currentsize to 0
template <class T1, class T2>
RedBlackTree<T1, T2>::RedBlackTree()
{
   root = nullptr;
   currentSize = 0;
}
//copy constructor
//input: redblacktree reference parameter
//output: new redblacktree obj, with properties copied from obj parameter
template <class T1, class T2>
RedBlackTree<T1, T2>::RedBlackTree(const RedBlackTree<T1, T2> &obj)
{
    //copy all the nodes from ref obj 
    root = copyNodes(obj.root, nullptr);
    currentSize = obj.currentSize;
}
//assignment operator
//input: redblacktree reference parameter
//output: new redblack tree obj, with properties copied from obj parameter
template <class T1, class T2>
RedBlackTree<T1, T2> & RedBlackTree<T1, T2>::operator=(const RedBlackTree<T1, T2> &obj)
{
    //checking to see if theyre equal
    if(this!=&obj)
    {
        //deallocate any memory, and then copy all nodes and set the size
        clearNodes(root);
        root = copyNodes(obj.root, nullptr);
        currentSize = obj.currentSize;
    }
    return *this;
}
//destructor
//input: none
//output: none, but deallocates any memory in a redblacktree object
template <class T1, class T2>
RedBlackTree<T1, T2>::~RedBlackTree()
{
    clearNodes(root);
}
//insert method
//input: template variable T1 (key), template variable T2 (val)
//output: returns true if inserted, otherwise returns false
template <class T1, class T2>
bool RedBlackTree<T1, T2>::insert(T1 key, T2 val)
{
    //check to see if they are any duplicate keys
    if(checkDuplicates(root, key))
    {
        return false;
    }
    //if the tree is empty, we can save a lot of time by doing only these functions
    if(root == nullptr)
    {
        root = bstInsert(root, key, val);
        root->isBlack = true;
        currentSize++;
        return true;
    }
    //if the currentsize is at least 1, we run this
    else
    {
        //insert like normally
        bstInsert(root, key, val);
        //find the node we just inserted
        NodeT<T1, T2>* x = search(root, key);
        //initially red
        x->isBlack = false;
        //while the parent is red we run this
        while(x != root && x->parent->isBlack == false)
        {   
            //if the parent is a left child
            if(x->parent == x->parent->parent->left)
            {
                //find the uncle of the node
                NodeT<T1, T2>* y = x->parent->parent->right;
                //if the uncle is black or is a nullptr
                if(y == nullptr || y->isBlack)
                {
                    //fix the tree by rotations
                    if(x == x->parent->right)
                    {
                        x = x->parent;
                        leftRotate(x);
                    }
                    //set the parent to black, and the preceding node to red (rb tree property)
                    x->parent->isBlack = true;
                    x->parent->parent->isBlack = false;
                    //fix the tree
                    rightRotate(x->parent->parent);
                }
                //if the uncle is red
                else
                {
                    //bunch of colour switches to hold redblack tree properties
                    x->parent->isBlack = true;
                    y->isBlack = true;
                    x->parent->parent->isBlack = false;
                    x = x->parent->parent;
                }
            }
            //symmetric to code above; if the parent is a right child
            else
            { 
                //find the uncle of the node
                NodeT<T1, T2>* y = x->parent->parent->left;
                //if the uncle is black or is a nullptr
                if(y == nullptr || y->isBlack)
                {
                    //fix the tree by rotations
                    if(x == x->parent->left)
                    {
                        x = x->parent;
                        rightRotate(x);
                    }
                    //set the parent to black, and the preceding node to red (rb tree property)
                    x->parent->isBlack = true;
                    x->parent->parent->isBlack = false;
                    //fix the tree
                    leftRotate(x->parent->parent);
                }
                //if the uncle is red
                else
                {
                    //bunch of colour switches to hold redblack tree properties
                    x->parent->isBlack = true;
                    y->isBlack = true;
                    x->parent->parent->isBlack = false;
                    x = x->parent->parent;
                }
            }
        }
        //set the root to black
        root->isBlack = true;
    }
    //increment size by 1
    currentSize++;
    return true;
}
//slides
//remove method
//input: key value which the user wants to remove
//output: returns true if successfully removed, or else returns false
template <class T1, class T2>
bool RedBlackTree<T1, T2>::remove(T1 key)
{
    //search for the node we want to remove
    NodeT<T1, T2>* z = search(root, key);
    //if the node does not exist, return false
    if(z == nullptr)
    {
        return false;
    }
    //replacement node
    NodeT<T1, T2>* y;
    //removal node
    NodeT<T1, T2>* x;
    //node for x parent, since x could be a nullptr
    NodeT<T1, T2>* xParent;
    //bool to check if x is a left or right child
    bool isRight;
    //case where node we want to remove has 1 or 2 children
    if(z->left == nullptr ||z->right == nullptr)
    {
        y = z;
    }
    //no children
    else
    {
        //find the predecessor (rightmost node in left subtree)
        y = predecessor(z);
    }
    //if the left child exists
    if(y->left!=nullptr)
    {
        //set the removal node to left replacement node
        x = y->left;
    }
    //if the right child exists
    else
    {
        //set the removal node to right replacement node
        x = y->right;
    }
    //if x does not equal nullpointer, then we can execute the line
    //x -> parent = y -> parent
    //to detatch
    if(x!=nullptr)
    {
        x->parent = y->parent;
        xParent = x->parent;
    }
    //else, we make a reference to y for x parent, since x is either
    //its left or right child
    else
    {
        xParent = y->parent;
    }
    //if y parent is nullptr, it means its the root
    if(y->parent == nullptr)
    {
        //set to root
        root = x;
    }
    //else, it is any other node on the tree
    else
    {
        //if y is a left child
        if(y == y->parent->left)
        {
            //attatch removal node to replacement node (left child)
            y->parent->left = x;
            //set the rightchild bool to false
            isRight = false;
        }
        //if y is a right child
        else
        {
            //attatch removal node to replacement node (right child)
            y->parent->right = x;
            //set the rightchild bool to true
            isRight = true;
        }
    }
    //set the values
    if(y!=z)
    {
        z->key = y->key;
        z->val = y->val;
    }
    //if the node we replaced is black, we have to fix the tree
    if(y->isBlack)
    {  
        //if its a right child
        if((x == nullptr || x!=nullptr) && isRight)
        {
            rbFix(x, xParent, isRight);
        }
        //else if its a left child
        else if((x == nullptr || x!=nullptr) && !isRight)
        {
            rbFix(x, xParent, !isRight);
        }
    }
    //delete the node since we moved all of the data
    delete y;
    //decrement current size by 1
    currentSize--;
    return true;
}
//search function
//input: template key that we want to search for
//output: returns true if the key is in the tree, false otherwise
template <class T1, class T2>
bool RedBlackTree<T1, T2>::search(T1 key) const
{
    //node for the root
    NodeT<T1, T2>* n = root;
    //traverse down the tree until we reach a nullptr
    while (n != nullptr) 
    {
        //if the key is found, we return true
        if (key == n->key) 
        {
            return true;
        }
        //if the key is less than the key, traverse left
        else if (key < n->key) 
        {
            n = n->left;
        }
        //if the item is greater than the key, traverse right
        else
        {
            n = n->right;
        }
    }
    //item is not found
    return false;
}
//search function (vector)
//input: two template variables for the keys
//output: returns all the vals between the two template variables, in key order, in a vector
template <class T1, class T2>
vector<T2> RedBlackTree<T1, T2>::search(T1 val1, T1 val2)
{
    //vector to be returned
    vector<T2> v;
    //loop through the vector
    //if val 2 > val 1 (since they are not specific upper/lower bounds)
    if(val1< val2)
    {
        for(int i = val1; i<=val2; i++)
        {
            //if the node is in the tree
            if(search(i))
            {
                //find the node
                NodeT<T1, T2>* n = search(root, i);
                //put the element to the back of the vector, so its in ascending order
                v.push_back(n->val);
            }
        }
    }
    //else 
    else
    {
        for(int i = val2; i>=val1; i--)
        {
            //if the node is in the tree
            if(search(i))
            {
                //find the node
                NodeT<T1, T2>* n = search(root, i);
                //put the element to the back of the vector, so its in ascending order
                v.insert(v.begin(), n->val);
            }
        }
    }
    return v;
}
//values function
//input: none
//output: returns all the values in the tree, in key order, in a vector
template <class T1, class T2>
vector<T2> RedBlackTree<T1, T2>::values()
{
    //vector to be returned
    vector<T2> v;
    //helper function to find the key order, by vals
    //pass the vector by reference 
    findOrderByVals(root, v);
    return v;
}
//keys function
//input: none
//output: returns all the keys in the tree, in key order, in a vector
template <class T1, class T2>
vector<T1> RedBlackTree<T1, T2>::keys()
{
    //vector to be returned
    vector<T1> v;
    //helper function to find the key order, by keys
    //pass the vector by reference 
    findOrderByKeys(root, v);
    return v;
}
//size function
//input: none
//output: returns the current size of the tree as an int
template <class T1, class T2>
int RedBlackTree<T1, T2>::size() const
{
    return currentSize;
}
//get root function
//bad practice but used for this assignment
//input: none
//output: returns a pointer to the root of the rb tree
template <class T1, class T2>
NodeT<T1, T2>* RedBlackTree<T1, T2>::getRoot() const
{
    return root;
}

////////////////////////
//PRIVATE METHODS
////////////////////////

//left rotation
//input: pointer to the node we do the rotation around
//output: none, but the calling obj gets rotated left
template <class T1, class T2>
void RedBlackTree<T1, T2>::leftRotate(NodeT<T1, T2>* x)
{
    //get the right child
    NodeT<T1, T2>* y = x->right;
    //set node right child to the right -> left child
    x->right = y->left;
    if(y->left!=nullptr)
    {
        y->left->parent = x;
    }
    //set y parent to x parent
    y->parent = x->parent;
    //if x -> parent is a nullptr, it means its the root
    if(x->parent == nullptr)
    {
        root = y;
    }
    //detatch x, check if its a left child first
    else if(x == x->parent->left)
    {
        x->parent->left = y;
    }
    else
    {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}
//right rotation
//input: pointer to the node we do the rotation around
//output: none, but the calling obj gets rotated right
template <class T1, class T2>
void RedBlackTree<T1, T2>::rightRotate(NodeT<T1, T2>* x)
{
    //get the left child
    NodeT<T1, T2>* y = x->left; 
    //set node left child to the left -> right child
    x->left = y->right;
    if(y->right!=nullptr)
    {
        y->right->parent = x;
    }   
    //set parents equal
    y->parent = x->parent;
    //if x -> parent is a nullptr, it means its the root
    if(x->parent == nullptr)
    {
        root = y;
    }
    //detatch x, check if its a left child first
    else if(x == x->parent->right)
    {
        x->parent->right = y;
    }
    else
    {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}
//checkduplicates
//input: node (most cases root) pointer, and the key we want to check duplicates for
//output: returns true if we find matching key val, false otherwise
template <class T1, class T2>
bool RedBlackTree<T1, T2>::checkDuplicates(NodeT<T1, T2>* n, T1 key)
{
    //recursively call until we reach a nullptr
    if(n!=nullptr)
    {
        //if we find the key, return true
        if(key == n->key)
        {
            return true;
        }
        //search the left subtree first
        checkDuplicates(n->left, key);
        //and then the right
        checkDuplicates(n->right, key);
    }
    //return false if not found
    return false;
}
//https://www.geeksforgeeks.org/binary-search-tree-set-1-search-and-insertion/
//input: node we start traversing from (usually root), the key and val
//output: returns the (unchanged) root, with the node inserted
template <class T1, class T2>
NodeT<T1, T2>* RedBlackTree<T1, T2>::bstInsert(NodeT<T1, T2>* n, T1 key, T2 val)
{
    //if we finish traversing, we insert the node here
    if(n == nullptr)
    {
        return new NodeT<T1, T2>(key, val);
    }
    //if the key is greater than the current node key, traverse right
    if(key > n->key)
    {
        NodeT<T1, T2>* rchild = bstInsert(n->right, key, val);
        //set the parent nodes, and corresponding left/right nodes
        n->right = rchild;
        rchild->parent = n;
    }
    //if the key is less than the current node key, traverse left
    else
    {
        NodeT<T1, T2>* lchild = bstInsert(n->left, key, val);
        //set the parent nodes, and corresponding left/right nodes
        n->left = lchild;
        lchild->parent = n;
    }
    return n;
}
//search method
//input: node we start traversing from (usually root), and template key value we want to find
//output: corresponding node with the template key value 
template <class T1, class T2>
NodeT<T1, T2>* RedBlackTree<T1, T2>::search(NodeT<T1, T2>* n, T1 key)
{
    //if the node is not found, return null
    if(n == nullptr)
    {
        return nullptr;
    }
    //if we find the key, return this node
    if(n->key == key)
    {
        return n;
    }
    //first search the left subtree
    NodeT<T1, T2>* found = search(n->left, key);
    if(found == nullptr)
    {
        //afterwards search the right subtree, if not in left subtree
        found = search(n->right, key);
    }
    return found;
}
//rb fix method 
//input: node we need to fix, the parent of the node, and a bool if the node is a right or left child
//output: none, but fixes the rb tree obj to hold rb tree properties
template <class T1, class T2>
void RedBlackTree<T1, T2>::rbFix(NodeT<T1, T2>* x, NodeT<T1, T2>* parent, bool isRightChild)
{
    //while the node does not equal the root, and is black (or null)
    while(x!=root && (x == nullptr || x->isBlack))
    {
        //if the node is a right child
        if(isRightChild)
        {
            //get the uncle
            NodeT<T1, T2>* y = parent->right;
            //if the uncle is red, fix colouring and rotoate the tree
            if(!y->isBlack)
            {
                y->isBlack = true;
                parent->isBlack = false;
                leftRotate(parent);
                y = parent->right;
            }
            //if the uncle is a leaf, or if it has 2 black nodes as children
            //we set the uncle to red
            if((y->left == nullptr || y->left->isBlack) &&(y->right == nullptr || y->right->isBlack))
            {
                y->isBlack = false;
                x = parent;
                parent = x->parent;
            }
            //if the uncle has 1 child
            else
            {
                //if the uncle has a right child thats a black node
                if(y->right == nullptr || y->right->isBlack)
                {
                    //fix the tree by recolouring and rotations
                    if(y->left != nullptr)
                    {
                        y->left->isBlack = true;
                    }
                    y->isBlack = false;
                    rightRotate(y);
                    y = parent->right;
                }
                //fix the tree by recolouring and rotations
                y->isBlack = parent->isBlack;
                parent->isBlack = true;
                if(y->right != nullptr)
                {
                    y->right->isBlack = true;
                }
                leftRotate(parent);
                x = root;
            }
        }
        //symmetric to code above
        //if the node is a right child
        else
        {
            //get the uncle
            NodeT<T1, T2>* y = parent->left;
            //if the uncle is red, fix colouring and rotoate the tree
            if(!y->isBlack)
            {
                y->isBlack = true;
                parent->isBlack = false;
                rightRotate(parent);
                y = parent->left;
            }
            //if the uncle is a leaf, or if it has 2 black nodes as children
            //we set the uncle to red
            if((y->left == nullptr || y->left->isBlack) &&(y->right ==nullptr || y->right->isBlack))
            {
                y->isBlack = false;
                x = parent;
                parent = x->parent;
            }
             //if the uncle has 1 child
            else
            {
                //if the uncle has a left child thats a black node
                if(y->left == nullptr || y->left->isBlack)
                {
                    //fix the tree by recolouring and rotations
                    if(y->right != nullptr)
                    {
                        y->right->isBlack = true;
                    }
                    y->isBlack = false;
                    leftRotate(y);
                    y = parent->left;
                }
                //fix the tree by recolouring and rotations
                y->isBlack = parent->isBlack;
                parent->isBlack = true;
                if(y->left != nullptr)
                {
                    y->left->isBlack = true;
                }
                rightRotate(parent);
                x = root;
            }
        }
    }
    //if out initial node was not null, we set it to black
    if(x!=nullptr)
    {
        x->isBlack = true;
    }
}
//predecessor function
//input: node to find predecessor from
//output: the predecessor (left subtree rightmost node)
template <class T1, class T2>
NodeT<T1, T2>* RedBlackTree<T1, T2>::predecessor(NodeT<T1, T2>* n)
{
    //get the left subtree
    NodeT<T1, T2>* temp = n->left;
    //find the rightmost node by traversing
    while(temp->right!=nullptr)
    {
        temp = temp->right;
    }
    return temp;
}
//order by keys function
//input: node to start traversing from (usually root), reference vector parameter
//output: none, but fills the reference parameter with key values in key order
template <class T1, class T2>
void RedBlackTree<T1, T2>::findOrderByKeys(NodeT<T1, T2>* n, vector<T1> &v)
{
    //recursively traverse (in order traversal)
    if(n!= nullptr)
    {
        //traverse to left first
        findOrderByKeys(n->left, v);
        //add the value of the data to the back of the vector, so its in order
        v.push_back(n->key);
        //traverse to the right
        findOrderByKeys(n->right, v);
    }
}
//order by values function
//input: node to start traversing from (usually root), reference vector parameter
//output: none, but fills the reference parameter with value values in key order
template <class T1, class T2>
void RedBlackTree<T1, T2>::findOrderByVals(NodeT<T1, T2>* n, vector<T2> &v)
{
    //recursively traverse (in order traversal)
    if(n!= nullptr)
    {
        findOrderByVals(n->left, v);
        //add the value of the data to the back of the vector, so its in order
        v.push_back(n->val);
        //traverse to the right
        findOrderByVals(n->right, v);
    }
}
//clear nodes function
//input: node to start deleting nodes from (usually root)
//output: none, but deallocates any memory in rb tree obj
template <class T1, class T2>
void RedBlackTree<T1, T2>::clearNodes(NodeT<T1, T2>* n)
{
    //post order traversal
    //recursively delete the nodes
    if(n!=nullptr)
    {
        //traverse left, and then to the right, and delete the nodes recursively
        clearNodes(n->left);
        clearNodes(n->right);
        delete n;
    }
    //set the size to 0, and root to nullptr
    currentSize = 0;
    root = nullptr;
}
//copy nodes function
//input: node to start deleting from (usually root), the parent of the node
//output: returns a nullptr, and allocates memory to the calling obj based on the calling object
//start off by calling the parent as a nullptr, if starting from root
template <class T1, class T2>
NodeT<T1, T2>* RedBlackTree<T1, T2>::copyNodes(NodeT<T1, T2>* n, NodeT<T1, T2>* parent)
{
    //recursively copy nodes (post order)
    if(n != nullptr)
    {
        //allocate memory for a new node
        NodeT<T1, T2>* newNode = new NodeT<T1, T2>(n->key, n->val);
        //set properties
        newNode->parent = parent;
        newNode->isBlack = n->isBlack;
        //recursively copy the nodes
        newNode->left = copyNodes(n->left, newNode);
        newNode->right = copyNodes(n->right, newNode);
        return newNode;
    }
    return nullptr;
}