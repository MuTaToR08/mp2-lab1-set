#include <gtest.h>
#include "tbitfield.h"
#include <iostream>
#include <bitset>

int main(int argc, char **argv) {
	try
	{
		TBitField temp1(60);
		TBitField temp(temp1);
		temp.pMem[0] = 2;
		std::cin >> temp;
		std::cout << temp;
		std::cout << RUN_ALL_TESTS();
		//std::cout<<std::endl << temp.BitLen<<"|"<<temp.GetCountField()<<std::endl;
		//std::cout << temp.MemLen;



		//::testing::InitGoogleTest(&argc, argv);
	}
	catch (BITFIELD_ERRORS err)
	{
		std::cout << "Throw error number:" << err<<endl;
		
	}
	system("PAUSE");
	
    return 0;
}
