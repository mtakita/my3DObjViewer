#include "pch.h"
#include "IncreasingNumber.h"

IncreasingNumber::IncreasingNumber(){
	currentNumber = 0;
}

int IncreasingNumber::GetNextNumber() {
	currentNumber++;
	return currentNumber;
}