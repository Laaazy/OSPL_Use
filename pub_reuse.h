/*
功能：创建分区、主题；发送数据；删除创建的分区、主题
当不再需要通信时再删除分区、主题，不必每次收发都重新创建分区、主题
*/

#ifndef REUSE
#define REUSE

#include <string>
#include <sstream>
#include <iostream>
#include "DDSEntityManager.h"
#include "vortex_os.h"
#include "ccpp_code.h"//记得使用自己的ccpp
//#include "D:\DDS\HDE\x86_64.win64\examples\include\example_main.h"
#include "example_main.h"//此处记得将OSPL的example_main.h文件拷贝到当前项目下

using namespace DDS;
using namespace CodeData;//此处也是使用根据自己定义的IDL生成的命名空间

extern "C"
{
	OS_API_EXPORT
		int HelloWorldDataPublisher(int argc, char *argv[]);
}

class reuse
{
	 DDSEntityManager mgr;
public:
	//创建连接，只在第一次发送时创建，之后每次发送不再创建
	void createConnection(const char* partition, char *topic);//参数1：分区名；参数2：主题名

	//进行发布
	void pubMsg(int coderID, const char* fileName, const char* uploadTime, const char* codeContent);

	//删除连接，只有在退出整个程序时才删除创建的域、主题等等
	void endDelete();
	~reuse();
};

#endif
