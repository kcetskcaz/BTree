#pragma once
template<class Type>
class Node {
template<class Type>
friend class BTree; //BTree is friend
private:
	Type** keys; //Array of keys
	Node<Type>** children; //Array of children
	Node<Type>* parent; //Parent of node
	int order; //Order
	int length; //Current length
	bool leaf; //Leaf flag
	bool root; //Root flag
public:

	Node();

	Node(Type* key, int inOrder);

	Node(const Node<Type>& in);

	~Node();

	void insert(Type* key);

	Type* remove(Type* key);

	Type* find(Type* key);
	/*---------------------------------------------------------------
	Probably do not need these setters and getters since BTree is a
	friend class
	---------------------------------------------------------------*/
	
	void setLeaf(bool in) { leaf = in; }

	void setRoot(bool in) { root = in; }

	void setParent(Node<Type>* inPar) { parent = inPar; }

	bool isRoot() { return root; }

	bool isLeaf() { return leaf; }

	Node<Type>* getParent(Node<Type>* inPar) { return parent; }
	//------------------------------------------------------------//

	//bool operator==(const Node<Type>& in) {return keys == in.data;}

	//bool operator>(const Node<Type>& in) {return data > in.data;}

	//bool operator<(const Node<Type>& in) {return data < in.data;}

};

template<class Type>
Node<Type>::Node() {
	keys = new Type*[3]();
	children = new Node<Type>*[4];
	for (int i = 0; i < 4; i++) {
		children[i] = nullptr;
	}
	int length = 0;
	int order = 3;
	leaf = false;
	root = false;
}

template<class Type>
Node<Type>::Node(Type* key, int inOrder) {
	order = inOrder;
	length = 1;
	parent = nullptr;
	keys = new Type*[order]();
	children = new Node<Type>*[order+1];
	for (int i = 0; i < order+1; i++) {
		children[i] = nullptr;
	}
	keys[0] = key;
	root = false;
	leaf = false;
}

template<class Type>
Node<Type>::Node(const Node<Type>& in) {
	order = in.order;
	length = in.length;
	keys = in.keys;
	children = in.children;
}

template<class Type>
Node<Type>::~Node() {
	for (int i = 0; i < length; i++) {
		delete keys[i];
	}
	delete[] keys;
 }

template<class Type>
void Node<Type>::insert(Type* key) {
	int i = 0;
	while (keys[i] && *keys[i] < *key && i < length) {
		i++;
	}
	if (!keys[i]) {
		keys[i] = key;
		length++;
	}
	else {
		for (int k = order - 1; k > i; k--) {
			keys[k] = keys[k - 1];
		}
		keys[i] = key;
		length++;
	}
}

template<class Type>
Type* Node<Type>::remove(Type* key) {
	int iterator = 0;
	Type * retval = nullptr; //initialize return value as nullptr
	while (iterator < order - 1) { //find the location of the object
		if (keys[iterator] && *key == *keys[iterator]) {
			break;
		}
		iterator++;
	}
	retval = keys[iterator];
	for (int i = iterator; i < order - 1; i++) {
		keys[i] = keys[i + 1];
	}
	length--;
	return retval;
}

template<class Type>
Type* Node<Type>::find(Type* key) {
	int iterator = 0;
	while (iterator < order-1) { //find the location of the object
		if (keys[iterator] && *key == *keys[iterator]) {
			break;
		}
		iterator++;
	}
	if (iterator == order - 1 && *keys[iterator] != *key) {
		return nullptr;
	}
	return keys[iterator];
}