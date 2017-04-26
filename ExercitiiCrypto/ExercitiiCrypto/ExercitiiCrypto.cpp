// ExercitiiCrypto.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#define NUMBERS_COUNT 10

int find_numbers[NUMBERS_COUNT];

int VerifyNumbers(int counter, int* vNumbers, int len)
{
	int count = 0;
	
	for (int i = 0; i < counter - 1; i++)
	{
		for (int j = i + 1; j < counter; j++)
		{
			if (find_numbers[i] == find_numbers[j])
				return 0;
		}
	}
	bool flag;
	for (int i = 0; i < counter ; i++)
	{
		for (int j = i + 1; j < counter; j++)
		{
			flag = false;
			int sum = find_numbers[i] + find_numbers[j];
			for (int k = 0; k < len; k++)
			{
				if (sum == vNumbers[k])
				{
					flag = true;
					break;
				}
			}
			if (flag == false)
				return 0;
		}
	}
	return 1;
}

void FindNumbers(int start_point, int counter, int* vNumbers, int len)
{
	int static end_point = 100;
	static bool flag = false;
	
	if (counter == NUMBERS_COUNT)
	{
		if (VerifyNumbers(counter, vNumbers, len))
			flag = true;
		return;
	}
	if (counter == 1)
	{
		start_point = -find_numbers[0];
	}
	if (counter > 0 && find_numbers[0] > 0)
		return;

	if (counter >= 2)
	{
		if (!VerifyNumbers(counter,vNumbers, len))
		{
			return;
		}
	}

	for (int i = start_point; i < end_point; i++)
	{
		if (flag == true)
			break;
		find_numbers[counter] = i;
		FindNumbers(start_point+1, counter+1, vNumbers, len);
	}
}

int main()
{
	int numbers[] = { 0, 2, 9, 12, 19, 20, 21, 30, 31, 32, 37, 39, 41, 43, 47, 49, 50, 53, 56, 61, 61, 63, 65, 67, 71, 72, 73, 78, 80, 83, 86, 91, 94, 96, 98, 100, 102, 105, 108, 116, 124, 127, 133, 149, 157 };
	FindNumbers(-10, 0, numbers, 45);
	for (int i = 0; i < NUMBERS_COUNT; i++)
	{
		printf("%d ",find_numbers[i]);
	}
	return 0;
}

