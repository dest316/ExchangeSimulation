#include <iostream>

using namespace std;

// data structure that represents a node in the tree
template <class T>
struct Node {
	T data; // holds the key
	Node* parent; // pointer to the parent
	Node* left; // pointer to left child
	Node* right; // pointer to right child
	int color; // 1 -> Red, 0 -> Black
	int count; // ���-�� ����� ���������, �������������� � ������
};




// class RBTree implements the operations in Red Black Tree
template <class T>
class RBTree {
	friend class RBTreeIterator;
private:
	Node<T>* root;
	Node<T>* TNULL;

	// initializes the nodes with appropirate values
	// all the pointers are set to point to the null pointer
	void initializeNULLNode(Node<T>* node, Node<T>* parent) {
		node->data = 0;
		node->parent = parent;
		node->left = nullptr;
		node->right = nullptr;
		node->color = 0;
	}

	void preOrderHelper(Node<T>* node) {
		if (node != TNULL) {
			cout << node->data << " ";
			preOrderHelper(node->left);
			preOrderHelper(node->right);
		}
	}

	void inOrderHelper(Node<T>* node) {
		if (node != TNULL) {
			inOrderHelper(node->left);
			cout << node->data << " ";
			inOrderHelper(node->right);
		}
	}

	void postOrderHelper(Node<T>* node) {
		if (node != TNULL) {
			postOrderHelper(node->left);
			postOrderHelper(node->right);
			cout << node->data << " ";
		}
	}

	void foundKeyOrder(Node<T>* node, int key, bool& flag) {
		if (node != TNULL) {
			if (key == node->data)
			{
				flag = true;
				return;
			}
			foundKeyOrder(node->left, key, flag);
			foundKeyOrder(node->right, key, flag);
		}
	}

	bool foundKey(int key) {
		bool fl = false;
		foundKeyOrder(this->root, key, fl);
		if (fl)
		{
			return true;
		}
		return false;
	}

	void countOrder(Node<T>* node, int& value, int& count) {	//���������� ����� preorder, ������� ��������� �������� ������� � ������� �� ����������
		if (node != TNULL)
		{
			value += node->data;
			count++;
			countOrder(node->left, value, count);
			countOrder(node->right, value, count);
		}
	}



	double averageKeyValue(Node<T>* node) { //����� ������� ������� ��������������
		int value = 0;
		int count = 0;
		countOrder(root, value, count); //����� ����� � �����
		return ((double)value / count); //���������� �.�.
	}

	Node<T>* searchTreeHelper(Node<T>* node, int key) {
		if (node == TNULL || key == node->data) {
			return node;
		}

		if (key < node->data) {
			return searchTreeHelper(node->left, key);
		}
		return searchTreeHelper(node->right, key);
	}

	// fix the rb tree modified by the delete operation
	void fixDelete(Node<T>* x) {
		Node<T>* s;
		while (x != root && x->color == 0) {
			if (x == x->parent->left) {
				s = x->parent->right;
				if (s->color == 1) {
					// case 3.1
					s->color = 0;
					x->parent->color = 1;
					leftRotate(x->parent);
					s = x->parent->right;
				}

				if (s->left->color == 0 && s->right->color == 0) {
					// case 3.2
					s->color = 1;
					x = x->parent;
				}
				else {
					if (s->right->color == 0) {
						// case 3.3
						s->left->color = 0;
						s->color = 1;
						rightRotate(s);
						s = x->parent->right;
					}

					// case 3.4
					s->color = x->parent->color;
					x->parent->color = 0;
					s->right->color = 0;
					leftRotate(x->parent);
					x = root;
				}
			}
			else {
				s = x->parent->left;
				if (s->color == 1) {
					// case 3.1
					s->color = 0;
					x->parent->color = 1;
					rightRotate(x->parent);
					s = x->parent->left;
				}

				if (s->right->color == 0 && s->left->color == 0) {
					// case 3.2
					s->color = 1;
					x = x->parent;
				}
				else {
					if (s->left->color == 0) {
						// case 3.3
						s->right->color = 0;
						s->color = 1;
						leftRotate(s);
						s = x->parent->left;
					}

					// case 3.4
					s->color = x->parent->color;
					x->parent->color = 0;
					s->left->color = 0;
					rightRotate(x->parent);
					x = root;
				}
			}
		}
		x->color = 0;
	}


	void rbTransplant(Node<T>* u, Node<T>* v) {
		if (u->parent == nullptr) {
			root = v;
		}
		else if (u == u->parent->left) {
			u->parent->left = v;
		}
		else {
			u->parent->right = v;
		}
		v->parent = u->parent;
	}

	void deleteNodeHelper(Node<T>* node, int key) {
		// find the node containing key
		if (foundKey(key) && searchTree(key)->count > 1)
		{
			searchTree(key)->count--;
			return;
		}

		Node<T>* z = TNULL;
		Node<T>* x, y;
		while (node != TNULL) {
			if (node->data == key) {
				z = node;
			}

			if (node->data <= key) {
				node = node->right;
			}
			else {
				node = node->left;
			}
		}

		if (z == TNULL) {
			cout << "Couldn't find key in the tree" << endl;
			return;
		}
		

		y = z;
		int y_original_color = y->color;
		if (z->left == TNULL) {
			x = z->right;
			rbTransplant(z, z->right);
		}
		else if (z->right == TNULL) {
			x = z->left;
			rbTransplant(z, z->left);
		}
		else {
			y = maximum(z->left); //���� minimum(z->right)
			y_original_color = y->color;
			x = y->left;
			if (y->parent == z) {
				x->parent = y;
			}
			else {
				rbTransplant(y, y->left);
				y->left = z->left;
				y->left->parent = y;
			}

			rbTransplant(z, y);
			y->right = z->right;
			y->right->parent = y;
			y->color = z->color;
		}
		delete z;
		if (y_original_color == 0) {
			fixDelete(x);
		}
	}

	// fix the red-black tree
	void fixInsert(Node<T>* k) {
		Node<T>* u;
		while (k->parent->color == 1) {
			if (k->parent == k->parent->parent->right) {
				u = k->parent->parent->left; // uncle
				if (u->color == 1) {
					// case 3.1
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				}
				else {
					if (k == k->parent->left) {
						// case 3.2.2
						k = k->parent;
						rightRotate(k);
					}
					// case 3.2.1
					k->parent->color = 0;
					k->parent->parent->color = 1;
					leftRotate(k->parent->parent);
				}
			}
			else {
				u = k->parent->parent->right; // uncle

				if (u->color == 1) {
					// mirror case 3.1
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				}
				else {
					if (k == k->parent->right) {
						// mirror case 3.2.2
						k = k->parent;
						leftRotate(k);
					}
					// mirror case 3.2.1
					k->parent->color = 0;
					k->parent->parent->color = 1;
					rightRotate(k->parent->parent);
				}
			}
			if (k == root) {
				break;
			}
		}
		root->color = 0;
	}

	void printHelper(Node<T>* root, string indent, bool last) {
		// print the tree structure on the screen
		if (root != TNULL) {
			cout << indent;
			if (last) {
				cout << "R----";
				indent += "     ";
			}
			else {
				cout << "L----";
				indent += "|    ";
			}

			string sColor = root->color ? "RED" : "BLACK";
			cout << root->data << "(" << sColor << ")" << endl;
			printHelper(root->left, indent, false);
			printHelper(root->right, indent, true);
		}
		// cout<<root->left->data<<endl;
	}

public:
	RBTree() {
		TNULL = new Node<T>;
		TNULL->color = 0;
		TNULL->left = nullptr;
		TNULL->right = nullptr;
		root = TNULL;
	}

	~RBTree()
	{
		clear();
	}

	// Pre-Order traversal
	// Node->Left Subtree->Right Subtree
	void preorder() {
		preOrderHelper(this->root);
	}

	// In-Order traversal
	// Left Subtree -> Node -> Right Subtree
	void inorder() {
		inOrderHelper(this->root);
	}

	// Post-Order traversal
	// Left Subtree -> Right Subtree -> Node
	void postorder() {
		postOrderHelper(this->root);
	}

	void printAverageKeyValue()
	{
		cout << averageKeyValue(this->root) << endl; //������������� ������� �������� ����� � ������
	}

	void clear() {
		while (this->getRoot() != TNULL)
		{
			this->deleteNode(this->getRoot()->data);
		}
	}

	// search the tree for the key k
	// and return the corresponding node
	Node<T>* searchTree(int k) {
		return searchTreeHelper(this->root, k);
	}

	// find the node with the minimum key
	Node<T>* minimum(Node<T>* node) {
		while (node->left != TNULL) {
			node = node->left;
		}
		return node;
	}

	// find the node with the maximum key
	Node<T>* maximum(Node<T>* node) {
		while (node->right != TNULL) {
			node = node->right;
		}
		return node;
	}

	// find the successor of a given node
	Node<T>* successor(Node<T>* x) {
		// if the right subtree is not null,
		// the successor is the leftmost node in the
		// right subtree
		if (x->right != TNULL) {
			return minimum(x->right);
		}

		// else it is the lowest ancestor of x whose
		// left child is also an ancestor of x.
		Node<T>* y = x->parent;
		while (y != TNULL && x == y->right) {
			x = y;
			y = y->parent;
		}
		return y;
	}

	// find the predecessor of a given node
	Node<T>* predecessor(Node<T>* x) {
		// if the left subtree is not null,
		// the predecessor is the rightmost node in the 
		// left subtree
		if (x->left != TNULL) {
			return maximum(x->left);
		}

		Node<T>* y = x->parent;
		while (y != TNULL && x == y->left) {
			x = y;
			y = y->parent;
		}

		return y;
	}

	// rotate left at node x
	void leftRotate(Node<T>* x) {
		Node<T>* y = x->right;
		x->right = y->left;
		if (y->left != TNULL) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		}
		else if (x == x->parent->left) {
			x->parent->left = y;
		}
		else {
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}

	// rotate right at node x
	void rightRotate(Node<T>* x) {
		Node<T>* y = x->left;
		x->left = y->right;
		if (y->right != TNULL) {
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		}
		else if (x == x->parent->right) {
			x->parent->right = y;
		}
		else {
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}

	// insert the key to the tree in its appropriate position
	// and fix the tree
	void insert(int key) {
		// Ordinary Binary Search Insertion
		if (!foundKey(key)) {
			Node<T>* node = new Node<T>;
			node->parent = nullptr;
			node->data = key;
			node->left = TNULL;
			node->right = TNULL;
			node->color = 1; // new node must be red
			node->count = 1;
			Node<T>* y = nullptr;
			Node<T>* x = this->root;

			while (x != TNULL) {
				y = x;
				if (node->data < x->data) {
					x = x->left;
				}
				else {
					x = x->right;
				}
			}

			// y is parent of x
			node->parent = y;
			if (y == nullptr) {
				root = node;
			}
			else if (node->data < y->data) {
				y->left = node;
			}
			else {
				y->right = node;
			}

			// if new node is a root node, simply return
			if (node->parent == nullptr) {
				node->color = 0;
				return;
			}

			// if the grandparent is null, simply return
			if (node->parent->parent == nullptr) {
				return;
			}

			// Fix the tree
			fixInsert(node);
		}
		else
		{
			searchTree(key)->count++;
		}
	}

	Node<T>* getRoot() {
		return this->root;
	}

	// delete the node from the tree
	void deleteNode(int data) {
		deleteNodeHelper(this->root, data);
	}

	// print the tree structure on the screen
	void prettyPrint() {
		if (root) {
			printHelper(this->root, "", true);
		}
	}

	void virtPrint(Node<T>* node, int spaces) {
		if (node != TNULL)
		{
			virtPrint(node->right, spaces + 1);
			for (int i = 1; i < spaces; i++)
			{
				cout << "  ";
			}
			char temp;
			(node->color == 0) ? (temp = 'b') : (temp = 'r');
			cout << node->data << temp << endl;
			virtPrint(node->left, spaces + 1);
		}
	}

	bool checkKeyInTree(int key) {
		return this->foundKey(key);
	}
};

template <class T>
class RBTreeIterator
{
private:
	RBTree obj;
	Node<T> data;
public:
	RBTreeIterator(RBTree obj)
	{
		this->obj = obj;
		this->data = nullptr;
	}
	RBTreeIterator(RBTree obj, Node<T> data) : RBTreeIterator(obj)
	{
		this->data = data;
	}
	void operator = (T data)
	{
		this->data = data;
	}
	void operator ++()
	{
		if (data.right->data != obj.TNULL)
		{
			data = data.right;
		}
		else
		{
			while (data.parent != obj.getRoot() && data.parent->data < data.data ) //� ��������� ���� ������������� ���� data, ��� ��� ���������
			{
				data = data.parent;
			}
		}
	}
};

