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

	while(true)
	{
		command_file.getline(cmd, 50);
		pCmd = strtok(cmd, " ");

		if(strcmp(pCmd, "") == 0)
		{
			continue;
		}
		else if(strcmp(pCmd, "LOAD") == 0)
		{
			if(LOAD)
			{

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

			}
			else
			{
				printErrorCode(200);
			}
		}
		else if(strcmp(pCmd, "ADD") == 0)
		{
			
			if(~ADD())
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

}

bool Manager::VLOAD() {
	avl->GetVector(Print_vector);
}

bool Manager::ADD() {

}

bool Manager::SEARCH_BP(string name) {

}

bool Manager::SEARCH_BP(string start, string end) {

}

bool Manager::SEARCH_AVL(string name) {

}

bool Compare(VaccinationData* vac1, VaccinationData* vac2) {

}

bool Manager::VPRINT(string type_) {

}

void Manager::printErrorCode(int n) {
	ofstream fout;
	fout.open("log.txt", ofstream::app);
	fout << "========== ERROR ==========" <<endl;
	fout << n << endl;
	fout << "===========================" << endl << endl;
	fout.close();
}
