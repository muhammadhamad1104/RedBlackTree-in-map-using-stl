#include<iostream>
#include<string>
#include<queue>
template<class k, class v>
struct pair {//pair structure which has first which represent key and second represent value
	k first;
	v second;
	pair(k f = k(), v s = v()) {//give first and second as default value
		first = f;
		second = s;
	}
};
template<class k, class v>
struct mnode {//strct a node which contain a pair of key value and the node left right parent and isnill remain false
	pair<k, v> val;
	mnode<k, v>* left, * right, * parent;
	bool is_nill = false;
	std::string color="red";
};
template<class k, class v>
class map {
public://public class iterator
	class iterator;
private://in private of map class we have head node
	mnode<k, v>* Head;
	int n;
	void rotate_left(mnode<k, v>* ptr) {
		mnode<k, v>* temp = ptr->right;
		ptr->right = temp->left;
		if (temp->left != this->Head) {
			temp->left->parent = ptr;
		}
		temp->parent = ptr->parent;
		if (ptr->parent == this->Head) {
			this->Head->parent = temp;
		}
		else if (ptr == ptr->parent->left) {
			ptr->parent->left = temp;
		}
		else {
			ptr->parent->right = temp;
		}
		temp->left = ptr;
		ptr->parent = temp;
	}
	void rotate_right(mnode<k, v>* ptr) {
		mnode<k, v>* temp = ptr->left;
		ptr->left = temp->right;
		if (temp->right != this->Head) {
			temp->right->parent = ptr;
		}
		temp->parent = ptr->parent;
		if (ptr->parent == this->Head) {
			this->Head->parent = temp;
		}
		else if (ptr == ptr->parent->right) {
			ptr->parent->right = temp;
		}
		else {
			ptr->parent->left = temp;
		}
		temp->right = ptr;
		ptr->parent = temp;
	}
	void fix_insertion(mnode<k, v>* ptr) {
		while (ptr->parent!=nullptr&&ptr->parent->color == "red") {
			if (ptr->parent == ptr->parent->parent->left) {
				mnode<k, v>* temp = ptr->parent->parent->right;
				if (temp!=nullptr&&temp->color == "red") {
					ptr->parent->color = "black";
					temp->color = "black";
					ptr->parent->parent->color = "red";
					ptr = ptr->parent->parent;
				}
				else {
					if (ptr == ptr->parent->right) {
						ptr = ptr->parent;
						rotate_left(ptr);
					}
					ptr->parent->color = "black";
					ptr->parent->parent->color = "red";
					rotate_right(ptr->parent->parent);
				}
			}
			else {
				mnode<k, v> *temp = ptr->parent->parent->left;
				if (temp!=nullptr&&temp->color == "red") {
					ptr->parent->color = "black";
					temp->color = "black";
					ptr->parent->parent->color = "red";
					ptr = ptr->parent->parent;
				}
				else {
					if (ptr == ptr->parent->left) {
						ptr = ptr->parent;
						rotate_right(ptr);
					}
					ptr->parent->color = "black";
					ptr->parent->parent->color = "red";
					rotate_left(ptr->parent->parent);
				}
			}
			/*if (ptr == this->Head->parent) {
				break;
			}*/
		}
		this->Head->parent->color = "black";
	}
	pair<iterator, bool> insert_r(mnode<k, v>* ptr, const pair<k, v>& val) {//this function is call in private insert function which return an iterator and bool
		if (ptr == this->Head) {//if the given node point to this node
			mnode<k, v>* temp = new mnode<k, v>;
			temp->val = val;
			temp->parent = nullptr;
			temp->left = temp->right = this->Head;
			++this->n;
			iterator it;
			it.ptr = temp;
			return { it,true };//return the pair
		}
		else if (ptr->val.first == val.first) { //if key of inserted node is equall to root node
			iterator it;
			it.ptr = ptr;
			return { it,false };
		}
		else if (ptr->val.first > val.first) {//if key of inserted node is less than the root node
			if (ptr->left == this->Head) {
				mnode<k,v> *temp=new mnode<k, v>;
				temp->val = val;
				temp->parent = ptr;
				temp->left = temp->right = this->Head;
				ptr->left = temp;
				++this->n;
				fix_insertion(temp);
				iterator it;
				it.ptr = temp;
				return { it,true };
			}
			return insert_r(ptr->left, val);
		}
		else {//if key of inserted node is greator than the root node
			if (ptr->right == this->Head) {
				mnode<k, v>* temp = new mnode<k, v>;
				temp->val = val;
				temp->parent = ptr;
				temp->left = temp->right = this->Head;
				ptr->right = temp;
				++this->n;
				fix_insertion(temp);
				iterator it;
				it.ptr = temp;
				return { it,true };
			}
			return insert_r(ptr->right, val);
		}
	}
	iterator find_r(mnode<k, v>* ptr, k key) {//recursive function of find which return iterator
		if (ptr == this->Head) { //if root node is head then return the iterator which point to head
			iterator it;
			it.ptr = this->Head;
			return it;
		}
		else if (ptr->val.first == key) {//if root node key is equall to inserted key then return iterator which point to that node
			iterator it;
			it.ptr = ptr;
			return it;
		}
		else {
			if (ptr->val.first > key) {//if root node key is greator to inserted key then return recursive function by passinf the left of root node
				return find_r(ptr->left, key);
			}
			else {//if root node key is greator to inserted key then return recursive function by passinf the left of root node
				return find_r(ptr->right, key);
			}
		}
	}
public:
	map() {//default constructor of map to initilize the head to give value of left right and parent to that head node and isnill true
		this->Head = new mnode<k, v>;
		this->Head->left = this->Head->right = this->Head->parent = this->Head;
		this->Head->is_nill = true;
		this->Head->color = "black";
		this->n = 0;
	}
	void level_order_traversal()const {
		if (this->Head->parent == this->Head) {
			throw "map underflow";
		}
		else {
			std::queue<mnode<k, v>*> q;
			q.push(this->Head->parent);
			while (!q.empty()) {
				mnode<k, v>* temp = q.front();
				q.pop();
				if (temp->is_nill) {
					continue;
				}
				std::cout << "{(key, " << temp->val.first << ") : (value, " << temp->val.second << ") : (color, " << temp->color << ")}" << std::endl;
				if (temp->left && !temp->left->is_nill) {
					q.push(temp->left);
				}
				if (temp->right && !temp->right->is_nill) {
					q.push(temp->right);
				}
			}
		}
	}
	class iterator {//iterator class having node ptr and friend class map
	private:
		mnode<k, v>* ptr;
		friend class map<k, v>;
	public:
		iterator& operator++() {//this function is for iterate the iteretor
			if (this->ptr->right->is_nill != true) { //if iterator right node isnill is not true then 
				this->ptr = this->ptr->right;
				while (this->ptr->left->is_nill != true) {
					this->ptr = this->ptr->left;
				}
			}
			else {
				if (this->ptr == this->ptr->parent->left) {// if the node is equall to their parent left node
					this->ptr = this->ptr->parent;
				}
				else {
					while (this->ptr != this->ptr->parent->left && this->ptr->parent->is_nill != true) { // if the node is not equall to their parent left node and their parent is nill is not true
						this->ptr = this->ptr->parent;
					}
					this->ptr = this->ptr->parent;
				}
			}
			return *this;
		}
		iterator& operator++(int) {//same as ++ operator but not return the next node return the first pointer iterator
			iterator temp = *this;
			if (this->ptr->right->is_nill != true) {
				this->ptr = this->ptr->right;
				while (this->ptr->left->is_nil != true) {
					this->ptr = this->ptr->left;
				}
			}
			else {
				if (this->ptr == this->ptr->parent->left) {
					this->ptr = this->ptr->parent;
				}
				else {
					while (this->ptr->parent->is_nil != true && this->ptr != this->ptr->parent->left) {
						this->ptr = this->ptr->parent;
					}
					this->ptr = this->ptr->parent;
				}
			}
			return temp;
		}
		v& operator*()const { //return whole pair value
			return this->ptr->val.second;
		}
		pair<k, v>* operator->()const {
			return &this->ptr->val;
		}
		bool operator==(const iterator& rhs)const {//check if this node is equall to other node
			return this->ptr == rhs.ptr;
		}
		bool operator!=(const iterator& rhs)const {//check if this node is not equall to other node
			return this->ptr != rhs.ptr;
		}
	};
	iterator begin()const {//this will point the iterator to the begin of bst mean smallest node
		/*iterator it;
		it.ptr = this->Head->left;
		return it;*/
		iterator it;
		mnode<k, v>* ptr = this->Head->parent;
		while (ptr->left->is_nill != true) {
			ptr = ptr->left;
		}
		it.ptr = ptr;
		return it;
	}
	iterator end()const {//this point the iterator to head node
		iterator it;
		it.ptr = this->Head;
		return it;
	}
	pair<iterator, bool> insert(const pair<k, v>& val) {//this will insert the whole pair and return the pair of iterator and bool
		if (this->Head->parent == this->Head) {//if head parent point to head
			mnode<k, v>* temp = new mnode<k, v>;
			temp->val = val;
			temp->color = "black";
			temp->left = temp->right = temp->parent = this->Head;
			this->Head->parent = this->Head->left = this->Head->right = temp;
			++this->n;
			iterator it;
			it.ptr = temp;
			return { it,true };
		}
		else {//therwise call the recursive insert function and return the pair
			return insert_r(this->Head->parent, val);
		}
	}
	iterator find(k key) {//find function return the iterator
		if (this->Head->parent == this->Head) {//if head parent point to head
			iterator it;
			it.ptr = this->Head;
			return it;
		}
		else {//otherwise call recursive find function
			return find_r(this->Head->parent, key);
		}
	}
	v& operator[](const k& key) {//the given key is find in whole bst if the key is found then return the value part otherwise insert this key and then return the value part
		iterator it;
		it = find(key);
		if (it.ptr->val.first == key) {
			return it.ptr->val.second;
		}
		else {
			pair<iterator, bool> p;
			p = insert({ key,v() });
			return p.first.ptr->val.second;
		}
	}
	void print() {
		iterator it;
		it = begin();
		while (it != end()) {
			std::cout << "{(key : "<<it->first << "), (value, " << it->second << "), (color, " << it.ptr->color <<")}" << std::endl;
			++it;
		}
	}
};
int main() {
	map<char, int> m;
	pair<char, int> p({ 'm',1 });
	m.insert({ p });
	m.insert({ 'n',1 });
	m.insert({ 'n',2 });
	m.insert({ 'l',1 });
	m.insert({ 'n',1 });
	m.insert({ 'f',1 });
	m.insert({ 'j',1 });
	m.insert({ 'n',1 });
	m.insert({ 'e',1 });
	m.insert({ 's',1 });
	m.insert({ 'g',1 });
	m.insert({ 'c',1 });
	m.insert({ 'k',1 });
	m.insert({ 'b',1 });
	m.insert({ 'y',1 });
	m.insert({ 'v',1 });
	m.insert({ 'x',1 });
	m.insert({ 'j',1 });
	m.insert({ 'i',1 });
	m.insert({ 'a',1 });
	m.insert({ 'z',1 });
	m.insert({ 'd',1 });
	m['w'] = 5;
	std::cout << "=====Printing map<char, int> using print function=====" << std::endl;
	std::cout << std::endl;
	m.print();
	std::cout << std::endl;
	map<int, int> m1;
	m1.insert({ 50,3 });
	m1.insert({ 10,1 });
	m1.insert({ 25,2 });
	m1.insert({ 75,7 });
	m1.insert({ 90,10 });
	m1.insert({ 85,9 });
	m1.insert({ 65,5 });
	m1.insert({ 60,4 });
	m1.insert({ 70,6 });
	m1.insert({ 80,8 });
	m1.insert({ 95,11 });
	map<char, int>::iterator itCHAR;
	map<int, int>::iterator itINT;
	itCHAR = m.find('w');
	itINT = m1.find(70);
	std::cout << "=====Find function=====" << std::endl;
	std::cout << std::endl;
	std::cout << "{(key, w) : (value, " << itCHAR->second <<")}" << std::endl;
	std::cout << "{(key, 70) : (value, " << itINT->second << ")}" << std::endl;
	std::cout << std::endl;
	std::cout << "=====Printing map<int, int> using level order traversal=====" << std::endl;
	std::cout << std::endl;
	m1.level_order_traversal();
}