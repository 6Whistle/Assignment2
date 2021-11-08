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

	if(~command_file.is_open())
	{
		log_file << "Can't find " << command_txt << endl;
	}
	
	char cmd[50];
	char* pCmd;

	while(!command_file.eof())
	{
		command_file.getline(cmd, 50);
		pCmd = strtok(cmd, " ");

		if(strcmp(pCmd, "") == 0)
		{
			continue;
		}
		else if(strcmp(pCmd, "LOAD") == 0)
		{
			if(~LOAD())
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
			if(VLOAD)
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
			if(~ADD(pCmd))
			{
				printErrorCode(300);
			}
		}
		else if(strcmp(pCmd, "SEARCH_BP") == 0)
		{
			pCmd = strtok(NULL, " ");
			string op1(pCmd);
			pCmd = strtok(NULL, " ");
			string op2(pCmd);

			if(op1.compare("") != 0 && op2.compare("") != 0)
			{
				if(~SEARCH_BP(op1, op2))
				{
					printErrorCode(400);
				}
			}
			else if(op1.compare("") != 0)
			{
				if(~SEARCH_BP(op1))
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
				if(~SEARCH_AVL(op1))
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
			string op1(pCmd);

			if(op1.compare("") != 0)
			{
				if(~VPRINT(op1))
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
			string startp = "A";
			string endp = "z";
			if(~SEARCH_BP(startp, endp))
			{
				printErrorCode(700);
			}
		}
		else
		{
			printErrorCode(800);
		}

	}
	

}

bool Manager::LOAD() {
	if(bp->GetRoot() == NULL)
	{
		return false;
	}
	ifstream fdata;
	fdata.open("input_data.txt");

	if(~fdata.is_open())
	{
		return false;
	}

	char dataline[50];
	char* pData;

	while(!fdata.eof())
	{
		VaccinationData* Vdata = new VaccinationData;

		fdata.getline(dataline, 50);
		pData = strtok(pData, " ");
		
		if(InputVaccinationData(pData, Vdata) == false)
		{
			delete Vdata;
			fdata.close();
			return false;
		}

		bp->Insert(Vdata);
		
		if(Vdata->GetTimes() == 1 && Vdata->GetVaccineName().compare("Janssen") == 0)
		{
			avl->Insert(Vdata);
		}
		else if(Vdata->GetTimes() == 2)
		{
			avl->Insert(Vdata);
		}
	}

	fdata.close();

	if(bp->GetRoot() == NULL)
	{
		return false;
	}

	return true;
}

bool Manager::VLOAD() {
	avl->GetVector(Print_vector);
}

bool Manager::ADD(char* pData) {
		
	VaccinationData* Vdata = new VaccinationData;

	pData = strtok(NULL, " ");

	if(InputVaccinationData(pData, Vdata) == false)
	{
		delete Vdata;
		return false;
	}

	ofstream flog;
	flog.open("log.txt", ios::app);

	flog << "========== ADD ==========" <<endl;
	flog << Vdata->GetUserName() << " "
		 << Vdata->GetVaccineName() << " "
		 << Vdata->GetAge() << " "
		 << Vdata->GetLocationName() << endl;
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

}

bool Compare(VaccinationData* vac1, VaccinationData* vac2) {
	if(vac1->GetVaccineName().compare(vac2->GetVaccineName()) < 0)
	{
		return vac1 < vac2;
	}
	if(vac1->GetVaccineName().compare(vac2->GetVaccineName()) > 0)
	{
		return vac1 > vac2;
	}

	if(vac1->GetAge() < vac2->GetAge())
	{
		return vac1 < vac2;
	}
	if(vac1->GetAge() > vac2->GetAge())
	{
		return vac1 > vac2;
	}

	if(vac1->GetUserName().compare(vac2->GetUserName()) < 0)
	{
		return vac1 < vac2;
	}

	return vac1 > vac2;
}

bool Compare2(VaccinationData* vac1, VaccinationData* vac2) {
	if(vac1->GetLocationName().compare(vac2->GetLocationName()) < 0)
	{
		return vac1 < vac2;
	}
	if(vac1->GetLocationName().compare(vac2->GetLocationName()) > 0)
	{
		return vac1 > vac2;
	}

	if(vac1->GetAge() > vac2->GetAge())
	{
		return vac1 < vac2;
	}
	if(vac1->GetAge() < vac2->GetAge())
	{
		return vac1 > vac2;
	}

	if(vac1->GetUserName().compare(vac2->GetUserName()) < 0)
	{
		return vac1 < vac2;
	}

	return vac1 > vac2;
}

bool Manager::VPRINT(string type_) {
	avl->GetVector(Print_vector);
	if(Print_vector.empty())
	{
		return false;
	}
	
	ofstream flog;
	flog.open("log.txt", ios::app);

	if(type_.compare("A") == 0)
	{
		sort(Print_vector.begin(), Print_vector.end(), Compare);
		
		for(int i = i; Print_vector[i] != NULL; i++)
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

		for(int i = i; Print_vector[i] != NULL; i++)
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
	if(strcmp(pData, "") == 0)
	{
		return false;
	}
	string name = pData;
	Vdata->SetUserName(pData);


	pData = strtok(NULL, " ");
	if(strcmp(pData, "") == 0)
	{
		return false;
	}
	string name = pData;
	Vdata->SetVaccineName(pData);


	pData = strtok(NULL, " ");
	if(strcmp(pData, "") == 0)
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
	if(strcmp(pData, "") == 0)
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
	if(strcmp(pData, "") == 0)
	{
		return false;
	}
	string name = pData;
	Vdata->SetLocationName(pData);

	return true;
}