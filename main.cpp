#include <iostream>
#include <list>
#include <algorithm>
#include <iterator>
#include <queue>

#include <iomanip>
#include <sstream>

#define _SIZE_WIDTH (12)

enum TypeNode{
	OLD_ARM,
	AVR, 
	ARM
};
//------------------------------------------------------------------
struct DataDevice{
	DataDevice(int n) : num(n), type(ARM){} 
	int num;
	bool isOn;
	bool isTransmit;
	TypeNode type;
	friend std::ostream& operator<<(std::ostream& os, const DataDevice& data){
		switch(data.type){
			case OLD_ARM:
				os << "old arm";
				break;
			case ARM:
				os << "arm";
				break;
			case AVR:
				os << "avr";
				break;
		}
		os << "_" << data.num;
		return os;
	}
	friend bool operator==(const DataDevice& d1, const DataDevice& d2){
		return (d1.num) == (d2.num);
	}
};
template<typename T>
class loaderTree{
	//------------------------------------------------------------------
	struct Node{
		Node(T d) : data(d){}
		T data;
		std::list<Node*> children;
		std::list<Node*> parents;
	};
//------------------------------------------------------------------
	typedef Node NT;
	typedef std::list<NT*> LNT;
	typedef typename LNT::iterator IT;
	typedef typename LNT::const_iterator CIT;
	typedef typename LNT::reverse_iterator RIT;
	typedef typename LNT::const_reverse_iterator CRIT;
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
	void printPostorder(std::stringstream& out)const{
		for(CIT i = _roots.begin(); i != _roots.end(); ++i){
			out << "\n||";
			_printPostorder(out, *i);
		}
		out << "\n";
	}
	inline std::size_t size(){return _allNodes.size();}
	inline bool empty(){return _allNodes.empty();}
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

	inline reverse_iterator v_rbegin(const T& t){return reverse_iterator(this->v_end(t));}
	inline reverse_iterator v_rend(const T& t){return reverse_iterator(this->v_begin(t));}

	//iterator find(const T& t){return iterator(_allNodes.find(this->_find(t)));}

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
			if((*i)->data == t)
				return *i;
			NT* node = __find((*i)->children, t);
			if(node) 
	 			return node;
		}
		return NULL;
	}
	void _printPostorder(std::stringstream& out, const NT* p)const{
		std::stringstream ss_l, ss_l2;
		ss_l << ">" << p->data << (p->children.size() > 1 ? "_ " : "");
		ss_l2 << std::setfill('-') << std::setw(_SIZE_WIDTH) << ss_l.str();
		out << ss_l2.str() ;
		for(CIT it = p->children.begin(); it != p->children.end(); ++it){
			if(p->children.size() > 1)
				_setSpStr(out, *it);
			
			_printPostorder(out, *it);
		}
		if(p->children.size() > 1)
			_setSpStr(out, p);
	}
	void _setSpStr(std::stringstream& out, const NT* p)const{
		out << "\n||";
		for(CRIT it = p->parents.rbegin(); it != p->parents.rend(); ++it)
				out << std::setw(_SIZE_WIDTH) << ((*it)->children.size() > 1  ? "|" : "");
	}
};
//------------------------------------------------------------------
int main(int argc, char const *argv[]){
	loaderTree<DataDevice> lT;
	DataDevice n = 1, n2 = 11, n3 = 111; 

	lT.add(n);
	lT.add(n2, n);
	lT.add(n3, n2);
	lT.add(1111, n3);
	lT.add(11111, 1111);
	lT.add(11112, 1111);
	lT.add(111111, 11111);
	lT.add(112, n2);
	lT.add(113, n2);
	lT.add(12, n);
	lT.add(121, 12);
	n2  = 13;
	lT.add(n2, n);
	lT.add(131, n2);
	lT.add(132, n2);
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
	for(loaderTree<DataDevice>::iterator i = lT.h_begin(); i != lT.h_end(); ++i)
		std::cout << (*i) << " ";
	std::cout << std::endl;

	for(loaderTree<DataDevice>::iterator i = std::find(lT.h_begin(), lT.h_end(), 111); i != lT.h_end(); ++i)
		std::cout << (*i) << " ";
	std::cout << std::endl;

	// for(loaderTree<int>::iterator i = lT.h_begin(); i != lT.h_end(); ++i)
	// 	++(*i); 
	// //2 3 12 13 14 15 22 23 14 25 112 113 132 212 213 232 1112 2112 
	// for(loaderTree<int>::iterator i = lT.h_begin(); i != lT.h_end(); ++i)
	// 	std::cout << (*i) << " ";
	// std::cout << std::endl;

	for(loaderTree<DataDevice>::iterator ih = lT.h_begin(); ih != lT.h_end(); ++ih){
		std::cout << "child: " << std::setw(4) << (*ih) << ", up_way: ";
		for(loaderTree<DataDevice>::iterator iv = lT.v_begin(*ih); iv != lT.v_end(*ih); ++iv)
			std::cout << (*iv) << " ";
		std::cout << std::endl;
	}
	std::stringstream ss;
	lT.printPostorder(ss);
	std::cout << ss.str(); 

	return 0;
}