#include "VendingMachine.hpp"

int main()
{
	VendingMachine Machine;
	Machine.Start();
	while (Machine.work())
	{
	}
}
