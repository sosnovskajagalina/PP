#pragma once
#include <iostream>
#include <vector>
#include <mutex>
#include "BankClient.h"

class CBank
{
public:
	CBank();
	CBankClient* CreateClient();
	void UpdateClientBalance(CBankClient& client, int value);

private:
	std::vector<CBankClient> m_clients;
	int m_totalBalance;

	int GetTotalBalance();
	void SetTotalBalance(int value);
	void SomeLongOperations();
	HANDLE mutex = CreateMutex(NULL, FALSE, NULL);
	CRITICAL_SECTION critSec;
};