#include <iostream>
#include <list>
#include <algorithm>
#include <iterator>
#include <queue>

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
	Node(T d, Node* p) : data(d), parent(p){}
	T data;
	std::list<Node*> children;
	Node* parent;
};
//------------------------------------------------------------------
template<typename T>
class TreeLoader{
	struct ThisNode;
	class NodeIterator;
	typedef Node<T> NT;
	typedef std::list<NT*> LNT;
	
	LNT _roots;
	LNT _allNodes;

public:
	typedef typename std::list<NT*>::iterator iterator;
	typedef typename std::list<NT*>::const_iterator const_iterator;
	typedef typename std::list<NT*>::reverse_iterator reverse_iterator;
	typedef typename std::list<NT*>::const_reverse_iterator const_reverse_iterator;

	TreeLoader(){}
	~TreeLoader(){
		_remove(_roots);
	}
	NT* get(int number){
		ThisNode thisNode(number);
		return _get(_roots, thisNode);
	}
	NT* add(T data, NT* parent = NULL){
		NT* n = new NT(data, parent);
		if(parent)
			parent->children.push_back(n);
		else 
			_roots.push_back(n);
		_updateAllNodes();
		return n;
	}
	inline iterator begin(){return _allNodes.begin();}
	inline iterator end(){return _allNodes.end();}

	inline const_iterator cbegin(){return _allNodes.cbegin();}
	inline const_iterator cend(){return _allNodes.cend();}

	inline reverse_iterator rbegin(){return _allNodes.rbegin();}
	inline reverse_iterator rend(){return _allNodes.rend();}

	inline const_reverse_iterator crbegin(){return _allNodes.crbegin();}
	inline const_reverse_iterator crend(){return _allNodes.crend();}

private:
	TreeLoader& operator=(const TreeLoader&);
	TreeLoader(const TreeLoader&);

	void _updateAllNodes(){
		_allNodes.clear();
		__updateAllNodes();
	}
	void __updateAllNodes(){
	    std::queue<NT*> vertex;
	    for(iterator i = _roots.begin(); i != _roots.end(); ++i)
	    	vertex.push(*i);
	    while (!vertex.empty()){
	        NT* current = vertex.front();
	        vertex.pop();
	        _allNodes.push_back(current);
	        for(iterator i = current->children.begin(); i != current->children.end(); ++i)
	        	vertex.push(*i);
	    }		
	}
	void _remove(LNT& nodes){
		for(iterator i = nodes.begin(); i != nodes.end();){
			_remove((*i)->children);
			delete *i;
			i = nodes.erase(i);
		}
	}
	NT* _get(LNT& nodes, const ThisNode& thisNode){	
		iterator it = std::find_if(nodes.begin(), nodes.end(), thisNode);
		if(it == nodes.end()){
			NT* node = NULL;
			for(iterator i = nodes.begin(); i != nodes.end(); ++i){
				node = _get((*i)->children, thisNode);
				if(node != NULL) 
					break;
			}
			return node;
		}
		return *it;
	}
	int _maxDeep(LNT& nodes){
		int res = 0;
		for(iterator i = nodes.begin(); i != nodes.end(); ++i){
			res = std::max(_maxDeep((*i)->children), res);
		}
		return res + 1;
	}
	//..............................................................
	struct ThisNode{
		int _num;
		ThisNode(int num) : _num(num){}
		bool operator()(NT* node)const{return (node->data.num) == _num;}
	};
};
//------------------------------------------------------------------
int main(int argc, char const *argv[]){
	TreeLoader<int> lT;
	
	Node<int>*n = lT.add(1);
	Node<int>*n2 = lT.add(11, n);
	Node<int>*n3 = lT.add(111, n2);
	lT.add(1111, n3);
	lT.add(112, n2);
	lT.add(12, n);
	n2 = lT.add(13, n);
	lT.add(131, n2);
	lT.add(14, n);

	n = lT.add(2);
	n2 = lT.add(21, n);
	n3 = lT.add(211, n2);
	lT.add(2111, n3);
	lT.add(212, n2);
	lT.add(22, n);
	n2 = lT.add(13, n);
	lT.add(231, n2);
	lT.add(24, n);

	for(TreeLoader<int>::iterator i = lT.begin(); i != lT.end(); ++i)
		std::cout << (*i)->data << " ";
	std::cout << std::endl;
	return 0;
}