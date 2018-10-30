#include <string>
#include <sstream>
#include <iostream>
#include "DDSEntityManager.h"
#include "vortex_os.h"
#include "ccpp_code.h"//记得使用自己的ccpp
//#include "D:\DDS\HDE\x86_64.win64\examples\include\example_main.h"
#include "example_main.h"//此处记得将OSPL的example_main.h文件拷贝到当前项目下
//#include"reuse.h"

using namespace DDS;
using namespace CodeData;//此处也是使用根据自己定义的IDL生成的命名空间


extern "C"
{
	OS_API_EXPORT
		int HelloWorldDataPublisher(int argc, char *argv[]);
}
DDSEntityManager mgr;//全局变量



//创建连接，只在第一次发送时创建，之后每次发送不再创建
//参数1：分区名；参数2：主题名
void createConnection(const char* partition,char *topic) {
	
	// create domain participant
	mgr.createParticipant(partition);//此处设置了域为默认域，并且设置了分区

	//create type
	CodeTypeSupport_var mt = new CodeTypeSupport();
	mgr.registerType(mt.in());

	//create Topic
	//char *topic_name = topic;
	mgr.createTopic(topic);//创建发布订阅的主题

	//create Publisher
	mgr.createPublisher();

	// create DataWriter :
	// If autodispose_unregistered_instances is set to true (default value),
	// you will have to start the subscriber before the publisher
	bool autodispose_unregistered_instances = false;
	mgr.createWriter(autodispose_unregistered_instances);
	return;
}


//进行发布
void pubMsg(int coderID, const char* fileName, const char* uploadTime, const char* codeContent) {
	os_time delay_1s = { 1, 0 };
	DataWriter_var dwriter = mgr.getWriter();
	CodeDataWriter_var CodeWriter = CodeDataWriter::_narrow(dwriter.in());

	Code msgInstance;//定义自己的消息实体
	msgInstance.coderID = coderID;
	msgInstance.fileName = DDS::string_dup(fileName);//使用DDS封装过的字符串
	msgInstance.uploadTime = DDS::string_dup(uploadTime);//eg. "2018/10/25"
	//此处代码的输出格式可能需要注意，从文件读取的字符串是否包含格式信息？
	msgInstance.codeContent = DDS::string_dup(codeContent);
	cout << "=== [Publisher] writing a code_message containing :" << endl;
	cout << "    coderID  : " << msgInstance.coderID << endl;
	cout << "    fileName : \"" << msgInstance.fileName << "\"" << endl;
	cout << "    uploadTime : \"" << msgInstance.uploadTime << "\"" << endl;
	cout << "    codeContent : \"" << msgInstance.codeContent << "\"" << endl;

	ReturnCode_t status = CodeWriter->write(msgInstance, DDS::HANDLE_NIL);//发送数据，并返回状态用于检测是否成功
	checkStatus(status, "MsgDataWriter::write");
	os_nanoSleep(delay_1s);
	return;
}

//删除连接，只有在退出整个程序时才删除创建的域、主题等等
void endDelete() {
	// Remove the DataWriters 
	mgr.deleteWriter();

	// Remove the Publisher. 
	mgr.deletePublisher();

	// Remove the Topics. 
	mgr.deleteTopic();

	// Remove Participant. 
	mgr.deleteParticipant();

	return;
}


int OSPL_MAIN(int argc, char *argv[])
{
	//reuse reuser;
	int coderID;
	char fileName[20];
	char uploadTime[20];
	char codeContent[100000];//暂定10W大小的代码
	printf("请输入要发送的信息：\n");
	
	printf("代码编写人员：");
	scanf("%d", &coderID);
	
	printf("代码文件名：");
	scanf("%s", fileName);
	
	printf("代码上传时间：");
	scanf("%s", uploadTime);

	printf("代码内容：");
	scanf("%s", codeContent);
	printf("\n");

	createConnection("codeTransform", "code_Msg");
	pubMsg(coderID, fileName, uploadTime, codeContent);
	//pubMsg(1, "helloWorld.java", "2018/10/30", "Hello,World!");
	endDelete();

	//printf("\n\n");
	//coderID = 2;
	//pubMsg(coderID, fileName, uploadTime, codeContent);
	return 0;
}
