#include "BpTree.h"
bool BpTree::Insert(VaccinationData* newData){

}
BpTreeNode * BpTree::searchDataNode(string n) {

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
    pDataNode->getNext()->setPrev(newDataNode);
    pDataNode->setNext(newDataNode);
    newDataNode->setNext(pDataNode);

    newDataNode->setParent(pDataNode->getParent());

    newDataNode->getParent()->insertIndexMap(newDataNode->getDataMap()->begin()->first, newDataNode);

    return;
}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode) {
    BpTreeIndexNode *newIndexchildNode1 = new BpTreeIndexNode;
    BpTreeIndexNode *newIndexchildNode2 = new BpTreeIndexNode;
    
    /*left Index Node*/
    auto iter = pIndexNode->getIndexMap()->begin();
    
    newIndexchildNode1->setMostLeftChild(pIndexNode->getMostLeftChild());
    newIndexchildNode1->getMostLeftChild()->setParent(newIndexchildNode1);

    newIndexchildNode1->insertIndexMap(iter->first, iter->second);
    pIndexNode->getIndexMap()->erase(iter);
    newIndexchildNode1->getIndexMap()->begin()->second->setParent(newIndexchildNode1);

    newIndexchildNode1->setParent(pIndexNode);


    /*Left Index Node*/
    iter = pIndexNode->getIndexMap()->end();
    iter--;
    
    newIndexchildNode2->setMostLeftChild(pIndexNode->getIndexMap()->begin()->second);
    newIndexchildNode2->getMostLeftChild()->setParent(newIndexchildNode2);

    newIndexchildNode2->insertIndexMap(iter->first, iter->second);
    pIndexNode->getIndexMap()->erase(iter);
    
    newIndexchildNode2->getIndexMap()->begin()->second->setParent(newIndexchildNode2);

    newIndexchildNode2->setParent(pIndexNode);

    /*parent Node*/
    pIndexNode->setMostLeftChild(newIndexchildNode1);
    string str = pIndexNode->getIndexMap()->begin()->first;
    pIndexNode->getIndexMap()->clear();
    pIndexNode->insertIndexMap(str, newIndexchildNode2);

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
    
    BpTreeNode *pNode = root;
    
    while(pNode->getMostLeftChild())
    {
        auto iter = pNode->getIndexMap()->end();
        iter--;
        while(true)
        {
            if((iter->first).compare(start) > 0)
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
        flog << printData->GetUserName() << " " << printData->GetVaccineName() << " " << printData->GetTimes() << " " << printData->GetAge() << " " << printData->GetLocationName() << endl;
        if(iter == pNode->getDataMap()->end())
        {
            pNode = pNode->getNext();
            iter = pNode->getDataMap()->begin();
        }
        else
        {
            iter++;
        }
    }

    flog.close();

    return;
}

void BpTree::Print() {

    

}
