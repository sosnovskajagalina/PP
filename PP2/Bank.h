#pragma once
#include <iostream>
#include <vector>
#include <mutex>
#include <map>
#include "BankClient.h"

enum SynchronousPrimitive {
	CriticalSection,
	Mutex
};

using namespace std;

class CBank
{
public:
	CBank(int primitive);

	CBankClient* CreateClient();
	void UpdateClientBalance(CBankClient& client, int value);
	SynchronousPrimitive GetPrimitive();
	map<int, int>ClientBalance;
	int getClientBalance(int clientId);
	void setClientBalance(int clientId, int value);
	int GetTotalBalance();
	vector<CBankClient>totalClients();
	


private:
	
	vector<CBankClient> m_clients;
	int m_totalBalance;

	void SetTotalBalance(int value);
	void SomeLongOperations();

	SynchronousPrimitive m_primitive;

	void EnableSynchronous();
	void DisableSynchronous();

	mutex mutexUpdateBalance;
	CRITICAL_SECTION csUpdateBalance;
};