#include "Manager.h"
Manager::Manager(int bpOrder) {			//Constructor
	avl = new AVLTree;
	bp = new BpTree(bpOrder);

}

Manager::~Manager() {					//Distructor
	delete avl;
	delete bp;
}

//Program run
void Manager::run(const char* command_txt) {
	
	ifstream command_file;
	ofstream log_file;
	command_file.open(command_txt);
	log_file.open("log.txt", ios::app);

	//if command.txt is not exist, end func
	if(!command_file.is_open())
	{
		log_file << "Can't find " << command_txt << endl;
		log_file.close();
		return;
	}
	
	char cmd[50];
	char* pCmd;

	while(!command_file.eof())			//while all of data in command.txt is read
	{	
		command_file.getline(cmd, 50);	//read line
		pCmd = strtok(cmd, " ");		//seperate line by " "

		if(pCmd == NULL)				//if line have no data, read again
		{
			continue;
		}
		else if(strcmp(pCmd, "LOAD") == 0)		//if Command is "LOAD"
		{
			if(LOAD())			//if LOAD() is worked, print success at log.txt
			{
				log_file << "========== LOAD ==========" <<endl;
				log_file << "Success" << endl;
				log_file << "===========================" << endl << endl;
			}
			else			//if LOAD() isn't worked, print fail at log.txt
			{
				printErrorCode(100);
			}
		}
		else if(strcmp(pCmd, "VLOAD") == 0)		//if Command is "VLOAD"
		{
			if(VLOAD())			//if VLOAD() is worked, print success at log.txt
			{
				log_file << "========== VLOAD ==========" <<endl;
				log_file << "Success" << endl;
				log_file << "===========================" << endl << endl;
			}
			else			//if VLOAD() isn't worked, print fail at log.txt
			{
				printErrorCode(200);
			}
		}
		else if(strcmp(pCmd, "ADD") == 0)		//if Command is "ADD"
		{
			pCmd = strtok(NULL, " ");			//seperate 4 data
			char* d1 = pCmd;
			pCmd = strtok(NULL, " ");
			char* d2 = pCmd;
			pCmd = strtok(NULL, " ");
			char* d3 = pCmd;
			pCmd = strtok(NULL, " ");
			char* d4 = pCmd;

			if(!ADD(d1, d2, d3, d4))			//if ADD() isn't work, print fail at log.txt
			{
				printErrorCode(300);
			}
		}
		else if(strcmp(pCmd, "SEARCH_BP") == 0)		//if Command is "SEARCH_BP"
		{
			pCmd = strtok(NULL, " ");			//seperate 2 data
			char* op1 = pCmd;
			pCmd = strtok(NULL, " ");
			char* op2 = pCmd;

			if(op1 != NULL && op2 != NULL)		//if two data is exist
			{
				if(!SEARCH_BP(op1, op2))		//if SEARCH_BP(op1, op2) isn't worked,  print fail at log.txt
				{
					printErrorCode(400);
				}
			}
			else if(op1 != NULL)			//if one data is exist
			{
				if(!SEARCH_BP(op1))			//if SEARCH_BP(op1) isn't worked, print fail at log.txt
				{
					printErrorCode(400);
				}
			}
			else							//if no data is exist, print fail at log.txt
			{
				printErrorCode(400);
			}
		}
		else if(strcmp(pCmd, "SEARCH_AVL") == 0)	//if Command is "SEARCH_AVL"
		{
			pCmd = strtok(NULL, " ");		//seperate 1 data

			if(pCmd != NULL)				//if data is exist
			{
				if(!SEARCH_AVL(pCmd))		//if SEARCH_AVL() isn't worked, print fail at log.txt
				{
					printErrorCode(500);
				}
			}
			else							//if data isn't exist, print fail at log.txt
			{
				printErrorCode(500);
			}
		}
		else if(strcmp(pCmd, "VPRINT") == 0)		//if Command is "VPRINT"
		{
			pCmd = strtok(NULL, " ");		//seperate 1 data

			if(pCmd != NULL)				//if data is exist
			{
				if(!VPRINT(pCmd))			//if VPRINT() isn't worked, print fail at log.txt
				{
					printErrorCode(600);
				}
			}
			else							//if data isn't exist, print fail at log.txt
			{
				printErrorCode(600);
			}
		}
		else if(strcmp(pCmd, "PRINT_BP") == 0)		//if Command is "PRINT_BP"
		{
			if(bp->GetRoot() == NULL)		//if bp have no data, print fail at log.txt
			{
				printErrorCode(700);
			}
			else							//if bp have data, PRINT()
			{
				bp->Print();
			}
		}
		else if (strcmp(pCmd, "EXIT") == 0)			//if Command is "EXIT", break loop
		{
			break;
		}
		else								//else command, print fail at log.txt
		{
			printErrorCode(800);
		}

	}
	
	bp->DeleteBpTree();		//delete bp and avl and Print_vector
	avl->DeleteTree();
	Print_vector.clear();

	command_file.close();	//file close
	log_file.close();


	return;
}

//Load data from input_data.txt
bool Manager::LOAD() {
	if(bp->GetRoot() != NULL)		//if Root has data, return false
	{
		return false;
	}
	ifstream fdata;
	fdata.open("input_data.txt");

	if(!fdata.is_open())			//if input_data.txt isn't exist, return false
	{
		return false;
	}

	char dataline[50];
	char* pData;

	while(!fdata.eof())				//read data until fdata is end of file
	{
		VaccinationData* Vdata = new VaccinationData;

		fdata.getline(dataline, 50);			//read 1 line
		pData = strtok(dataline, " ");
		
		if(InputVaccinationData(pData, Vdata) == false)		//if VaccinationData is not made, delete it and return false.
		{
			delete Vdata;
			fdata.close();
			return false;
		}

		bp->Insert(Vdata);						//insert data
	}

	fdata.close();

	if(bp->GetRoot() == NULL)				//if no data is exist, return false
	{
		return false;
	}

	return true;
}

//Input Print_vector from AVLTree
bool Manager::VLOAD() {
	if (!Print_vector.empty())		//if Print_vector is not empty, clear it
	{
		Print_vector.clear();
	}

	avl->GetVector(Print_vector);	//insert at Print_vector
	if (Print_vector.empty())		//if Print_vector is empty, return false
	{
		return false;
	}

	return true;
}

bool Manager::ADD(char* pData1, char* pData2, char* pData3, char* pData4) {
		
	if (pData1 == NULL || pData2 == NULL || pData3 == NULL || pData4 == NULL)		//even if one data is not exist, return false
	{
		return false;
	}

	for (int i = 0; pData3[i] != '\0'; i++)			//check pData3 is num
	{
		if (isdigit(pData3[i]) == false)
		{
			return false;
		}
	}
	int num = atoi(pData3);

	VaccinationData* Vdata = new VaccinationData;		//new Data
	Vdata->SetUserName(pData1);
	Vdata->SetVaccineName(pData2);
	Vdata->SetTimes(1);
	Vdata->SetAge(num);
	Vdata->SetLocationName(pData4);

	if (!bp->Insert(Vdata))			//if inserting Data is failed, delete new Data and return false
	{
		delete Vdata;
		return false;
	}

	BpTreeNode* find_node = bp->searchDataNode(pData1);			//find inserted dataNode
	VaccinationData* find_data = NULL;

	for (auto iter = find_node->getDataMap()->begin(); iter != find_node->getDataMap()->end(); iter++)		//find inserted data
	{
		if (iter->first.compare(pData1) == 0)
		{
			find_data = iter->second;
			break;
		}
	}

	if (find_data->GetTimes() == 1 && find_data->GetVaccineName().compare("Janssen") == 0)		//if inserted data's Vaccine is Janssen and time is 1, insert at avl
	{
		avl->Insert(find_data);
	}
	else if (find_data->GetTimes() == 2)	//if inserted data's time is 2, insert at avl
	{
		avl->Insert(find_data);
	}

	ofstream flog;
	flog.open("log.txt", ios::app);

	flog << "========== ADD ==========" <<endl;		//print added node
	flog << pData1 << " "
		 << pData2 << " "
		 << pData3 << " "
		 << pData4 << endl;
	flog << "===========================" << endl << endl;

	flog.close();

	return true;
}

//Search data from BpTree
bool Manager::SEARCH_BP(string name) {
	
	BpTreeNode *data = bp->searchDataNode(name);			//search name at BpTree
	if( data == NULL)		//if DataNode isn't exist, return false
	{
		return false;
	}

	VaccinationData* Vdata = NULL;
	for(auto iter = data->getDataMap()->begin(); iter != data->getDataMap()->end(); iter++)		//search name at DataNode's map
	{
		if(iter->second->GetUserName().compare(name) == 0)
		{
			Vdata = iter->second;
		}
	}

	if(Vdata == NULL)				//if there are no same name, return false
	{
		return false;
	}

	ofstream flog;
	flog.open("log.txt", ios::app);			//print Searched data

	flog << "========== SEARCH_BP ==========" <<endl;
	flog << Vdata->GetUserName() << " "
		 << Vdata->GetVaccineName() << " "
		 << Vdata->GetTimes() << " "
		 << Vdata->GetAge() << " "
		 << Vdata->GetLocationName() << endl;
	flog << "===========================" << endl << endl;

	flog.close();

	return true;
}

//Print data in range
bool Manager::SEARCH_BP(string start, string end) {
	if(bp->GetRoot() == NULL)		//if data isn't exist, return false
	{
		return false;
	}

	bp->SearchRange(start, end);		//print in Range
	return true;
}

//Search data from AVL
bool Manager::SEARCH_AVL(string name) {
	VaccinationData* Vdata = avl->Search(name);			//Search data by name
	if(Vdata == NULL)				//if data isn't exist, return false
	{
		return false;
	}

	ofstream flog;
	flog.open("log.txt", ios::app);			//print Data

	flog << "========== SEARCH_AVL ==========" <<endl;
	flog << Vdata->GetUserName() << " "
		 << Vdata->GetVaccineName() << " "
		 << Vdata->GetTimes() << ""
		 << Vdata->GetAge() << " "
		 << Vdata->GetLocationName() << endl;
	flog << "===========================" << endl << endl;

	flog.close();

	return true;
}

//Sort by A(Vaccine -> Age -> name)
bool Compare(VaccinationData* vac1, VaccinationData* vac2) {
	if(vac1->GetVaccineName().compare(vac2->GetVaccineName()) == 0)
	{
		if (vac1->GetAge() == vac2->GetAge())
		{
			return vac1->GetUserName() < vac2->GetUserName();
		}

		return vac1->GetAge() < vac2->GetAge();
	}

	return vac1->GetVaccineName() < vac2->GetVaccineName();
}

//Sort by B(Location, age(reverse) -> name)
bool Compare2(VaccinationData* vac1, VaccinationData* vac2) {
	if(vac1->GetLocationName().compare(vac2->GetLocationName()) == 0)
	{
		if (vac1->GetAge() == vac2->GetAge())
		{
			return vac1->GetUserName() < vac2->GetUserName();
		}
		return vac1->GetAge() > vac2->GetAge();
	}
	return vac1->GetLocationName() < vac2->GetLocationName();

}

//Print Print_vector
bool Manager::VPRINT(string type_) {
	if(Print_vector.empty())		//if Print_vector have no data, return false
	{
		return false;
	}
	
	ofstream flog;
	flog.open("log.txt", ios::app);

	if(type_.compare("A") == 0)		//if type is A
	{
		sort(Print_vector.begin(), Print_vector.end(), Compare);	//Sort by Compare
		flog << "========== PRINT A ==========" << endl;			//Print it
		for(int i = 0; i < Print_vector.size(); i++)
		{
			flog << Print_vector[i]->GetUserName() << " "
				 << Print_vector[i]->GetVaccineName() << " "
				 << Print_vector[i]->GetTimes() << " "
			 	 << Print_vector[i]->GetAge() << " "
			 	 << Print_vector[i]->GetLocationName() << endl;	
		}
		flog << "===========================" << endl << endl;

	}
	else if(type_.compare("B") == 0)	//if type is B
	{
		sort(Print_vector.begin(), Print_vector.end(), Compare2);	//Sort by Compare2
		flog << "========== PRINT B ==========" << endl;			//print it
		for(int i = 0; i < Print_vector.size(); i++)
		{
			flog << Print_vector[i]->GetUserName() << " "
				 << Print_vector[i]->GetVaccineName() << " "
				 << Print_vector[i]->GetTimes() << " "
			 	 << Print_vector[i]->GetAge() << " "
			 	 << Print_vector[i]->GetLocationName() << endl;		
		}
		flog << "===========================" << endl << endl;
	}
	else		//if type is not A or B
	{
		flog.close();
		return false;
	}

	flog.close();
	return true;

	
}

//Print ErrorCode at log.txt
void Manager::printErrorCode(int n) {
	ofstream fout;
	fout.open("log.txt", ofstream::app);
	fout << "========== ERROR ==========" <<endl;
	fout << n << endl;
	fout << "===========================" << endl << endl;
	fout.close();
}

//input VaccinationData from pData
bool Manager::InputVaccinationData(char* pData, VaccinationData* Vdata)
{
	if(pData == NULL)		//if name isn't exist, return false
	{
		return false;
	}
	Vdata->SetUserName(pData);


	pData = strtok(NULL, " ");
	if(pData == NULL)		//if Vaccine name isn't exist, return false
	{
		return false;
	}
	Vdata->SetVaccineName(pData);


	pData = strtok(NULL, " ");
	if(pData == NULL)		//if times are not exist, return false
	{
		return false;
	}
	for(int i = 0; pData[i] != '\0' ; i++)		//check times are num
	{
		if(isdigit(pData[i]) == false)
		{
			return false;
		}
	}
	int num = atoi(pData);
	if(Vdata->GetVaccineName().compare("Janssen") == 0 && num != 0)		//if Vaccine is Janssen and num is not 0, return false
	{
		return false;
	}
	if(num != 1 && num != 0)			//if num isn't 0 or 1, return false
	{
		return false;
	}
	Vdata->SetTimes(num);


	pData = strtok(NULL, " ");
	if(pData == NULL)				//if age isn't exist, return false
	{
		return false;
	}
	for(int i = 0; pData[i] != '\0' ; i++)		//if age is not int, return false
	{
		if(isdigit(pData[i]) == false)
		{
			return false;
		}
	}
	num = atoi(pData);
	Vdata->SetAge(num);


	pData = strtok(NULL, " ");			//if location isn't exist, return false
	if(pData == NULL)
	{
		return false;
	};
	Vdata->SetLocationName(pData);

	return true;
}