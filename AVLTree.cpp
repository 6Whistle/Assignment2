#include "AVLTree.h"
bool AVLTree::Insert(VaccinationData* pVac){
    if(pVac->GetTimes() != 1 && pVac->GetVaccineName().compare("Janssen") == 0)
    {
        return false;
    }
    if(pVac->GetTimes() != 2)
    {
        return false;
    }
    
    if(root == NULL)
    {
        root = new AVLNode;
        root->setVacData(pVac);

        return true;
    }

    AVLNode *a = root, *pa = NULL, *p= root, *pp = NULL, *rootsub = NULL;

    while(p != NULL)
    {
        if(p->getBF() != 0)
        {
            a = p;
            pa = pp;
        }

        if(compare_string(pVac->GetUserName(), p->getVacData()->GetUserName()) < 0)
        {
            pp = p;
            p = p->getLeft();
        }
        else if(compare_string(pVac->GetUserName(), p->getVacData()->GetUserName()) > 0)
        {
            pp = p;
            p = p->getRight();
        }
        else
        {
            delete pVac;
            return false;
        }
    }

    AVLNode* newNode = new AVLNode;
    newNode->setVacData(pVac);

    if(compare_string(pVac->GetUserName(), pp->getVacData()->GetUserName()) < 0)
    {
        pp->setLeft(newNode);
    }
    else
    {
        pp->setRight(newNode);
    }

    int d;
    AVLNode *b, *c;

    if(compare_string(pVac->GetUserName(), a->getVacData()->GetUserName()) > 0)
    {
        b = a->getRight();
        p = b;
        d = -1;
    }
    else
    {
        b = a->getLeft();
        p = b;
        d = 1;
    }

    while(p != newNode)
    {
        if(compare_string(pVac->GetUserName(), p->getVacData()->GetUserName()) > 0)
        {
            p->setBF(-1);
            p = p->getRight();
        }
        else
        {
            p->setBF(1);
            p = p->getLeft();
        }
    }

    if(a->getBF() == 0 || a->getBF() + d == 0)
    {
        a->setBF(a->getBF() + d);
        return true;
    }

    if(d == 1)
    {
        if(b->getBF() == 1)
        {
            a->setLeft(b->getRight());
            b->setRight(a);
            a->setBF(0);
            b->setBF(0);
            rootsub = b;
        }
        else
        {
            c = b->getRight();
            b->setRight(c->getLeft());
            a->setLeft(c->getRight());
            c->setLeft(b);
            c->setRight(a);

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
            rootsub = c;
        }
    }
    else
    {
        if(b->getBF() == -1)
        {
            a->setRight(b->getLeft());
            b->setLeft(a);
            a->setBF(0);
            b->setBF(0);
            rootsub = b;
        }
        else
        {
            c = b->getLeft();
            b->setLeft(c->getRight());
            a->setRight(c->getLeft());
            c->setRight(b);
            c->setLeft(a);

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
            rootsub = c;
        }
    }
    if(pa == NULL)
    {
        root = rootsub;
    }
    else if(a == pa->getLeft())
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
            pa = toupper(pb);
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