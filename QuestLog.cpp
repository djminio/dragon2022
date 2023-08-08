#include "stdafx.h"
#include "QuestLog.h"

QuestLog::QuestLog()
{
	readQuests();
}

QuestLog::~QuestLog()
{
	m_Quests.clear();	
}

void QuestLog::readQuests()
{
	ifstream QuestFile;
	QuestFile.open("./data/quests.txt", ios::binary, ios::in);
	if (!QuestFile.is_open())
	{
		// Error here!
		return;
	}

	short nQuestCount = 0;
	QuestFile >> nQuestCount;
	m_Quests.reserve(nQuestCount);

	while (!QuestFile.eof())
	{
		Quest quest;

		short nLenName = 0;	
		QuestFile >> nLenName;

		quest.strName.resize(nLenName);
		QuestFile.read(const_cast<char*>(quest.strName.c_str()), nLenName);

		short nLenDescription = 0;
		QuestFile >> nLenDescription;

		quest.strDescription.resize(nLenDescription);
		QuestFile.read(const_cast<char*>(quest.strDescription.c_str()), nLenDescription);

		m_Quests.push_back(quest);
	}
	
	QuestFile.close();
}