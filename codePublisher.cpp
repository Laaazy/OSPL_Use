
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

/* entry point exported and demangled so symbol can be found in shared library */
extern "C"
{
	OS_API_EXPORT
		int HelloWorldDataPublisher(int argc, char *argv[]);
}

int CodeDataPublisher(int argc, char *argv[])
{
	os_time delay_1s = { 1, 0 };
	DDSEntityManager mgr;

	// create domain participant
	mgr.createParticipant("CodeTransform");//此处设置了域为默认域，并且设置了分区

	//create type
	CodeTypeSupport_var mt = new CodeTypeSupport();
	mgr.registerType(mt.in());

	//create Topic
	char topic_name[] = "CodeData_Msg";
	mgr.createTopic(topic_name);//创建发布订阅的主题

	//create Publisher
	mgr.createPublisher();

	// create DataWriter :
	// If autodispose_unregistered_instances is set to true (default value),
	// you will have to start the subscriber before the publisher
	bool autodispose_unregistered_instances = false;
	mgr.createWriter(autodispose_unregistered_instances);

	// Publish Events
	DataWriter_var dwriter = mgr.getWriter();
	CodeDataWriter_var CodeWriter = CodeDataWriter::_narrow(dwriter.in());

	Code msgInstance;//定义自己的消息实体
	msgInstance.coderID = 2;
	msgInstance.fileName = DDS::string_dup("HelloWorld.java");//使用DDS封装过的字符串
	msgInstance.uploadTime = DDS::string_dup("2018/10/25");
	//此处代码的输出格式可能需要注意，从文件读取的字符串是否包含格式信息？
	msgInstance.codeContent = DDS::string_dup("public class Test{\n\tpublic static void main(string[] args) {\n\t\tsystem.out.println(\"HelloWorld!\");\n\t\treturn 0;\n\t}\n}");
	cout << "=== [Publisher] writing a message containing :" << endl;
	cout << "    coderID  : " << msgInstance.coderID << endl;
	cout << "    fileName : \"" << msgInstance.fileName << "\"" << endl;
	cout << "    uploadTime : \"" << msgInstance.uploadTime << "\"" << endl;
	cout << "    codeContent : \"\n" << msgInstance.codeContent << "\"" << endl;

	ReturnCode_t status = CodeWriter->write(msgInstance, DDS::HANDLE_NIL);//发送数据，并返回状态用于检测是否成功
	checkStatus(status, "MsgDataWriter::write");
	os_nanoSleep(delay_1s);

	/* Remove the DataWriters */
	mgr.deleteWriter();

	/* Remove the Publisher. */
	mgr.deletePublisher();

	/* Remove the Topics. */
	mgr.deleteTopic();

	/* Remove Participant. */
	mgr.deleteParticipant();

	return 0;
}

int OSPL_MAIN(int argc, char *argv[])
{
	return CodeDataPublisher(argc, argv);
}
