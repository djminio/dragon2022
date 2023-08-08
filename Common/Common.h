//Common.h
//本文件内容改动后,必须新编译AppDll的两个DLL和MainDll的两个DLL

//AppDll,MainDll释放到系统DLL目录下的名称定义
#define HOOKAPI_DLL_NAME	"mcirlecd.dll" //MainDll
#define MY_DLL_NAME			"mcirleda.dll" //AppDll

//共享内存的标志,一般不需改动
#define SM_SAFEHWND "DB3CAD38E"
#define SM_GAMEHWND "B4D3A76C9"
#define SM_SAFEMEMPATH "E59A3CDE8"

#define DELAYTIME 25

