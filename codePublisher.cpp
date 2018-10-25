
#include <string>
#include <sstream>
#include <iostream>
#include "DDSEntityManager.h"
#include "vortex_os.h"

#include "ccpp_code.h"

#include "D:\DDS\HDE\x86_64.win64\examples\include\example_main.h"

using namespace DDS;
using namespace CodeData;
//using namespace HelloWorldData;

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
	mgr.createParticipant("CodeTransform");

	//create type
	CodeTypeSupport_var mt = new CodeTypeSupport();
	mgr.registerType(mt.in());

	//create Topic
	char topic_name[] = "CodeData_Msg";
	mgr.createTopic(topic_name);

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

	Code msgInstance; /* Example on Stack */
	msgInstance.coderID = 2;
	msgInstance.fileName = DDS::string_dup("HelloWorld.java");
	msgInstance.uploadTime = DDS::string_dup("2018/10/25");
	msgInstance.codeContent = DDS::string_dup("public class Test{\n\tpublic static void main(string[] args) {\n\t\tsystem.out.println(\"HelloWorld!\");\n\t\treturn 0;\n\t}\n}");
	cout << "=== [Publisher] writing a message containing :" << endl;
	cout << "    coderID  : " << msgInstance.coderID << endl;
	cout << "    fileName : \"" << msgInstance.fileName << "\"" << endl;
	cout << "    uploadTime : \"" << msgInstance.uploadTime << "\"" << endl;
	cout << "    codeContent : \"\n" << msgInstance.codeContent << "\"" << endl;

	ReturnCode_t status = CodeWriter->write(msgInstance, DDS::HANDLE_NIL);
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
