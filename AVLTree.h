#ifndef _AVLTREE_H_
#define _AVLTREE_H_

#include "VaccinationData.h"
#include "AVLNode.h"
/* class AVL */

class AVLTree{
private:
	AVLNode*	root;

public:
	
	AVLTree(){
		root=NULL;
	}
	~AVLTree(){
		DeleteTree();
	};
	bool		Insert(VaccinationData* pVac);
	VaccinationData*	Search(string name);
	void GetVector(vector<VaccinationData*>& v);
	int compare_string(string a, string b);
	void DeleteTree(void);

};

#endif

