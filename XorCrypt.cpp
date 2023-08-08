/* XOR Encryption class - Thralas */
#include "StdAfx.h"
#include "XorCrypt.h"
#include "dragon.h"

int XorCrypt::GetDecrypted(void)
{
	return (value ^ key );
}

void XorCrypt::PutEncrypted(int subj)
{
	int	subj2 = subj; 
	unsigned int oldkey;
	unsigned int newkey;
	oldkey = newkey = key;
	srand((unsigned)time(NULL));
	if(oldkey != key && newkey != key) ExitApplication(EA_CRACK_THREAD_FIND_CRACK);
	while(newkey == oldkey && newkey == key)
	{
		newkey = rand();
	}

	key = newkey;
	if(key != newkey) ExitApplication(EA_CRACK_THREAD_FIND_CRACK);

	if(subj2 != subj) ExitApplication(EA_CRACK_THREAD_FIND_CRACK);
	subj += 1;

	if(subj - 1 != subj2) ExitApplication(EA_CRACK_THREAD_FIND_CRACK);
	value = subj2 ^ newkey;
}

/*
int XorCrypt::GetEncrypted()
{
	return value;
}
*/