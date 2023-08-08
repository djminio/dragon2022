#include "skill_lsw.h"
#define CMD_SKILL_RARE_UPGRADE					5600
#define CMD_SKILL_RARE_UPGRADE_RESULT			5601

#define GM_TOOL_VERSION							2122//020101 lsw 

typedef struct HWOARANG_SANGWOO_SKILLRAREUPGRADE
{
	short int iSkillNo;

	ItemAttr	SourceItem;

	int iBuffer1;
	int iBuffer2;
	
	POS		pos[MAX_UPGRADE_RARE];

}t_SkillRareUpgrade;

typedef struct HWOARANG_SANGWOO_SKILLRAREUPGRADE_RESULT
{
	short int	iSuccess;
	ItemAttr	SourceItem;

}t_SkillRareUpgradeResult;


#define CMD_CALL_SMALLMENUCUSTOM				2002
typedef struct server_call_smallmenucustom
{
	short int iMenuNo;
	short int iPicNo;
	short int iWidth;
	short int iHeight;
	short int iExt;
}t_server_call_smallmenucustom;

#define CMD_RARE_UPGRADE_BBS					2003
#define CMD_RARE_UPGRADE_BBS_MAP_TO_MAP			2004

typedef struct HWOARANG_SANGWOO_RAREUPGRADE_BBS
{	
	char	name[20];
	ItemAttr	Item;
}t_RareUpgradeBBS;

typedef struct HWOARANG_SANGWOO_RAREUPGRADE_BBS_MAP_TO_MAP
{	
	char	name[20];
	ItemAttr	Item;
}t_RareUpgradeBBSMapToMap;


#define CMD_ON_CONNECT	6099
typedef struct ONCONNECTHSELINIT
{
	HSEL_INITIAL	eninit;
	HSEL_INITIAL	deinit;
}t_OnConnectHselInit;

#define CMD_MOVE_TO_HOMETOWN					2005
#define CMD_MOVE_TO_HOMETOWN_WITH_ALIVE			2007

#define CMD_DAYLIGHT							2006//광원 처리 보내기
typedef struct DAYLIGHT
{
	short int nDayLightControl;
}t_DayLight,*LPDAYLIGHT;

#define CMD_GET_SKILL_POINT						9018

#define CMD_NOW_SKILL_POINT						9019
typedef struct NOWSKILLPOINT
{
	ItemAttr	item;
	int			iNowPoint;	
}t_NowSkillPoint,*LPNOWSKILLPOINT;

#define CMD_MERCHANT_BUY_ITEM_SEARCH			16000//020815 lsw

//<soto-030511 수정.
typedef struct SEARCHPACKETCLIENT
{
	char	szKeyWord[21];	//키워드 검색에 들어갈
	char	szMerchant[21]; //상인이름 검색에 들어갈
	int		nPeriod;		//등록 기간 설정.// 0:1일, 1:3일,  2:일주일.
	int		iIndex;
	int		iKey;
}t_SearchPacketClient,*lpSEARCHPACKETCLIENT;
//>soto-030511 수정.


//구매자의 서치를 위한 구조체들
typedef struct SEARCHRESULT
{
	int			iIndex;
	char		szSellerName[21];
	char		szBuyerName[21];
	int			iSellValue;//판매금액은 20억을 넘기지 않습니다.
	int			iIsEnd;
	int			iSellerTake;
	int			iBuyerTake;
	ItemAttr	m_ResultItem;
}t_SearchResult,*lpSEARCHRESULT;

#define CMD_MERCHANT_BUY_ITEM_SEARCH_RESULT			16001//020815 lsw
#define CMD_MERCHANT_SELL_ITEM_INFO_REQUEST_RESULT	16004

#define CMD_MERCHANT_RESULT_LIST_REQUEST			16030
#define CMD_MERCHANT_RESULT_LIST_REQUEST_RESULT		16031

const int MAX_SEARCH_RESULT_LIST = 24;
const int MAX_VIEW_ABLE_PER_PAGE = 6;//한페이지에 볼 수 있는 리스트 수

const int AUCTION_PAGE_MIN = 0;//저거에서 나온 페이지 수
const int AUCTION_PAGE_MAX = 3;//저거에서 나온 페이지 수

typedef struct SEARCHRESULTLIST
{
	int iCn;
	char szName[21];
	int iKey;
	SEARCHRESULT ResultList[MAX_SEARCH_RESULT_LIST];
}t_SearchResultList,*lpSEARCHRESULTLIST;
//
//구매자용 아이템 구입을 하겠다는 패킷
#define CMD_MERCHANT_ITEM_BUY						16005
typedef struct MERCHANT_ITEM_BUY
{
	int			iIndex;//구입할 아이템의 인덱스 값
	int			iCn;//커넥션 값
	char		szSellerName[21];//판매자 이름
	char		szBuyerName[21];//구매자 이름
	DWORD		dwSellValue;//판매금액은 20억을 넘기지 않습니다.
	ItemAttr	SellItem;
	int			iKey;
}t_MerchantItemBuy,*lpMERCHANTITEMBUY;

#define CMD_MERCHANT_ITEM_BUY_RESULT	16006
//

//판매자용 구조체
#define CMD_MERCHANT_SELL_ITEM_INFO_REQUEST		16003
typedef struct SELLERITEMREQUEST
{
	int iCn;
	int iIndex;
	int iKey;
	char szName[21];//자신의 이름을 보내서 아이템을 검색 합니다.
}t_SellerItemRequest,*lpSELLERITEMREQUEST;


#define CMD_MERCHANT_SELL_ITEM_REGISTER					16009//판매아이템을 등록 합니다.
#define CMD_MERCHANT_SELL_ITEM_REGISTER_RESULT			16010//판매아이템을 등록 합니다.
typedef struct SELLERITEMREGISTER
{
	int			iCn;
	char		szName[21];
	int			iFKRareType;
	int			iFKLevel;
	int			iFKTacticType;
	int			iFKWearType;
	POS			pos;
	DWORD		dwSellValue;//--> 값이 0 이면 직접거래
	ItemAttr	SellItem;
	int			iKey;
}t_SellerItemRegister,*lpSELLERITEMREGISTER;

#define CMD_MERCHANT_SELL_ITEM_DELETE				16020
#define CMD_MERCHANT_SELL_ITEM_DELETE_RESULT		16021
#define CMD_MERCHANT_RESULT_TAKE					16032
#define CMD_MERCHANT_RESULT_TAKE_RESULT				16033

#define CMD_MERCHANT_BACKUP_LIST_REQUEST			16036	// BBD 040303
#define CMD_MERCHANT_BACKUP_LIST_RESPONSE			16037	// BBD 040303
#define	CMD_MERCHANT_BACKUP_TAKE_REQUEST			16038	// BBD 040303
#define CMD_MERCHANT_BACKUP_TAKE_RESPONSE			16039	// BBD 040303

typedef struct SELLERITEMDELETE
{
	int			iIndex;//판매물품의 인덱스
	int			iCn;
	char		SellerszName[21];
	char		szName[21];
	DWORD		dwSellValue;
	ItemAttr	SellItem;
	int			iKey;
}t_SellerItemDelete,lpSELLERITEMDELETE;

//#define CMD_MERCHANT_RESULT_TAKE						16032
//#define CMD_MERCHANT_RESULT_TAKE_RESULT					16033
#define CMD_MERCHANT_RESULT_TAKE_COMFORM				16034
#define CMD_MERCHANT_RESULT_TAKE_COMFORM_RESULT			16035
typedef struct MERCHANTRESULTTAKE
{
	int			iIndex;//판매물품의 인덱스
	int			iCn;
	char		szMyName[21];
	char		SellerName[21];
	char		BuyerName[21];
	DWORD		dwSellValue;
	ItemAttr	SellItem;
	int			iSellerTake;
	int			iBuyerTake;
	int			iKey;
}t_MerchantResultTake,*lpMERCHANTRESULTTAKE;

#define CMD_MERCHANT_DIRECT_EXCHANGE_LIST_REQUSET					16090
#define CMD_MERCHANT_DIRECT_EXCHANGE_LIST_REQUSET_RESULT			16091
#define CMD_MERCHANT_EXCHANGE_RESULT								16101
typedef struct MERCHANTEXCHANGEREQUEST
{
	int			iIndex;//0으로 날리도록 하고
	int			iCn;
	char		szSellerName[21];
	char		szMyName[21];
	int			iKey;//인덱스 위인가 아래인가.
}t_MerchantExchangeRequest,*lpMERCHANTEXCHANGEREQUEST;

const int CMD_SKILL_MASTER_MULTI_RARE_MAKE				=		16200;

typedef struct MULTIRARESRC
{
	MakeItem MultiRareSrc[2];
}t_MultiRareSrc,*LPMULTIRARESRC;

typedef struct SKILLMASTERMULTIRAREMAKE
{
	int iCn;
	int iSkillNo;
	MULTIRARESRC MultiRareSrc;
}t_SkillMasterMultiRareMake,*LPSKILLMASTERMULTIRAREMAKE;

const int CMD_EXCHANGE_BOND_MONEY				=		16201;
typedef struct EXCHANGEBONDMONEY
{
	int iCn;
	POS	pos;//위치 
	ITEMATTR BondMoneyItem;//본드머니 정보
	int iKey;//여분으로 사용할 키
}t_ExchangeBondMoney,*LPEXCHANGEBONDMONEY;

const int CMD_SELECTED_CHAR_RESULT_CLIENT_BOTH_DB_DEMON = 16103;
typedef struct SELECTEDCHARRESULT
{
	int			iKey;
	char		szName[MAX_NAME];
	int			iLv;
}t_SelectedCharResult,*LPSELECTEDCHARRESULT;

const int CMD_EXCHANGE_CANCEL_SIGN = 16202;

const int CMD_MAP_MOVE_LV_LIMIT		= 16203;

typedef struct MAPMOVELVLIMIT
{
	int iLvMin;
	int iLvMax;
}t_MapMoveLvLimit,*LPMAPMOVELVLIMIT;

const int CMD_TRADE_MODE_CHANGE				=		16204;//030127 lsw
typedef struct TRADEMODECHANGE
{
	int iCn;
	bool bIsTradeOn;
}t_TradeModeChange,*LPTRADEMODECHANGE;

#define CMD_CONNECT_AGENT_SERVER_CONFORM	6098
typedef struct CONNECTAGENTSERVERCONFORM
{
	char szUserID[ID_LENGTH];
	char szUserPASS[PW_LENGTH];
	int iKey;
}t_ConnectAgentServerConform,*LPCONNECTAGENTSERVERCONFORM;

// Mail System //////////////////////////////////////////////////////////////////////////////
//메일 관련 보내기 (클라이언트)
const int MAX_MAIL_PAGE	= 12;
const int MAX_MAIL_RECEIVER	= 5;
const int MAX_MAIL_BODY_LEN	= 1000;

typedef struct MAILDATA
{
	int iIndex;

	int iType;
	bool bIsRead;
	
	char szSender[20];
	char szReceiver[20];
	
	char szTitle[60];
	TIMESTAMP_STRUCT	date;
	int	iBodyIndex;
}t_MailDdata, *LPMAILDATA;

typedef struct INTERFACEMAILDATA//인터페이스에서 쓸 구조체
{
	bool bIsSelect;		
	MAILDATA MailData;
}t_InterfaceMailData,*LPINTERFACEMAILDATA;

typedef struct MAILBODY
{
	int iIndex;
	char szBody[MAX_MAIL_BODY_LEN];
}t_MailBody, *LPMAILBODY;

const int CMD_MAIL_REQ_BODY		= 16102;//해당 메일의 바디
typedef struct REQESTMAILBODY
{
//	int iWantMailType;
	int iMailIndex;
	char szName[MAX_NAME];
}t_ReqestMailBody,*LPREQESTMAILBODY;

const int CMD_MAIL_REQ_BODY_RESULT		= 16113;//해당 메일의 바디
typedef struct REQESTMAILBODYRESULT
{
//	int iWantMailType;
	int iMailIndex;
	int iType;
	char szSender[MAX_NAME];
	char szReceiver[MAX_NAME];
	char szTitle[MAX_NAME];
	TIMESTAMP_STRUCT	date;
	char szBody[MAX_MAIL_BODY_LEN];
}t_ReqestMailBodyResult,*LPREQESTMAILBODYRESULT;

const int CMD_MAIL_REQUEST_LIST = 16114;//한페이지 분량의 메일 리스트.
typedef struct REQESTMAILLIST
{
	int iWantMailType;
	int iPageIndex;
	char szName[MAX_NAME];
}t_ReqestMailList,*LPREQESTMAILLIST;

//메일 관련 받기 (클라이언트)
const int CMD_MAIL_ALERT = 16400;//메일 얼럿

const int CMD_MAIL_SEND	=10142;	// 메일 보내기
typedef struct MAILSEND
{
	int iType;
	char szTarget[MAX_NAME*MAX_MAIL_RECEIVER];//5명
	char szSender[MAX_NAME];
	char szTitle[60];
	char szBody[MAX_MAIL_BODY_LEN];
}t_MailSend,*LPMAILSEND;

const int CMD_MAIL_LIST_FROM_DBDEMON = 16100;
typedef struct MAILLISTRESULT
{
	char szName[MAX_NAME];
	int iWantViewType;
	int iWantViewPage;
	MAILDATA MailData[MAX_MAIL_PAGE];
}t_MailListResult,*LPMAILLISTRESULT;

const int CMD_MAIL_DELETE	=10143;	// 메일 삭제
const int CMD_MAIL_REPAIR	=10145;	// 메일 삭제
typedef struct MAILDELETE
{
	int cn;
	char szName[MAX_NAME];
	int aiMailIndex[MAX_MAIL_PAGE];
}t_MailDelete,*LPMAILDELETE;


#define	CMD_REQ_RECV_MAIL_TITLE_GUILD		10357
#define CMD_REQ_RECV_MAIL_BODY_GUILD		10359
#define CMD_UPDATE_GUILD_MAIL_ID			10360

#define CMD_RECV_MAIL_DELETE_GUILD			10361
#define CMD_REQ_RECV_MAIL_BODY				10144	// 메일 내용 요청


#define CMD_REP_RECV_MAIL_BODY				10146
#define	CMD_RECV_MAIL_DELETE				10147

//< CSD-030808
#define CMD_SEND_ACCEL_TYPE	6061
typedef struct tag_ACCEL_TYPE
{
	int		nType;
	char	strMsg[64];
}t_ACCEL_TYPE;
//> CSD-030808

typedef struct HWOARANG_PACKET
{
	union			
	{
		char	data[MAX_PACKET_SIZE];
		t_SkillMasterMultiRareMake	SMMultiRareMake;
		t_ExchangeBondMoney			ExBondMoney;
		t_SelectedCharResult		SelectedCharResult;
		t_MapMoveLvLimit			MapMoveLvLimit;
		t_TradeModeChange			TradeModeChange;
		t_ConnectAgentServerConform	ConnectAgentServerConform; //030325 태국과금
	
		t_ReqestMailList			ReqestMailList;
		t_MailListResult			MailListResult;
		t_MailSend					MailSend;
		t_MailDelete				MailDelete;
		t_ReqestMailBody			ReqestMailBody;
		t_ReqestMailBodyResult		ReqestMailBodyResult;		
	}rang;
}t_Hwoarang;