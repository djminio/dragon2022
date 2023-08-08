#ifndef __QUESTTEXT_H__
#define __QUESTTEXT_H__

#define QUEST_NAME 512
#define QUEST_MAX 200
// 퀘스트 텍스트 줄나누기 
#define QUEST_TEXT_DEVISION		20

#define		SCREEN_SIZE		SCREEN_HEIGHT
//퀘스트 조건 관련 기본설정 
#define		QUEST_IF_COUNT					10		//퀘스트 조건 수/*정확한 수치 입력*/  
	
#define		QUEST_IF_LEVEL					1
#define		QUEST_IF_CLASS					2
#define		QUEST_IF_JOB					3
#define		QUEST_IF_SKILL					4
#define		QUEST_IF_MEMBERCOUNT			5
#define		QUEST_IF_ITEM					6
#define		QUEST_IF_FAME					7
#define		QUEST_IF_JOB_FAME				8
#define		QUEST_IF_QUEST_NUM				9
#define		QUEST_IF_QUEST_TIME				10
// 퀘스트 완료시 주어지는 수치들 관련 
#define		QUEST_TAKE_COUNT				9		//퀘스트 조건 수/*정확한 수치 입력*/  

#define		QUEST_TAKE_MONEY				1
#define		QUEST_TAKE_FIGHT_FAME			2
#define     QUEST_TAKE_JOB_FAME				3
#define		QUEST_TAKE_FIGHT_DO				4
#define		QUEST_TAKE_JOB_DO				5
#define		QUEST_TAKE_ITEM					6
#define		QUEST_TAKE_WSWIZ				7
#define		QUEST_TAKE_PSWIZ				8
#define		QUEST_TAKE_POINT				9

extern void LoadQuestTable( char *filename ,char *filename2,char *filename_bin );

//extern void ReadQuestData(  int no, char *buf );
extern void kh_InitMenu();
extern void SetEndQuest(int num);
extern void SetStartQuest(int num);
//ReadQuestTable ob;

extern int	QUEST_GIVE;
extern int	QUEST_COUNT;
extern int end_quest[QUEST_MAX];

extern  void CallKhMenuProc( int menu, int field );		// 화면 출력
extern bool DoButtonCheckOfMenu_kh( int i, int j );//020410 lsw

extern int kein_kh_key_on;
extern void LoadSpriteOnlyOne( Spr *s, char filename[MAX_FILENAME] );		// 스프라이트 화일 하나만 읽어 올때
extern void FreeSpriteOnlyOne( Spr *s );
extern void FreeSpriteOnlyOne( Spr *s );
extern void kh_menu_set( );
extern void lsw_menu_set( );//010830 lsw
extern int	quest_num[QUEST_MAX] ; 
extern int	QUEST_COUNT ;
extern void kh_KeyProc();







//kkh 퀘스트에서 더 가지고 있어야 할 정보 시작 시간과 끝낸시간 즉 걸린시간 
class QuestTable {
public :
//	ReadQuestTable*		next;	//Linked list
// 필드에서 번호	이름. 기타 관련 ,,,
	int		num;							//	퀘스트 번호
	int		code;							//	퀘스트 코드
	char	name[QUEST_NAME];				//	퀘스트 이름 
	int		place;							//	퀘스트 발생장소 
	char	ch[QUEST_NAME];					//	퀘스트 내용 
	int		quest_if[QUEST_IF_COUNT];		//  퀘스트 발생 조건 
	int     quest_take[QUEST_TAKE_COUNT];	//  퀘스트 완료시 주어지는 수치 관련 	
	int		picnum;	
	char	picture[QUEST_NAME];
	Spr		sq;
//......................................
	//int Q_Conut[QUEST_INDEX_VIEW];
//	int	req_lv;
//	int	req_str;
	int count;
	char* buf;
	char* token;
// FUNCTION............................................................
	void LoadTable ( char *filename );	// 문자열에서 데이타를 분리
	void LoadPicture ( char *filename );	// 
	void SaveTable();		// txt 파일 구해오기
	//	void ReadQuestData( int no, char *buf );
	QuestTable() { num = -1; /*next = NULL;*/ }
	QuestTable(int n) { num = n;/*next = NULL;*/}
/*	void QUEST_ON(int QUEST_NUM);
	void QUEST_OFF(int QUEST_NUM);
	void QUEST_PRINT();*/	//Linked list
//.....................................................................
};
	



extern QuestTable quest_text[QUEST_MAX];
extern void FreeQuest();			// 스프라이트 프리
extern void QestDataFree( );		// 데이타 구조체 0으로 셋팅
extern void CharacterWorkingWater( LPCHARACTER ch );





#endif