#ifndef _XORCRYPT_H_
#define _XORCRYPT_H_
#include <time.h>

class XorCrypt
{
private:
	//unsigned int keyvalsum;
	unsigned int key;
	unsigned int value;
public:
	//virtual ~XorCrypt();
	void PutEncrypted(int subj);
	int GetDecrypted(void);
	//int GetEncrypted();

};

#endif
