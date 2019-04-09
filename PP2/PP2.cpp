#include "stdafx.h"
#include "Bank.h"
#include "BankClient.h"

using namespace std;

set<string> esc = { "exit", "quit", "esc" };
string quit;
int clients = 2, primitive = 0, ClientsBalance = 0;

bool isEsc(string es)
{
	return (esc.find(es) != esc.end());
}

void Info()
{
	cout << "this is not right commands" << endl;
	cout << "you need at less 3 commands" << endl;
	cout << "For example < pp2.exe 3> <clients number> then (0 or 1) SynchronousPrimitiveOption: CriticalSection = 0, Mutex = 1" << endl;

}

int main(int argc, char *argv[])
{
	Info();
	if (argc == 2) 
	{
		primitive = atoi(argv[1]);
		if (primitive != 0 || primitive != 1)
			Info();
	}
	else if (argc == 3) {
		clients = atoi(argv[1]);
		primitive = atoi(argv[2]);
		if (clients < 1 || primitive != 0 || primitive != 1)
			Info();
	}
	else
		Info();
	
	CBank* bank = new CBank(primitive);

	for(size_t i = 0; i < clients; i++){
		CBankClient* client1 = bank->CreateClient();
	}

	// TODO: WaitForMultipleObjects
	while (cin >> quit)
	{
		if (isEsc(quit))
			break;
	}

	for (CBankClient client : bank->totalClients()) {
		auto clientBalance = bank->getClientBalance(client.GetId());

		ClientsBalance += clientBalance;
	}

	cout << "Clients balance: " << ClientsBalance << endl;
	cout << "Bank balance: " << bank->GetTotalBalance() << endl;

	return 0;
}