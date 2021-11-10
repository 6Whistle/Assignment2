#include "AVLTree.h"
//Insert data at AVLTree
bool AVLTree::Insert(VaccinationData* pVac){
    if(root == NULL)        //if AVLTree has no data, insert at root
    {
        root = new AVLNode;
        root->setVacData(pVac);

        return true;
    }

    //a = most recent node with +-1, pa = parent of a
    //pp is parent of p
    AVLNode *a = root, *pa = NULL, *p= root, *pp = NULL, *rootsub = NULL;
    

    while(p != NULL)        //while p is not leaf
    {
        if(p->getBF() != 0)     //set a and pa if bf is +-1
        {
            a = p;
            pa = pp;
        }

        if(compare_string(pVac->GetUserName(), p->getVacData()->GetUserName()) < 0)     //move to left
        {
            pp = p;
            p = p->getLeft();
        }
        else if(compare_string(pVac->GetUserName(), p->getVacData()->GetUserName()) > 0)        //move to right
        {
            pp = p;
            p = p->getRight();
        }
        else        //if data is same
        {
            delete pVac;
            return false;
        }
    }

    AVLNode* newNode = new AVLNode;         //make new Node and set data
    newNode->setVacData(pVac);

    if(compare_string(pVac->GetUserName(), pp->getVacData()->GetUserName()) < 0)    //insert at left
    {
        pp->setLeft(newNode);
    }
    else                //insert at right
    {
        pp->setRight(newNode);
    }

    int d;          //if -1, newNode is inserted at left of a. else newNode is iserted at right of a 
    AVLNode *b, *c;     //child of a and b

    // set d, b, c
    if(compare_string(pVac->GetUserName(), a->getVacData()->GetUserName()) > 0) //if data is inserted at a->right
    {
        b = a->getRight();
        p = b;
        d = -1;
    }
    else                //if data is inserted at a->left
    {
        b = a->getLeft();
        p = b;
        d = 1;
    }

    while(p != newNode)//set p's bf
    {
        if(compare_string(pVac->GetUserName(), p->getVacData()->GetUserName()) > 0)     //if data is inserted at p->right
        {
            p->setBF(-1);
            p = p->getRight();
        }
        else                 //if data is inserted at p->left
        {
            p->setBF(1);
            p = p->getLeft();
        }
    }

    if(a->getBF() == 0 || a->getBF() + d == 0)      //if tree is balanced, set a's bf and end
    {
        a->setBF(a->getBF() + d);
        return true;
    }

    if(d == 1)      //if left is not balanced
    {
        if(b->getBF() == 1)     //if LL type, rotation
        {
            a->setLeft(b->getRight());
            b->setRight(a);
            a->setBF(0);
            b->setBF(0);
            rootsub = b;        //top of rotate
        }
        else        //if LR type, rotation twice
        {
            //rotation
            c = b->getRight();
            b->setRight(c->getLeft());
            a->setLeft(c->getRight());
            c->setLeft(b);
            c->setRight(a);

            //set a b c's bf
            if(c->getBF() == 0)
            {
                b->setBF(0);
                a->setBF(0);
            }
            else if(c->getBF() == 1)
            {
                a->setBF(-1);
                b->setBF(0);
            }
            else if(c->getBF() == -1)
            {
                b->setBF(1);
                a->setBF(0);
            }

            c->setBF(0);
            rootsub = c;    //top of rotata
        }
    }
    else        //if right is not balanced
    {
        if(b->getBF() == -1)        //if RR type, rotation
        {
            a->setRight(b->getLeft());
            b->setLeft(a);
            a->setBF(0);
            b->setBF(0);
            rootsub = b;        //top of rotate
        }
        else        //if RL type, rotation twice
        {
            //rotation
            c = b->getLeft();
            b->setLeft(c->getRight());
            a->setRight(c->getLeft());
            c->setRight(b);
            c->setLeft(a);

            //set a b c's bf
            if(c->getBF() == 0)
            {
                b->setBF(0);
                a->setBF(0);
            }
            else if(c->getBF() == 1)
            {
                b->setBF(-1);
                a->setBF(0);
            }
            else if(c->getBF() == -1)
            {
                a->setBF(1);
                b->setBF(0);
            }

            c->setBF(0);
            rootsub = c;    //top of rotate
        }
    }
    if(pa == NULL)      //root change
    {
        root = rootsub;
    }
    else if(a == pa->getLeft())     //a
    {
        pa->setLeft(rootsub);
    }
    else
    {
        pa->setRight(rootsub);
    }

    return true;
}

void AVLTree::GetVector(vector<VaccinationData*>& v){
    if(root == NULL)
    {
        return;
    }
    
    queue<AVLNode*> levelQ;

    levelQ.push(root);
    v.clear();

    while(!(levelQ.empty()))
    {
       v.push_back(levelQ.front()->getVacData());
       if (levelQ.front()->getLeft() != NULL)
       {
           levelQ.push(levelQ.front()->getLeft());
       }
       if (levelQ.front()->getRight() != NULL)
       {
           levelQ.push(levelQ.front()->getRight());
       }
       levelQ.pop();
    }

}

VaccinationData* AVLTree::Search(string name){
    if(root == NULL)
    {
        return NULL;
    }

    AVLNode* pNode = root;
    while(pNode)
    {
        if(compare_string(name, pNode->getVacData()->GetUserName()) == 0)
        {
            return pNode->getVacData();
        }
        else if(compare_string(name, pNode->getVacData()->GetUserName()) < 0)
        {
            pNode = pNode->getLeft();
        }
        else
        {
            pNode = pNode->getRight();
        }
    }

    return NULL;
}

int AVLTree::compare_string(string a, string b){
    
    for(int i = 0;;i++)
    {
        char pa = a[i];
        char pb = b[i];

        if(pa == '\0' && pb != '\0')
        {
            return -1;
        }
        else if(pa != '\0' && pb == '\0')
        {
            return 1;
        }
        else if(pa == '\0' && pb == '\0')
        {
            return 0;
        }

        if(islower(pa))
        {
            pa = toupper(pa);
        }
        
        if(islower(pb))
        {
            pb = toupper(pb);
        }

        if(pa < pb)
        {
            return -1;
        }
        else if(pa > pb)
        {
            return 1;
        }
    }
}

void AVLTree::DeleteTree(void){
    if(root == NULL)
    {
        return;
    }
    
    queue<AVLNode*> levelQ;

    levelQ.push(root);

    while(!(levelQ.empty()))
    {
       
       if (levelQ.front()->getLeft() != NULL)
       {
           levelQ.push(levelQ.front()->getLeft());
       }
       
       if (levelQ.front()->getRight() != NULL)
       {
           levelQ.push(levelQ.front()->getRight());
       }
       
       delete levelQ.front();
       levelQ.pop();
    }

    return;
}