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

	unsigned long m_UTime; //用户空间使用的时间
	unsigned long m_STime; //系统空间使用的时间

	unsigned long m_StartTime;//进程开始时间

	double m_PCpu;
};

class SystemStatParser
{
private:
	
	unsigned long m_UTime; //用户空间使用的时间
	unsigned long m_STime; //系统空间使用的时间
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

