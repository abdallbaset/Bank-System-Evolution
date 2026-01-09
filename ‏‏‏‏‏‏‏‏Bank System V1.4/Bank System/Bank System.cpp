// Bank System V1.4cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "clsLoginScreen.h"
using namespace std;

int main()
{
	while (true)
	{
	     if(!(clsLoginScreen::showLoginScreen()))
		 {
			 break;
		 }
	}

}


