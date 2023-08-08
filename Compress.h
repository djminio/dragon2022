#ifndef _COMPRESS_
#define _COMPRESS_

#define END_INDEX			0

#define INDEX_BIT_SIZE 		12
#define LENGTH_BIT_SIZE		4
#define DICT_SIZE			4096
#define BUFFER_SIZE			17
#define	ROOT				4096
#define UNUSED				0
#define MOD_WINDOW(a)		((a) & (DICT_SIZE - 1))
#define RAW_CHAR_MARK		1
#define TOKEN_MARK			0

typedef struct defNODE
{
  int parent;
  int left;
  int right;
} T_NODE[DICT_SIZE + 1];

extern void Compress(t_packet *packet);
extern void Expand(t_packet *packet);
extern BOOL Compare(t_packet *packet1, t_packet *packet2);

#endif