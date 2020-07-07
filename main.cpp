#include <iostream>
#include <list>
#include <algorithm>
#include <iterator>
#include <queue>
#include <functional>
#include <iomanip>

#define _SIZE_WIDTH 	(7)
#define _SIZE_HEIGHT	(5)
#define _SIZE_WSTEP		(1)
#define _SIZE_HSTEP		(1)

enum TypeNode{
	OLD,
	AVR, 
	ARM
};
//------------------------------------------------------------------
struct DataDevice{
	int num;
	bool isOn;
	bool isTransmit;
	TypeNode type;
};
//------------------------------------------------------------------
template<typename T>
struct Node{
	Node(T d) : data(d){}
	T data;
	std::list<Node*> children;
	std::list<Node*> parents;
};
//------------------------------------------------------------------
template<typename T, class Equal = std::equal_to<T> >
class loaderTree{
	struct ThisNode;
	class NodeIterator;
	typedef Node<T> NT;
	typedef std::list<NT*> LNT;
	typedef typename LNT::iterator IT;
	typedef typename LNT::const_iterator CIT;
	Equal _equal;
	LNT _roots;
	LNT _allNodes;

public:
	loaderTree(){}
	~loaderTree(){
		_remove(_roots);
	}
	bool add(const T& t, const T& p){
		NT* node = _find(t);
		NT* parent = _find(p);
		if(!node && parent){
			node = new NT(t);
			node->parents = parent->parents;
			node->parents.push_front(parent);
			parent->children.push_back(node);
			_updateAllNodes();
			return true;
		}
		return false;
	}
	bool add(const T& t){
		NT* node = _find(t);
		if(!node){
			node = new NT(t);
			_roots.push_back(node);
			_updateAllNodes();
			return true;
		}
		return false;
	}
	//..............................................................
	class iterator : public std::iterator<std::bidirectional_iterator_tag, T>{
		IT _hIt;
	public:
		iterator(IT hIt) : _hIt(hIt){}
		iterator& operator++(){
            ++_hIt;
            return *this;
        }
		iterator operator++(int){
            iterator tmp(*this);
            ++_hIt;
            return tmp;
        }
        iterator& operator--(){
            --_hIt;
            return *this;
        }
        iterator operator--(int){
            iterator tmp(*this);
            --_hIt;
            return tmp;
        }
        bool operator==(const iterator& other)const{
            return _hIt == other._hIt;
        }
        bool operator!=(const iterator& other)const{
            return !(*this == other);
        }
        T* operator->(){return &((*_hIt)->data);}
        T& operator*(){return (*_hIt)->data;}
	};
	typedef std::reverse_iterator<iterator> reverse_iterator;
	//..............................................................
	inline iterator h_begin(){return iterator(_allNodes.begin());}
	inline iterator h_end(){return iterator(_allNodes.end());}

	inline reverse_iterator h_rbegin(){return reverse_iterator(this->h_end());}
	inline reverse_iterator h_rend(){return reverse_iterator(this->h_begin());}

	iterator v_begin(const T& t){
		NT* node = _find(t);
		if(node)
			return iterator(node->parents.begin());
		return this->h_end();
	}
	iterator v_end(const T& t){
		NT* node = _find(t);
		if(node)
			return iterator(node->parents.end());
		return this->h_end();
	}

	inline reverse_iterator v_rbegin(){return reverse_iterator(this->v_end());}
	inline reverse_iterator v_rend(){return reverse_iterator(this->v_begin());}

private:
	loaderTree& operator=(const loaderTree&);
	loaderTree(const loaderTree&);

	void _updateAllNodes(){
		_allNodes.clear();
		__updateAllNodes();
	}
	void __updateAllNodes(){
	    std::queue<NT*> vertex;
	    for(IT i = _roots.begin(); i != _roots.end(); ++i)
	    	vertex.push(*i);
	    while (!vertex.empty()){
	        NT* current = vertex.front();
	        vertex.pop();
	        _allNodes.push_back(current);
	        for(IT i = current->children.begin(); i != current->children.end(); ++i)
	        	vertex.push(*i);
	    }		
	}
	void _remove(LNT& nodes){
		for(IT i = nodes.begin(); i != nodes.end();){
			_remove((*i)->children);
			delete *i;
			i = nodes.erase(i);
		}
	}
	NT* _find(const T& t)const{
		return __find(_roots, t);
	}
	NT* __find(const LNT& nodes, const T& t)const{
		for(CIT i = nodes.begin(); i != nodes.end(); ++i){
			if(_equal((*i)->data, t))
				return *i;
			NT* node = __find((*i)->children, t);
			if(node) 
	 			return node;
		}
		return NULL;
	}
	int _maxDeep(LNT& nodes){
		int res = 0;
		for(IT i = nodes.begin(); i != nodes.end(); ++i){
			res = std::max(_maxDeep((*i)->children), res);
		}
		return res + 1;
	}
};
//------------------------------------------------------------------
	// struct ThisNode{
	// 	int _num;
	// 	ThisNode(int num) : _num(num){}
	// 	bool operator()(NT* node)const{return (node->data.num) == _num;}
	// };
//------------------------------------------------------------------
int main(int argc, char const *argv[]){
	loaderTree<int> lT;
	int n = 1, n2 = 11, n3 = 111; 

	lT.add(n);
	lT.add(n2, n);
	lT.add(n3, n2);
	lT.add(1111, n3);
	lT.add(112, n2);
	lT.add(12, n);
	n2  = 13;
	lT.add(n2, n);
	lT.add(131, n2);
	lT.add(14, n);

	n = 2, n2 = 21, n3 = 211; 
	lT.add(n);
	lT.add(n2, n);
	lT.add(n3, n2);
	lT.add(2111, n3);
	lT.add(212, n2);
	lT.add(22, n);
	n2 = 23;
	lT.add(n2, n);
	lT.add(231, n2);
	lT.add(24, n);

	//1 2 11 12 13 14 21 22 13 24 111 112 131 211 212 231 1111 2111
	for(loaderTree<int>::iterator i = lT.h_begin(); i != lT.h_end(); ++i)
		std::cout << (*i) << " ";
	std::cout << std::endl;

	// for(loaderTree<int>::iterator i = lT.h_begin(); i != lT.h_end(); ++i)
	// 	++(*i); 
	// //2 3 12 13 14 15 22 23 14 25 112 113 132 212 213 232 1112 2112 
	// for(loaderTree<int>::iterator i = lT.h_begin(); i != lT.h_end(); ++i)
	// 	std::cout << (*i) << " ";
	// std::cout << std::endl;

	for(loaderTree<int>::iterator ih = lT.h_begin(); ih != lT.h_end(); ++ih){
		std::cout << "child: " << std::setw(4) << (*ih) << ", up_way: ";
		for(loaderTree<int>::iterator iv = lT.v_begin(*ih); iv != lT.v_end(*ih); ++iv)
			std::cout << (*iv) << " ";
		std::cout << std::endl;
	}
	return 0;
}