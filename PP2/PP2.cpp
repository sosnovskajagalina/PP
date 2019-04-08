#include "stdafx.h"
#include "Bank.h"
#include "BankClient.h"

using namespace std;

set<string> esc = { "exit", "quit", "esc" };
string quit;
int clients;

bool isEsc(string es)
{
	return (esc.find(es) != esc.end());
}

void Info()
{
	cout << "this is not right commands" << endl;
	cout << "you need at less 2 commands or 3 not more or less" << endl;
	cout << "For example < pp2.exe 3>" << endl;
}

int main(int argc, char *argv[])
{
	Info();
	if (argc == 2) 
	{
		clients = atoi(argv[1]);
		if (clients < 1)
			Info();
	}
	else
		Info();
	
	CBank* bank = new CBank();

	for(size_t i = 0; i < clients; i++){
		CBankClient* client1 = bank->CreateClient();
	}

	// TODO: WaitForMultipleObjects
	while (cin >> quit)
	{
		if (isEsc(quit))
			break;
	}

    return 0;
}