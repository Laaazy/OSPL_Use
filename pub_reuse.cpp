#include "reuse.h"


void reuse::createConnection(const char* partition, char *topic)//参数1：分区名；参数2：主题名
{
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

void reuse::pubMsg(int coderID, const char* fileName, const char* uploadTime, const char* codeContent) {
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

void reuse::endDelete() {
	/* Remove the DataWriters */
	mgr.deleteWriter();

	/* Remove the Publisher. */
	mgr.deletePublisher();

	/* Remove the Topics. */
	mgr.deleteTopic();

	/* Remove Participant. */
	mgr.deleteParticipant();

	return;
}

reuse::~reuse() {

}
