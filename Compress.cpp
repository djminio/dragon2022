#include "stdafx.h"																	
#include "String.h"
#include "mmsystem.h"														
#include "Dragon.h"	
//#include "MenuDef.h"

#include "network.h"																	
#include "compress.h"

T_NODE NODE;

BYTE dict[DICT_SIZE];
								
BYTE mask_byte;
BYTE buffer_byte;

int	 read_offset;

t_packet packet_comp;

//---------------------------------------------------------------------------------
int GetChar(t_packet *packet)
{
	    if(read_offset >= packet->h.header.size)
			return(-1);

		return((int)packet->u.data[read_offset ++]);
};

//---------------------------------------------------------------------------------
void PutChar(int c)
{
	    if(packet_comp.h.header.size >= MAX_PACKET_SIZE)
			return;

		packet_comp.u.data[packet_comp.h.header.size ++] = (char)c;
};

//---------------------------------------------------------------------------------
void WriteBits(unsigned int bits, int length)
{
	unsigned int mask_word;

		mask_word = 1 << (length - 1);

		while(mask_word != 0) 
		{
			switch(length) 
			{
				case 1  : 
					if(bits)
						buffer_byte |= mask_byte;
					break;

				default : 
					if(mask_word & bits)
						buffer_byte |= mask_byte;
					break;
			}

			mask_byte >>= 1;
			if(mask_byte == 0) 
			{
				PutChar(buffer_byte);
				buffer_byte = 0;
				mask_byte = 0x80;
			}
			mask_word >>= 1;
		}
};

//---------------------------------------------------------------------------------
void InitTree(int r)
{
		NODE[ROOT].right = r;
		NODE[r].parent = ROOT;
		NODE[r].right = UNUSED;
		NODE[r].left = UNUSED;
};

//---------------------------------------------------------------------------------
void ContractNode(int old_node, int new_node)
{
		NODE[new_node].parent = NODE[old_node].parent;

		if(NODE[NODE[old_node].parent].right == old_node)
			NODE[NODE[old_node].parent].right = new_node;
		else
			NODE[NODE[old_node].parent].left = new_node;

		NODE[old_node].parent = UNUSED;
};

//---------------------------------------------------------------------------------
void ReplaceNode(int old_node, int new_node)
{
	int parent;

		parent = NODE[old_node].parent;
		if(NODE[parent].left == old_node)
			NODE[parent].left = new_node;
		else
			NODE[parent].right = new_node;
		NODE[new_node] = NODE[old_node];
		NODE[NODE[new_node].left].parent = new_node;
		NODE[NODE[new_node].right].parent = new_node;
		NODE[old_node].parent = UNUSED;
};

//---------------------------------------------------------------------------------
int FindNextNode(int node)
{
	int next;

		next = NODE[node].left;
		while (NODE[next].right != UNUSED)
			next = NODE[next].right;
		return(next);
};

//---------------------------------------------------------------------------------
void DeleteString(int p)
{
	int replacement;

		if(NODE[p].parent == UNUSED)
			return;
		if(NODE[p].right == UNUSED)
			ContractNode(p, NODE[p].left);
		else 
		if(NODE[p].left == UNUSED)
		 ContractNode(p, NODE[p].right);
		else 
		{
			replacement = FindNextNode(p);
			DeleteString(replacement);
			ReplaceNode(p, replacement);
    }
};

//---------------------------------------------------------------------------------
int AddString(int new_node, int *match_position)
{
	int i;
	int test_node;
	int delta;
	int match_length;
	int *child;

		if(new_node == END_INDEX)
			return (0);

		test_node = NODE[ROOT].right;
		match_length = 0;

		while(1)
		{
			for(i = 0; i < BUFFER_SIZE; i++) 
			{
				delta = dict[MOD_WINDOW(new_node + i)] - dict[MOD_WINDOW(test_node + i)];
				if (delta != 0)
					break;
			}
			if(i >= match_length) 
			{
				match_length = i;
				*match_position = test_node;
				if(match_length >= BUFFER_SIZE) 
				{
					ReplaceNode(test_node, new_node);
					return (match_length);
				}
			}
			if(delta >= 0)
				child = &NODE[test_node].right;
			else
				child = &NODE[test_node].left;
			if(*child == UNUSED) 
			{
				*child = new_node;
				NODE[new_node].parent = test_node;
				NODE[new_node].right = UNUSED;
				NODE[new_node].left = UNUSED;
				return (match_length);
			}

			test_node = *child;
		}
};

//---------------------------------------------------------------------------------
unsigned int ReadBits(t_packet *packet, int length)
{
	unsigned mask_word;
	unsigned bits = 0;

		mask_word = 1 << (length - 1);

		while(mask_word != 0) 
		{
			if(mask_byte == 0x80) 
				buffer_byte = GetChar(packet);
			if(buffer_byte & mask_byte)
				bits |= mask_word;
			mask_word >>= 1;
			mask_byte >>= 1;
			if(mask_byte == 0)
				mask_byte = 0x80;
		}

		return bits;
};

//---------------------------------------------------------------------------------
void Compress(t_packet *packet)
{
	return;

	int c;
	int i;
	int rest_count;
	int current_position;
	int replace_count;
	int match_length;
	int match_position;

		if(packet->h.header.size <= 0)
			return;
		
		memset(&packet_comp, 0, sizeof(t_packet));
		read_offset	= 0;

		packet_comp.h.header.crc  = packet->h.header.crc;
		packet_comp.h.header.type = packet->h.header.type;

        mask_byte = 0x80;
		buffer_byte	= 0;
		current_position = 1;

		for(i = 0; i < BUFFER_SIZE; i++) 
		{
			c = GetChar(packet);
			if(c == -1)
				break;
			dict[current_position + i] = (BYTE)c;
		}

		rest_count = i;
		InitTree(current_position);

		match_length = 0;
		match_position = 0;

		while(rest_count > 0) 
		{
			if(match_length > rest_count)
				match_length = rest_count;

			if(match_length <= 1) 
			{
				replace_count = 1;
				WriteBits(RAW_CHAR_MARK, 1);
				WriteBits(dict[current_position], 8);
			}
			else 
			{
				WriteBits(TOKEN_MARK, 1);
				WriteBits(match_position, INDEX_BIT_SIZE);
				WriteBits((match_length - 2), LENGTH_BIT_SIZE);
				replace_count = match_length;
			}

			for(i = 0; i < replace_count; i ++)
			{
				DeleteString(MOD_WINDOW(current_position+BUFFER_SIZE));

				c = GetChar(packet);
				if(c == -1)
					rest_count--;
				else
					dict[MOD_WINDOW(current_position + BUFFER_SIZE)] = (BYTE)c;

				current_position = MOD_WINDOW(current_position + 1);
				if(rest_count)
					match_length = AddString(current_position, &match_position);
			}
		}

		WriteBits(TOKEN_MARK, 1);
		WriteBits(END_INDEX, INDEX_BIT_SIZE);

		if(mask_byte != 0x80)
			PutChar(buffer_byte);

		memcpy(packet, &packet_comp, sizeof(t_packet));
};

//---------------------------------------------------------------------------------
void Expand(t_packet *packet)
{
	return;

	int i;
	int current_position;
	int c;
	int match_length;
	int match_position;

		if(packet->h.header.size <= 0)
			return;
		
		memset(&packet_comp, 0, sizeof(t_packet));
		read_offset	= 0;

		packet_comp.h.header.crc  = packet->h.header.crc;
		packet_comp.h.header.type = packet->h.header.type;

        mask_byte = 0x80;
		buffer_byte	= 0;
		current_position = 1;

		while(1)
		{
			if(ReadBits(packet, 1)) 
			{
				c = (int)ReadBits(packet, 8);
				PutChar(c);
				dict[current_position] = (BYTE)c;
				current_position = MOD_WINDOW(current_position + 1);
			} 
			else 
			{
				match_position = (int)ReadBits(packet, INDEX_BIT_SIZE);
				if(match_position == END_INDEX)
					break;

				match_length = (int)ReadBits(packet, LENGTH_BIT_SIZE);
				match_length += 1;

				for (i = 0; i <= match_length; i++) 
				{
					c = dict[MOD_WINDOW(match_position + i)];
					PutChar(c);
					dict[current_position] = (BYTE)c;
					current_position = MOD_WINDOW(current_position + 1);
				}
			}
		}

		memcpy(packet, &packet_comp, sizeof(t_packet));
};

//---------------------------------------------------------------------------------
BOOL Compare(t_packet *packet1, t_packet *packet2)
{
	int i;

		if(packet1->h.header.size != packet2->h.header.size)
			return(FALSE);

		for(i = 0; i < packet1->h.header.size; i ++)
		{
			if(packet1->u.data[i] != packet2->u.data[i])
				return(FALSE);
		}

		return(TRUE);
};

//---------------------------------------------------------------------------------
