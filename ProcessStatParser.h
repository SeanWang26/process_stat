#ifndef PROCESS_STAT_H
#define PROCESS_STAT_H

#include <map>
#include <time.h>
#include <string>

struct stProcessStat
{
	stProcessStat():m_Pid(0),m_Status(0),m_UTime(0),m_STime(0),m_StartTime(0),m_PCpu(0.0)
	{}

	std::string m_Name;
	int m_Pid;

	char m_Status; //'R' 'S' 'D' 'Z' 'T' 'W' 

	unsigned long m_UTime; //�û��ռ�ʹ�õ�ʱ��
	unsigned long m_STime; //ϵͳ�ռ�ʹ�õ�ʱ��

	unsigned long m_StartTime;//���̿�ʼʱ��

	double m_PCpu;
};

class SystemStatParser
{
private:
	
	unsigned long m_UTime; //�û��ռ�ʹ�õ�ʱ��
	unsigned long m_STime; //ϵͳ�ռ�ʹ�õ�ʱ��
	double m_PCpu;
	
public:
	SystemStatParser();
	~SystemStatParser();
	int Init();
};

class ProcessStatParser
{
private:
	struct timeval m_OTime;
	
	std::map<int,stProcessStat> m_ProcessStatS;

	unsigned long m_Elapsed;

public:
	ProcessStatParser();
	~ProcessStatParser();

	int AddPid(int Pid);
	int RemovePid(int Pid);

	int Reflush();
};





#endif

