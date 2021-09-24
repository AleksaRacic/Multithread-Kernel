#include "thread.h"

extern int synchronizedPrintf(const char *format, ...);

int n = 10;

class tr : public Thread{
public:
	tr() : Thread(){}
	virtual ~tr(){}
	void run(){
		ID father;
		for (int i = 0; i<n; i++){
			father = getRunningId();
			ID ret = Thread::fork();
			if(ret == 0){
				synchronizedPrintf("me id: %d, my father %d\n", getRunningId(), father);
			}else if(ret == -1){
				synchronizedPrintf("fail\n");
			}else{
				synchronizedPrintf("son created, id:%d\n", ret);
			}
		}
		Thread::waitForForkChildren();
		synchronizedPrintf("\nFinn, id:%d\n\n", getRunningId());
	}
	Thread* clone() const {
		return new tr();
	}
};

int userMain(int h, char** argc){
	tr* t = new tr();
	t->start();
	t->waitToComplete();
	return 0;
}
