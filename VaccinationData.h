#pragma once
#pragma warning(disable: 4996)
using namespace std;
#include <iostream>
#include <cstring>
#include <fstream>
#include <map>
#include <math.h>
#include <vector>
#include <algorithm>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <functional>
#include <iomanip>


#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>




class VaccinationData {
private:
	string UserName; // User name
	int age;		 // User age
	string VaccineName; // Vaccine name
	string LocationName; // Location name
	int times; // The number of vaccination

public:
	VaccinationData() {
		LocationName = "";
		UserName = "";
		times = 0;
	}
	~VaccinationData() {

	}
	//--NODE information in--
	void SetUserName(string in_name) { UserName = in_name; }
	void SetVaccineName(string in_name) { VaccineName = in_name; }
	void SetLocationName(string in_location) { LocationName = in_location; }
	void SetTimes(int in_times) { times = in_times; }
	void SetAge(int in_age) { age = in_age; }
	void SetTimesInc() { times++; }

	string GetUserName() { return UserName; }
	string GetVaccineName() { return VaccineName; }
	string GetLocationName() { return LocationName; }
	int GetTimes() { return times; }
	int GetAge() { return age; }
};