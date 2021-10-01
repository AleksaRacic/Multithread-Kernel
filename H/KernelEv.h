#ifndef KERNELEV_H_
#define KERNELEV_H_

class PCB;
typedef unsigned char IVTNo;

class KernelEv{
public:
	KernelEv(IVTNo);
	~KernelEv();

	void wait();
	void signal();
private:
	int entryNum;
	PCB* myPCB;
};

#endif /* KERNELEV_H_ */
