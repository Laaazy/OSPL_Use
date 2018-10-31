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

	//create Subscriber
	mgr.createSubscriber();

	// create DataReader
	mgr.createReader();
	return;
}

void reuse::subMsg() {
	os_time delay_2ms = { 0, 2000000 };
	os_time delay_200ms = { 0, 200000000 };
	CodeSeq msgList;
	SampleInfoSeq infoSeq;

	DataReader_var dreader = mgr.getReader();
	CodeDataReader_var HelloWorldReader = CodeDataReader::_narrow(dreader.in());
	checkHandle(HelloWorldReader.in(), "MsgDataReader::_narrow");

	cout << "=== [Subscriber] Ready ..." << endl;

	bool closed = false;
	ReturnCode_t status = -1;
	int count = 0;
	//接收1500次,!closed && count < 1500
	while (!closed && count < 1500) // We dont want the example to run indefinitely
	{
		status = HelloWorldReader->take(msgList, infoSeq, LENGTH_UNLIMITED,
			ANY_SAMPLE_STATE, ANY_VIEW_STATE, ANY_INSTANCE_STATE);
		checkStatus(status, "msgDataReader::take");
		for (DDS::ULong j = 0; j < msgList.length(); j++)
		{
			cout << "=== [Subscriber] message received :" << endl;
			cout << "    coderID  : " << msgList[j].coderID << endl;
			cout << "    fileName : \"" << msgList[j].fileName << "\"" << endl;
			cout << "    uploadTime : \"" << msgList[j].uploadTime << "\"" << endl;
			cout << "    codeContent : \"" << msgList[j].codeContent << "\"" << endl;
			closed = true;
		}
		status = HelloWorldReader->return_loan(msgList, infoSeq);
		checkStatus(status, "MsgDataReader::return_loan");
		os_nanoSleep(delay_200ms);
		++count;
	}
	os_nanoSleep(delay_2ms);
	return;
}

void reuse::endDelete() {
	//cleanup
	mgr.deleteReader();
	mgr.deleteSubscriber();
	mgr.deleteTopic();
	mgr.deleteParticipant();
	return;
}

reuse::~reuse() {

}
