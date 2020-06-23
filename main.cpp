#include <iostream>
#include <list>
#include <algorithm>

#define _SIZE_WIDTH 	(7)
#define _SIZE_HEIGHT	(5)
#define _SIZE_WSTEP	(1)
#define _SIZE_HSTEP	(1)

enum TypeNode{
	OLD,
	AVR, 
	ARM
};
struct DrawParams{
	int leftSize, rightSize;
};
struct Node{
	int num;
	bool isOn;
	bool isTransmit;
	TypeNode type;
	std::list<Node*> children;
	std::list<int>	wayTo;
	DrawParams drawParams;
};
class deviceTreeLoader{
	struct ThisNode{
		int _num;
		ThisNode(int num) : _num(num){}
		bool operator()(Node* node)const{return (node->num) == _num;}
	};
	std::list<Node*> _roots;

public:
	deviceTreeLoader(){}
	~deviceTreeLoader(){
		_remove(_roots);
	}
	Node* getNode(int number){
		ThisNode thisNode(number);
		return _getNode(_roots, thisNode);
	}
	void addChild(Node* child, Node* parent = NULL){
		if(parent){
			child->wayTo = parent->wayTo;
			child->wayTo.push_back(parent->num);
		}
		else 
			_roots.push_back(child);
	}
private:
	deviceTreeLoader& operator=(const deviceTreeLoader&);
	deviceTreeLoader(const deviceTreeLoader&);

	void _remove(std::list<Node*>& nodes){
		for(std::list<Node*>::iterator i = nodes.begin(); i != nodes.end();){
			_remove((*i)->children);
			delete *i;
			i = nodes.erase(i);
		}
	}
	Node* _getNode(std::list<Node*>& nodes, const ThisNode& thisNode){	
		std::list<Node*>::iterator it = std::find_if(nodes.begin(), nodes.end(), thisNode);
		if(it == nodes.end()){
			Node* node = NULL;
			for(std::list<Node*>::iterator i = nodes.begin(); i != nodes.end(); ++i){
				node = _getNode((*i)->children, thisNode);
				if(node != NULL) 
					break;
			}
			return node;
		}
		return *it;
	}
	int _maxDeep(std::list<Node*>& nodes){
		int res = 0;
		for(std::list<Node*>::iterator i = nodes.begin(); i != nodes.end(); ++i){
			res = std::max(_maxDeep((*i)->children), res);
		}
		return res + 1;
	}
	void _setRListAll(){
		
	}
};

int main(int argc, char const *argv[]){
	/* code */
	return 0;
}