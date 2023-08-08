#include "stdafx.h"
#include "network.h"
#include "BanSys.h"

#include "Menu.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
void CBanSys::Banaccount(char* name, char* reason, char* duration)
{
	t_packet p;
	p.h.header.type = CMD_BAN_ACCOUNT;
	p.h.header.size = sizeof(t_ban_account);

	memcpy(&p.u.BanAccount.characterName, name, 20);
	memcpy(&p.u.BanAccount.reason, reason, 255);
	memcpy(&p.u.BanAccount.duration, duration, 255);
	QueuePacket(&p, 1);
}

void CBanSys::UnBanaccount(char* name)
{
	t_packet p;
	p.h.header.type = CMD_UNBAN_ACCOUNT;
	p.h.header.size = sizeof(t_ban_account);

	memcpy(&p.u.BanAccount.characterName, name, 20);
	QueuePacket(&p, 1);
}

void CBanSys::LockAccount(char* name, char* reason)
{
	t_packet p;
	p.h.header.type = CMD_ACCOUNT_LOCK;
	p.h.header.size = sizeof(t_ban_account);
	memcpy(&p.u.BanAccount.characterName, name, 20);
	memcpy(&p.u.BanAccount.reason, reason, 255);
	QueuePacket(&p, 1);

}
