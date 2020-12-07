/***************************************************************************************************
MIT License

Copyright (c) 2021 Suraj Thite

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************************************/

/**
 * @\file	gen_random.c
 * @\author	Suraj Thite
 * @\brief	This file creates a binary to generate random numbers for insertion,search and range querry operations 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <iostream>
#include <algorithm>

using namespace std;

int main(int argc, char **argv)
{
	if (argc != 8)
	{
		printf("Usage: ./gen_random [insert file_name] [Number of elements to insert] [Max limit] [search file_name] [Number of elements to search] [range file_name] [Number of range querry]\n");
		exit(0);
	}

	char *in_file = argv[1];
	int count = atoi(argv[2]);
	int max_limit = atoi(argv[3]);
	char *src_file = argv[4];
	int scount = atoi(argv[5]);
	char *rfilename = argv[6];
	int rcount = atoi(argv[7]);
	int buffer[count];

	int i = 0;

	FILE *ifptr=NULL, *sfptr=NULL, *rfptr=NULL;


	srand(time(NULL));

	ifptr = fopen(in_file, "w+");

	if (!ifptr)
	{
		printf("Failed to create a file\n");
		exit(0);
	}

	for (i = 0; i < count; i++)
	{
		buffer[i] = (int)(random() % max_limit);
		fprintf(ifptr, "%d %d\n", buffer[i], i);
	}

	fclose(ifptr);

	sfptr = fopen(src_file, "w+");

	if (!sfptr) {
		printf("Failed to create a file\n");

		exit(0);
	}

	for (i = 0; i < scount; i++)
	{
		fprintf(sfptr, "%d\n", buffer[i]);
	}

	fclose(sfptr);

	sort(buffer, (buffer + count));

	rfptr = fopen(rfilename, "w+");

	if (!rfptr)
	{
		printf("Failed to create a file\n");
		exit(0);
	}

	for (i = 0; i < rcount-1; i++)
	{
		fprintf(rfptr, "%d %d\n", buffer[i], buffer[i + 1]);
	}

	fclose(rfptr);

	return 0;
}
