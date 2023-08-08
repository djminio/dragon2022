#ifndef QUEST_LOG_H
#define QUEST_LOG_H

struct Quest
{
	string strName;
	string strDescription;
};

class QuestLog
{
public:
	QuestLog(); // Contructor
	virtual ~QuestLog(); // Destructor

	void readQuests();
	vector<Quest> m_Quests;
};

#endif // QUEST_LOG_H