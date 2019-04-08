#include "Bank.h"

CBank::CBank(int primitive)
{
	m_clients = std::vector<CBankClient>();
	m_totalBalance = 0;
	m_primitive = SynchronousPrimitive(primitive);
	InitializeCriticalSection(&csUpdateBalance);
}

CBankClient* CBank::CreateClient()
{
	unsigned clientId = unsigned(m_clients.size());
	CBankClient* client = new CBankClient(this, clientId);
	m_clients.push_back(*client);
	return client;
}

void CBank::UpdateClientBalance(CBankClient &client, int value)
{	
	EnableSynchronous();
	int totalBalance = GetTotalBalance();
	std::cout << "Client " << client.GetId() << " initiates reading total balance. Total = " << totalBalance << "." << std::endl;
	
	SomeLongOperations();
	totalBalance += value;
	//at first we need to set the new values of balance for each client so we need a setter and getter for it
	setClientBalance(client.GetId(), value);

	std::cout
		<< "Client " << client.GetId() << " updates his balance with " << value
		<< " and initiates setting total balance to " << totalBalance
		<< ". Must be: " << GetTotalBalance() + value << "." << std::endl;

	// Check correctness of transaction through actual total balance
	if (totalBalance != GetTotalBalance() + value) {
		std::cout << "! ERROR !" << std::endl;
	}

	SetTotalBalance(totalBalance);
	DisableSynchronous();
}

int CBank::GetTotalBalance()
{
	return m_totalBalance;
}

void CBank::SetTotalBalance(int value)
{
	m_totalBalance = value;
}

void CBank::SomeLongOperations()
{
	// TODO
	Sleep(1000);
}

void CBank::setClientBalance(int clientId, int value)
{	//here we search for the client by his id
	auto Balance = ClientBalance.find(clientId);
	//then if this is the end of list butting his last balance if not adding the new one
	if (Balance == ClientBalance.end()) {
		ClientBalance.emplace(clientId, value);
	}
	else {
		Balance->second += value;
	}
}

vector<CBankClient> CBank::GetTotalClients() {
	return m_clients;
}

int CBank::getClientBalance(int clientId)
{
	return ClientBalance.at(clientId);
}

SynchronousPrimitive CBank::GetPrimitive() {
	return m_primitive;
}

void CBank::EnableSynchronous() {
	if (CBank::GetPrimitive() == SynchronousPrimitive::CriticalSection) {
		EnterCriticalSection(&csUpdateBalance);
	}
	else if (CBank::GetPrimitive() == SynchronousPrimitive::Mutex) {
		mutexUpdateBalance.lock();
	}
}

void CBank::DisableSynchronous() {
	if (CBank::GetPrimitive() == SynchronousPrimitive::CriticalSection) {
		LeaveCriticalSection(&csUpdateBalance);
	}
	else if (CBank::GetPrimitive() == SynchronousPrimitive::Mutex) {
		mutexUpdateBalance.unlock();
	}
}

