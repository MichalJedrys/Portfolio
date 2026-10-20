#include "VendingMachine.hpp"

int main()
{
	VendingMachine Machine;
	Machine.Start();
	while (std::cin)
	{
		Machine.work();
	}
}
