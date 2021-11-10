#include "Manager.h"
Manager::Manager(int bpOrder) {
	avl = new AVLTree;
	bp = new BpTree(bpOrder);

}

Manager::~Manager() {
	delete avl;
	delete bp;

	//delete vecter data
}

void Manager::run(const char* command_txt) {
	
	ifstream command_file;
	ofstream log_file;
	command_file.open(command_txt);
	log_file.open("log.txt", ios::app);

	if(!command_file.is_open())
	{
		log_file << "Can't find " << command_txt << endl;
		log_file.close();
		return;
	}
	
	char cmd[50];
	char* pCmd;

	while(!command_file.eof())
	{
		command_file.getline(cmd, 50);
		pCmd = strtok(cmd, " ");

		if(pCmd == NULL)
		{
			continue;
		}
		else if(strcmp(pCmd, "LOAD") == 0)
		{
			if(LOAD())
			{
				log_file << "========== LOAD ==========" <<endl;
				log_file << "Success" << endl;
				log_file << "===========================" << endl << endl;
			}
			else
			{
				printErrorCode(100);
			}
		}
		else if(strcmp(pCmd, "VLOAD") == 0)
		{
			if(VLOAD())
			{
				log_file << "========== VLOAD ==========" <<endl;
				log_file << "Success" << endl;
				log_file << "===========================" << endl << endl;
			}
			else
			{
				printErrorCode(200);
			}
		}
		else if(strcmp(pCmd, "ADD") == 0)
		{
			pCmd = strtok(NULL, " ");
			char* d1 = pCmd;
			pCmd = strtok(NULL, " ");
			char* d2 = pCmd;
			pCmd = strtok(NULL, " ");
			char* d3 = pCmd;
			pCmd = strtok(NULL, " ");
			char* d4 = pCmd;

			if(!ADD(d1, d2, d3, d4))
			{
				printErrorCode(300);
			}
		}
		else if(strcmp(pCmd, "SEARCH_BP") == 0)
		{
			pCmd = strtok(NULL, " ");
			char* op1 = pCmd;
			pCmd = strtok(NULL, " ");
			char* op2 = pCmd;

			if(op1 != NULL && op2 != NULL)
			{
				if(!SEARCH_BP(op1, op2))
				{
					printErrorCode(400);
				}
			}
			else if(op1 != NULL)
			{
				if(!SEARCH_BP(op1))
				{
					printErrorCode(400);
				}
			}
			else
			{
				printErrorCode(400);
			}
		}
		else if(strcmp(pCmd, "SEARCH_AVL") == 0)
		{
			pCmd = strtok(NULL, " ");
			string op1(pCmd);

			if(op1.compare("") != 0)
			{
				if(!SEARCH_AVL(op1))
				{
					printErrorCode(500);
				}
			}
			else
			{
				printErrorCode(500);
			}
		}
		else if(strcmp(pCmd, "VPRINT") == 0)
		{
			pCmd = strtok(NULL, " ");

			if(pCmd != NULL)
			{
				if(!VPRINT(pCmd))
				{
					printErrorCode(600);
				}
			}
			else
			{
				printErrorCode(600);
			}
		}
		else if(strcmp(pCmd, "PRINT_BP") == 0)
		{
			if(bp->GetRoot() == NULL)
			{
				printErrorCode(700);
			}
			else
			{
				bp->Print();
			}
		}
		else if (strcmp(pCmd, "EXIT") == 0)
		{
			break;
		}
		else
		{
			printErrorCode(800);
		}

	}
	
	bp->DeleteBpTree();
	avl->DeleteTree();
	Print_vector.clear();
	command_file.close();
	log_file.close();
	_CrtDumpMemoryLeaks();

	return;
}

bool Manager::LOAD() {
	if(bp->GetRoot() != NULL)
	{
		return false;
	}
	ifstream fdata;
	fdata.open("input_data.txt");

	if(!fdata.is_open())
	{
		return false;
	}

	char dataline[50];
	char* pData;

	while(!fdata.eof())
	{
		VaccinationData* Vdata = new VaccinationData;

		fdata.getline(dataline, 50);
		pData = strtok(dataline, " ");
		
		if(InputVaccinationData(pData, Vdata) == false)
		{
			delete Vdata;
			fdata.close();
			return false;
		}

		bp->Insert(Vdata);
	}

	fdata.close();

	if(bp->GetRoot() == NULL)
	{
		return false;
	}

	return true;
}

bool Manager::VLOAD() {
	if (!Print_vector.empty())
	{
		Print_vector.clear();
	}

	avl->GetVector(Print_vector);
	if (Print_vector.empty())
	{
		return false;
	}

	return true;
}

bool Manager::ADD(char* pData1, char* pData2, char* pData3, char* pData4) {
		
	if (pData1 == NULL || pData2 == NULL || pData3 == NULL || pData4 == NULL)
	{
		return false;
	}

	for (int i = 0; pData3[i] != '\0'; i++)
	{
		if (isdigit(pData3[i]) == false)
		{
			return false;
		}
	}
	int num = atoi(pData3);

	VaccinationData* Vdata = new VaccinationData;
	Vdata->SetUserName(pData1);
	Vdata->SetVaccineName(pData2);
	Vdata->SetTimes(1);
	Vdata->SetAge(num);
	Vdata->SetLocationName(pData4);

	if (!bp->Insert(Vdata))
	{
		delete Vdata;
		return false;
	}

	BpTreeNode* find_node = bp->searchDataNode(pData1);
	VaccinationData* find_data = NULL;

	for (auto iter = find_node->getDataMap()->begin(); iter != find_node->getDataMap()->end(); iter++)
	{
		if (iter->first.compare(pData1) == 0)
		{
			find_data = iter->second;
			break;
		}
	}

	if (find_data->GetTimes() == 1 && find_data->GetVaccineName().compare("Janssen") == 0)
	{
		avl->Insert(find_data);
	}
	else if (find_data->GetTimes() == 2)
	{
		avl->Insert(find_data);
	}

	ofstream flog;
	flog.open("log.txt", ios::app);

	flog << "========== ADD ==========" <<endl;
	flog << pData1 << " "
		 << pData2 << " "
		 << pData3 << " "
		 << pData4 << endl;
	flog << "===========================" << endl << endl;

	flog.close();

	return true;
}

bool Manager::SEARCH_BP(string name) {
	
	BpTreeNode *data = bp->searchDataNode(name);
	if( data == NULL)
	{
		return false;
	}

	VaccinationData* Vdata = NULL;
	for(auto iter = data->getDataMap()->begin(); iter != data->getDataMap()->end(); iter++)
	{
		if(iter->second->GetUserName().compare(name) == 0)
		{
			Vdata = iter->second;
		}
	}

	if(Vdata == NULL)
	{
		return false;
	}

	ofstream flog;
	flog.open("log.txt", ios::app);

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

bool Manager::SEARCH_BP(string start, string end) {
	if(bp->GetRoot() == NULL)
	{
		return false;
	}

	bp->SearchRange(start, end);
	return true;
}

bool Manager::SEARCH_AVL(string name) {
	VaccinationData* Vdata = avl->Search(name);
	if(Vdata == NULL)
	{
		return false;
	}

	ofstream flog;
	flog.open("log.txt", ios::app);

	flog << "========== PRINT_AVL ==========" <<endl;
	flog << Vdata->GetUserName() << " "
		 << Vdata->GetVaccineName() << " "
		 << Vdata->GetTimes() << ""
		 << Vdata->GetAge() << " "
		 << Vdata->GetLocationName() << endl;
	flog << "===========================" << endl << endl;

	flog.close();

	return true;
}

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

bool Manager::VPRINT(string type_) {
	if(Print_vector.empty())
	{
		return false;
	}
	
	ofstream flog;
	flog.open("log.txt", ios::app);

	if(type_.compare("A") == 0)
	{
		sort(Print_vector.begin(), Print_vector.end(), Compare);
		
		for(int i = 0; Print_vector[i] != NULL; i++)
		{
			flog << "========== PRINT A ==========" <<endl;
			flog << Print_vector[i]->GetUserName() << " "
				 << Print_vector[i]->GetVaccineName() << " "
				 << Print_vector[i]->GetTimes() << " "
			 	 << Print_vector[i]->GetAge() << " "
			 	 << Print_vector[i]->GetLocationName() << endl;
			flog << "===========================" << endl << endl;			
		}

	}
	else if(type_.compare("B") == 0)
	{
		sort(Print_vector.begin(), Print_vector.end(), Compare2);

		for(int i = 0; Print_vector[i] != NULL; i++)
		{
			flog << "========== PRINT B ==========" <<endl;
			flog << Print_vector[i]->GetUserName() << " "
				 << Print_vector[i]->GetVaccineName() << " "
				 << Print_vector[i]->GetTimes() << " "
			 	 << Print_vector[i]->GetAge() << " "
			 	 << Print_vector[i]->GetLocationName() << endl;
			flog << "===========================" << endl << endl;			
		}
	}
	else
	{
		Print_vector.clear();
		flog.close();
		return false;
	}

	Print_vector.clear();
	flog.close();
	return true;

	
}

void Manager::printErrorCode(int n) {
	ofstream fout;
	fout.open("log.txt", ofstream::app);
	fout << "========== ERROR ==========" <<endl;
	fout << n << endl;
	fout << "===========================" << endl << endl;
	fout.close();
}

bool Manager::InputVaccinationData(char* pData, VaccinationData* Vdata)
{
	if(pData == NULL)
	{
		return false;
	}
	Vdata->SetUserName(pData);


	pData = strtok(NULL, " ");
	if(pData == NULL)
	{
		return false;
	}
	Vdata->SetVaccineName(pData);


	pData = strtok(NULL, " ");
	if(pData == NULL)
	{
		return false;
	}
	for(int i = 0; pData[i] != '\0' ; i++)
	{
		if(isdigit(pData[i]) == false)
		{
			return false;
		}
	}
	int num = atoi(pData);
	if(Vdata->GetVaccineName().compare("Janssen") == 0 && num != 0)
	{
		return false;
	}
	if(num != 1 && num != 0)
	{
		return false;
	}
	Vdata->SetTimes(num);


	pData = strtok(NULL, " ");
	if(pData == NULL)
	{
		return false;
	}
	for(int i = 0; pData[i] != '\0' ; i++)
	{
		if(isdigit(pData[i]) == false)
		{
			return false;
		}
	}
	num = atoi(pData);
	Vdata->SetAge(num);


	pData = strtok(NULL, " ");
	if(pData == NULL)
	{
		return false;
	};
	Vdata->SetLocationName(pData);

	return true;
}