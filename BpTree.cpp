#include "BpTree.h"

//insert 1 data at BpTree
bool BpTree::Insert(VaccinationData* newData){
    if(root == NULL)        //if BpTree is empty, make DataNode and insert data
    {
        BpTreeDataNode *DataNode = new BpTreeDataNode;

        DataNode->insertDataMap(newData->GetUserName(), newData);
        root = DataNode;
        return true;
    }

    BpTreeNode *pNode = root;       //BpTree isn't empty

    //move pNode to DataNode that have to save
    while(pNode->getMostLeftChild())        //while pNode is IndexNode
    {
        auto iter = pNode->getIndexMap()->end();        //last element of IndexMap
        iter--;

        while(true)
        {
            if((iter->first).compare(newData->GetUserName()) <= 0)  //if element's name <= insert name, pNode = element->second
            {
                pNode = iter->second;   
                break;
            }
            else if(iter == pNode->getIndexMap()->begin())  //if index->start's name > element's name, pNode = mostleftchild
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

    //Check same data is exist
    for(auto iter = pNode->getDataMap()->begin() ; iter != pNode->getDataMap()->end() ; iter++)     //search in DataNode
    {
        if(iter->first.compare(newData->GetUserName()) == 0)    //if same name Data is exist
        {
            //if data's Vaccine is Janssen and count == 0, or data == 0 or data == 1
            if(!(iter->second->GetVaccineName().compare("Janssen") == 0 && iter->second->GetTimes() == 1) && !(iter->second->GetTimes() == 2))
            {
                iter->second->SetTimesInc();            //increase times
                delete newData;                         //delete same data
                return true;
            }

            return false;                               //if Vaccine is Janssen and data != 0, or !(0 <= data <= 2)
        }
    }
    
    //Have no same data
    pNode->insertDataMap(newData->GetUserName(), newData);


    if(exceedDataNode(pNode))           //if DataNode have 3 data
    {
        if(pNode->getParent() == NULL)      //if root is DataNode, Make IndexNode and set root, and split current dataNode
        {
            BpTreeIndexNode* newNode = new BpTreeIndexNode;
            pNode->setParent(newNode);
            newNode->setMostLeftChild(pNode);
            root = newNode;
            splitDataNode(pNode);

            return true;
        }
        splitDataNode(pNode);       //root is Not DataNode, split current dataNode
        
        while(true)
        {
            pNode = pNode->getParent();     //mave to Parents

            if(exceedIndexNode(pNode))      //if IndexNode's have 3 data
            {
                if(pNode->getParent() == NULL)      //if current Node is root, make indexNode and set parent, and split current IndexNode
                {
                    BpTreeIndexNode* newNode = new BpTreeIndexNode;
                    pNode->setParent(newNode);
                    newNode->setMostLeftChild(pNode);
                    root = newNode;
                    splitIndexNode(pNode);

                    return true;
                }
                
                splitIndexNode(pNode);             //if current Node is not root, split current InextNode
            }
            else        //if Index Node have data less than 3, end
            {
                return true;
            }
        }
    }

    return true;        //if Data Node have data less than 3, end
}

//Search name in BpTree
BpTreeNode * BpTree::searchDataNode(string n) {
    if(root == NULL)        //if data is not exist
    {
        return NULL;
    }
    
    BpTreeNode *pNode = root;

    while(pNode->getMostLeftChild())        //while pNode is not IndexNode
    {
        auto iter = pNode->getIndexMap()->end();        //this element is end of IndexMap
        iter--;
        while(true)
        {
            if((iter->first).compare(n) <= 0)           //if element name < search name, pNode = element->second
            {
                pNode = iter->second;   
                break;
            }
            else if(iter == pNode->getIndexMap()->begin())      //if first element's name > search name, pNode = MostleftChild
            {
                pNode = pNode->getMostLeftChild();
                break; 
            }
            else            //else, element--
            {
                iter--;
            }
        }
    }

    return pNode;               //return DataNode
}

//Split DataNode
void BpTree::splitDataNode(BpTreeNode* pDataNode) {
    BpTreeDataNode* newDataNode = new BpTreeDataNode;       //new Node

    auto iter = pDataNode->getDataMap()->begin();
    iter++;

    //insert second and third element at new Node, and erase at pDataNode
    newDataNode->insertDataMap(iter->first, iter->second);
    iter = pDataNode->getDataMap()->erase(iter);
    newDataNode->insertDataMap(iter->first, iter->second);
    pDataNode->getDataMap()->erase(iter);

    newDataNode->setNext(pDataNode->getNext());     //set new Node's next
    if (pDataNode->getNext() != NULL)               //if pDataNode's next is exist, set that node's prev
    {
        pDataNode->getNext()->setPrev(newDataNode);
    }
    pDataNode->setNext(newDataNode);                //set pDataNode's next
    newDataNode->setPrev(pDataNode);                //set new Node's prev

    newDataNode->setParent(pDataNode->getParent());     //set new Node's parent

    newDataNode->getParent()->insertIndexMap(newDataNode->getDataMap()->begin()->first, newDataNode);  //insert index data at parent


    return;
}

//split IndexNode
void BpTree::splitIndexNode(BpTreeNode* pIndexNode) {
    BpTreeIndexNode *newIndexchildNode = new BpTreeIndexNode;       //mew node
    
    auto iter = pIndexNode->getIndexMap()->begin();
    iter++;

    string insert_string = iter->first;             //second element's name
    newIndexchildNode->setMostLeftChild(iter->second);      //second element's pointer is new node's mostleftchild
    iter = pIndexNode->getIndexMap()->erase(iter);          //erase second element in pIndexNode
    newIndexchildNode->insertIndexMap(iter->first, iter->second);       //insert third element at new node
    pIndexNode->getIndexMap()->erase(iter);                 //erase third element in pIndexNode
    
    //set new node child's parent
    newIndexchildNode->getMostLeftChild()->setParent(newIndexchildNode);
    newIndexchildNode->getIndexMap()->begin()->second->setParent(newIndexchildNode);

    //set new node's parent
    BpTreeNode* pParent = pIndexNode->getParent();
    newIndexchildNode->setParent(pParent);
    pParent->insertIndexMap(insert_string, newIndexchildNode);

    return;
}

//check dataNode have 3 data
bool BpTree::exceedDataNode(BpTreeNode* pDataNode) {
    if(pDataNode->getDataMap()->size() < this->order){
        return false;
    }
    return true;
}

//check indexNode have 3 data
bool BpTree::exceedIndexNode(BpTreeNode* pIndexNode) {
    if(pIndexNode->getIndexMap()->size() < this->order){
        return false;
    }
    return true;
}

//print start ~ end data from BpTree
void BpTree::SearchRange(string start, string end) {
    
    if(root == NULL)        //if data isn't exist
    {
        return;
    }

    BpTreeNode *pNode = root;
    
    while(pNode->getMostLeftChild())    //while pNode is indexNode
    {
        auto iter = pNode->getIndexMap()->end(); //element is last of map
        iter--;
        while(true)
        {
            if((iter->first).compare(start) <= 0)          //if element name < start name, pNode = element->second
            {
                pNode = iter->second;   
                break;
            }
            else if(iter == pNode->getIndexMap()->begin())  //if first element's name > start name, pNode = MostleftChild
            {
                pNode = pNode->getMostLeftChild();
                break; 
            }
            else        //else, element--
            {
                iter--;
            }
        }
    }

    ofstream flog;
    flog.open("log.txt", ios::app);

    auto iter = pNode->getDataMap()->begin();

    flog << "======= SEARCH_BP =======" << endl;

    
    while((iter->first).compare(end) <= 0)      //while element's data < end
    {
        VaccinationData* printData = iter->second;
        if (iter->first.compare(start) >= 0)        //if element's data > start, print it
        {
            flog << printData->GetUserName() << " " << printData->GetVaccineName() << " " << printData->GetTimes() << " " << printData->GetAge() << " " << printData->GetLocationName() << endl;
        }
        iter++;

        if(iter == pNode->getDataMap()->end())      //if element is end of map
        {
            pNode = pNode->getNext();               //if next is not exist, end
            if(pNode == NULL)
            {
                flog << "===========================" << endl << endl;
                flog.close();
                return;
            }
            iter = pNode->getDataMap()->begin();    //if next is exist, move to next
        }
    }
    
    flog << "===========================" << endl << endl;      //if element > end, end
    flog.close();

    return;
}

//print BpTree
void BpTree::Print() {
    if(root == NULL)        //if data isn't exist, end
    {
        return;
    }

    BpTreeNode* pNode = root;
    while(pNode->getMostLeftChild())        //go to most left DataNode
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
        //print data
        flog << printData->GetUserName() << " " << printData->GetVaccineName() << " " << printData->GetTimes() << " " << printData->GetAge() << " " << printData->GetLocationName() << endl;
        iter++;

        if(iter == pNode->getDataMap()->end())  //if element is end of map
        {
            pNode = pNode->getNext();

            if(pNode == NULL)           //if element's next is not exist, end
            {
                flog << "===========================" << endl << endl;
                flog.close();
                return;
            }
            iter = pNode->getDataMap()->begin();        //if element is exist, move next
        }
    }
}

//Delete all of BpTree
void BpTree::DeleteBpTree(void){
    if(root == NULL)        //if data isn't exist, end
    {
        return;
    }
    
    queue<BpTreeNode*> levelQ;          //delete by level order

    levelQ.push(root);      //push root

    //Delete IndexNode
    while(levelQ.front()->getMostLeftChild())       //while current node is IndexNode
    {
       
       levelQ.push(levelQ.front()->getMostLeftChild());     //push MostLeftChild
       auto delMap = levelQ.front()->getIndexMap();
       auto iter = delMap->begin();
       while(!(delMap->empty()))
       {
           levelQ.push(iter->second);               //push all of data's pointer in IndexMap and erase that
           iter = delMap->erase(iter);
       }

       BpTreeNode* del = levelQ.front();        //delete Node
       delete del;
       levelQ.pop();                    //pop queue
    }

    //Delete DataNode
    while(!(levelQ.empty()))    //delete all of data in Queue
    {
        auto delMap = levelQ.front()->getDataMap();
        auto iter = delMap->begin();
        while(!(delMap->empty()))           //delete all of VaccinationData in DataMap
        {
           VaccinationData* delVac = iter->second;
           delete delVac;
           iter = delMap->erase(iter);
        }

        BpTreeNode* del = levelQ.front();
        delete del;                         //delete DataNode
        levelQ.pop();                   //pop
    }

    root = NULL;
}