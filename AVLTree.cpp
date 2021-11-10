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
    else if(a == pa->getLeft())     //left rotate
    {
        pa->setLeft(rootsub);
    }
    else                            //right rotate
    {
        pa->setRight(rootsub);
    }

    return true;
}

//Give data to vector
void AVLTree::GetVector(vector<VaccinationData*>& v){
    if(root == NULL)        //if root is not exist, end
    {
        return;
    }
    
    queue<AVLNode*> levelQ;     //level order

    levelQ.push(root);          //push root
    v.clear();                  //vector clear

    while(!(levelQ.empty()))    //while queue is not empty
    {
       v.push_back(levelQ.front()->getVacData());   //insert front data at vector
       if (levelQ.front()->getLeft() != NULL)       //if data->left is exist, push it
       {
           levelQ.push(levelQ.front()->getLeft());
       }
       if (levelQ.front()->getRight() != NULL)       //if data->right is exist, push it
       {
           levelQ.push(levelQ.front()->getRight());
       }
       levelQ.pop();        //pop queue
    }

}

//Search name at AVLTree
VaccinationData* AVLTree::Search(string name){
    if(root == NULL)        //if data is not exist
    {
        return NULL;
    }

    AVLNode* pNode = root;
    while(pNode)        //while pNode is not leaf
    {
        if(compare_string(name, pNode->getVacData()->GetUserName()) == 0)       //if find data, return it
        {
            return pNode->getVacData();
        }
        else if(compare_string(name, pNode->getVacData()->GetUserName()) < 0)       //if data is in leftside, move pNode
        {
            pNode = pNode->getLeft();
        }
        else                                                                    //if data is in rightside, move pNode
        {
            pNode = pNode->getRight();
        }
    }

    return NULL;
}

//Compare two string(not influenced by Upper or Lower case)
int AVLTree::compare_string(string a, string b){
    
    for(int i = 0;;i++)
    {
        char pa = a[i];
        char pb = b[i];

        if(pa == '\0' && pb != '\0')    //when b is longer than a, return 1
        {
            return -1;
        }
        else if(pa != '\0' && pb == '\0')       //when a is longer than b, return 1
        {
            return 1;
        }
        else if(pa == '\0' && pb == '\0')       //if two string is same size, return 0
        {
            return 0;
        }

        if(islower(pa))                         //if pa is lower, make upper
        {
            pa = toupper(pa);
        }
        
        if(islower(pb))                         //if pb is lower, make upper
        {
            pb = toupper(pb);
        }

        if(pa < pb)                             //if a < b, return -1
        {
            return -1;
        }
        else if(pa > pb)                        //if a > b, return 1
        {
            return 1;
        }
    }
}

//Delete AVLTree
void AVLTree::DeleteTree(void){
    if(root == NULL)        //if data is not exist
    {
        return;
    }
    
    queue<AVLNode*> levelQ;     //delete by level order

    levelQ.push(root);          //push root at queue

    while(!(levelQ.empty()))    //while queue is not empty
    {
       
       if (levelQ.front()->getLeft() != NULL)       //if front's leftside is not empty, push it
       {
           levelQ.push(levelQ.front()->getLeft());
       }
       
       if (levelQ.front()->getRight() != NULL)      //if front's rightside is no empty, push it
       {
           levelQ.push(levelQ.front()->getRight());
       }
       
       delete levelQ.front();                       //delete front and pop it
       levelQ.pop();
    }

    return;
}