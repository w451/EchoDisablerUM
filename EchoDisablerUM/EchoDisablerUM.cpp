#include <iostream>
#include "comm.hpp"
#include "utils.hpp"
using namespace std;

int main()
{
	if (!Comm::initNt()) {
		cout << "Couldn't find win32u.dll!NtUserCheckProcessForClipboardAccess" << endl;
		getchar();
		return 0;
	}
	cout << "Succesfully got NtUserCheckProcessForClipboardAccess" << endl;

	if (!Comm::doHeartbeat()) {
		cout << "Driver not loaded or not working!" << endl;
		getchar();
		return 0;
	}

	cout << "Driver loaded up and working!" << endl;

	//Your stuff here

	getchar();
}