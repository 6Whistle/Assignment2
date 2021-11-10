#include "BpTree.h"
bool BpTree::Insert(VaccinationData* newData){
    if(root == NULL)
    {
        BpTreeDataNode *DataNode = new BpTreeDataNode;

        DataNode->insertDataMap(newData->GetUserName(), newData);
        root = DataNode;
        return true;
    }

    BpTreeNode *pNode = root;
    while(pNode->getMostLeftChild())
    {
        auto iter = pNode->getIndexMap()->end();
        iter--;
        while(true)
        {
            if((iter->first).compare(newData->GetUserName()) <= 0)
            {
                pNode = iter->second;   
                break;
            }
            else if(iter == pNode->getIndexMap()->begin())
            {
                pNode = pNode->getMostLeftChild();
                break; 
            }
            else
            {
                iter--;
            }
        }
    }

    for(auto iter = pNode->getDataMap()->begin() ; iter != pNode->getDataMap()->end() ; iter++)                         //if find same name
    {
        if(iter->first.compare(newData->GetUserName()) == 0)
        {
            if(!(iter->second->GetVaccineName().compare("Janssen") == 0 && iter->second->GetTimes() == 1) && !(iter->second->GetTimes() == 2))
            {
                iter->second->SetTimesInc();
                delete newData;
                return true;
            }

            return false;
        }
    }
    
    pNode->insertDataMap(newData->GetUserName(), newData);          //no same data

    if(exceedDataNode(pNode))
    {
        if(pNode->getParent() == NULL)
        {
            BpTreeIndexNode* newNode = new BpTreeIndexNode;
            pNode->setParent(newNode);
            newNode->setMostLeftChild(pNode);
            root = newNode;
            splitDataNode(pNode);

            return true;
        }
        splitDataNode(pNode);
        
        
        while(true)
        {
            pNode = pNode->getParent();

            if(exceedIndexNode(pNode))
            {
                if(pNode->getParent() == NULL)
                {
                    BpTreeIndexNode* newNode = new BpTreeIndexNode;
                    pNode->setParent(newNode);
                    newNode->setMostLeftChild(pNode);
                    root = newNode;
                    splitIndexNode(pNode);

                    return true;
                }
                
                splitIndexNode(pNode);
            }
            else
            {
                return true;
            }
        }
    }

    return true;
}

BpTreeNode * BpTree::searchDataNode(string n) {
    if(root == NULL)
    {
        return NULL;
    }
    
    BpTreeNode *pNode = root;

    while(pNode->getMostLeftChild())
    {
        auto iter = pNode->getIndexMap()->end();
        iter--;
        while(true)
        {
            if((iter->first).compare(n) <= 0)
            {
                pNode = iter->second;   
                break;
            }
            else if(iter == pNode->getIndexMap()->begin())
            {
                pNode = pNode->getMostLeftChild();
                break; 
            }
            else
            {
                iter--;
            }
        }
    }

    return pNode;
}
void BpTree::splitDataNode(BpTreeNode* pDataNode) {
    BpTreeDataNode* newDataNode = new BpTreeDataNode;

    auto iter = pDataNode->getDataMap()->begin();
    iter++;

    newDataNode->insertDataMap(iter->first, iter->second);
    iter = pDataNode->getDataMap()->erase(iter);
    newDataNode->insertDataMap(iter->first, iter->second);
    pDataNode->getDataMap()->erase(iter);

    newDataNode->setNext(pDataNode->getNext());
    if (pDataNode->getNext() != NULL)
    {
        pDataNode->getNext()->setPrev(newDataNode);
    }
    pDataNode->setNext(newDataNode);
    newDataNode->setPrev(pDataNode);

    newDataNode->setParent(pDataNode->getParent());

    newDataNode->getParent()->insertIndexMap(newDataNode->getDataMap()->begin()->first, newDataNode);


    return;
}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode) {
    BpTreeIndexNode *newIndexchildNode = new BpTreeIndexNode;
    
    auto iter = pIndexNode->getIndexMap()->begin();
    iter++;

    string insert_string = iter->first;
    newIndexchildNode->setMostLeftChild(iter->second);
    iter = pIndexNode->getIndexMap()->erase(iter);
    newIndexchildNode->insertIndexMap(iter->first, iter->second);
    pIndexNode->getIndexMap()->erase(iter);
    
    newIndexchildNode->getMostLeftChild()->setParent(newIndexchildNode);
    newIndexchildNode->getIndexMap()->begin()->second->setParent(newIndexchildNode);

    BpTreeNode* pParent = pIndexNode->getParent();
    newIndexchildNode->setParent(pParent);
    pParent->insertIndexMap(insert_string, newIndexchildNode);

    return;
}

bool BpTree::exceedDataNode(BpTreeNode* pDataNode) {
    if(pDataNode->getDataMap()->size() < this->order){
        return false;
    }
    return true;
}

bool BpTree::exceedIndexNode(BpTreeNode* pIndexNode) {
    if(pIndexNode->getIndexMap()->size() < this->order){
        return false;
    }
    return true;
}

void BpTree::SearchRange(string start, string end) {
    
    if(root == NULL)
    {
        return;
    }

    BpTreeNode *pNode = root;
    
    while(pNode->getMostLeftChild())
    {
        auto iter = pNode->getIndexMap()->end();
        iter--;
        while(true)
        {
            if((iter->first).compare(start) <= 0)
            {
                pNode = iter->second;   
                break;
            }
            else if(iter == pNode->getIndexMap()->begin())
            {
                pNode = pNode->getMostLeftChild();
                break; 
            }
            else
            {
                iter--;
            }
        }
    }

    ofstream flog;
    flog.open("log.txt", ios::app);

    auto iter = pNode->getDataMap()->begin();

    flog << "======= SEARCH_BP =======" << endl;

    
    while((iter->first).compare(end) <= 0)
    {
        VaccinationData* printData = iter->second;
        if (iter->first.compare(start) >= 0)
        {
            flog << printData->GetUserName() << " " << printData->GetVaccineName() << " " << printData->GetTimes() << " " << printData->GetAge() << " " << printData->GetLocationName() << endl;
        }
        iter++;

        if(iter == pNode->getDataMap()->end())
        {
            pNode = pNode->getNext();
            if(pNode == NULL)
            {
                flog << "===========================" << endl << endl;
                flog.close();
                return;
            }
            iter = pNode->getDataMap()->begin();
        }
    }
    
    flog << "===========================" << endl << endl;
    flog.close();

    return;
}

void BpTree::Print() {
    if(root == NULL)
    {
        return;
    }

    BpTreeNode* pNode = root;
    while(pNode->getMostLeftChild())
    {
        pNode = pNode->getMostLeftChild();
    }

    ofstream flog;
    flog.open("log.txt", ios::app);

    auto iter = pNode->getDataMap()->begin();

    flog << "======= PRINT_BP =======" << endl;      
    while(true)
    {
        VaccinationData* printData = iter->second;
        flog << printData->GetUserName() << " " << printData->GetVaccineName() << " " << printData->GetTimes() << " " << printData->GetAge() << " " << printData->GetLocationName() << endl;
        iter++;

        if(iter == pNode->getDataMap()->end())
        {
            pNode = pNode->getNext();

            if(pNode == NULL)
            {
                flog << "===========================" << endl << endl;
                flog.close();
                return;
            }
            iter = pNode->getDataMap()->begin();
        }
    }
}

void BpTree::DeleteBpTree(void){
    if(root == NULL)
    {
        return;
    }
    
    queue<BpTreeNode*> levelQ;

    levelQ.push(root);

    while(levelQ.front()->getMostLeftChild())
    {
       
       levelQ.push(levelQ.front()->getMostLeftChild());
       auto delMap = levelQ.front()->getIndexMap();
       auto iter = delMap->begin();
       while(!(delMap->empty()))
       {
           levelQ.push(iter->second);
           iter = delMap->erase(iter);
       }

       BpTreeNode* del = levelQ.front();
       delete del;
       levelQ.pop();
    }

    while(!(levelQ.empty()))
    {
        auto delMap = levelQ.front()->getDataMap();
        auto iter = delMap->begin();
        while(!(delMap->empty()))
        {
           VaccinationData* delVac = iter->second;
           delete delVac;
           iter = delMap->erase(iter);
        }

        BpTreeNode* del = levelQ.front();
        delete del;
        levelQ.pop();
    }

    root = NULL;
}