#include"reuse.h"


int OSPL_MAIN(int argc, char *argv[])
{
	reuse reuser;
	reuser.createConnection("codeTransform", "code_Msg");
	reuser.subMsg();
	reuser.endDelete();
	return 0;
}
