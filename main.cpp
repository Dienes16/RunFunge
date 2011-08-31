#include <iostream>
#include <cstring>

#include "VirtualMachineEx.h"

//#include "PrecisionTimer.h"

int main( int argc, char **argv )
{
    //PrecisionTimer timer;

    if ( argc <= 1 )
    {
        std::cerr << "Usage: RunFunge.exe <file> [-bf93]";
        return EXIT_FAILURE;
    }

	VirtualMachine* VM;

	if ( argc > 2 && !std::strcmp( argv[2], "-bf93" ) )
		VM = new VirtualMachine;
	else
		VM = new VirtualMachineEx;

	VM->LoadCode( argv[1] );

	//timer.Start();

	int result = VM->Run();

	//std::cout << "\nTime taken: " << timer.Stop() * 1000.0 << "ms";

	delete VM;

	return result;
}
