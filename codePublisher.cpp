#include"reuse.h"



int OSPL_MAIN(int argc, char *argv[])
{
	reuse reuser;
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

	reuser.createConnection("codeTransform", "code_Msg");
	reuser.pubMsg(coderID, fileName, uploadTime, codeContent);
	//pubMsg(1, "helloWorld.java", "2018/10/30", "Hello,World!");
	reuser.endDelete();

	//printf("\n\n");
	//coderID = 2;
	//pubMsg(coderID, fileName, uploadTime, codeContent);
	return 0;
}
