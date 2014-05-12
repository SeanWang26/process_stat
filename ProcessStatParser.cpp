#include "ProcessStatParser.h"
#include <stdio.h>
#include <stdlib.h>	

#include <sys/param.h>		/* for HZ */
#include <string.h>
#include <vector>
#include <string>
#include <sys/time.h>


char *strsep_s(char **stringp, const char *delim)
{
	char *tok;
	while(tok = strsep(stringp, delim))
	{
		if(strlen(tok))
			return tok;
	}

	return NULL;
}


SystemStatParser::SystemStatParser()
{

}
SystemStatParser::~SystemStatParser()
{


}
int SystemStatParser::Init()
{

}


ProcessStatParser::ProcessStatParser()
{

}
ProcessStatParser::~ProcessStatParser()
{

}
int ProcessStatParser::AddPid(int Pid)
{
	char path[256] = "";
	sprintf(path, "/proc/%d/stat", Pid);
	FILE *fpstat = fopen(path, "r");
	if(fpstat==NULL)
	{
		printf("[ProcessStatParser::Init]fp==NULL %d\n", Pid);
		return 0;
	}
	
	char s[1024]={0};
	if(fgets(s, 1024, fpstat)==NULL)
	{
		printf("[ProcessStatParser::Init]fgets %d\n", Pid);
		fclose(fpstat);
		return 0;
	}
	
	fclose(fpstat);

	printf("%s\n", s);

	std::vector<std::string> statlines;
	char * s1 = s;
	char * t;
	while(t = strsep_s(&s1, " ()"))
	{
		statlines.push_back(t);
	}

	stProcessStat ProcessStat;
	ProcessStat.m_Status = statlines[2][0];
	if(ProcessStat.m_Status=='Z')//僵尸进程了
	{
		printf("[ProcessStatParser::Init]zombie\n");
	}
	
	char* p = (char*)statlines[13].c_str();
	ProcessStat.m_UTime = strtoul(p, 0, 10);
	p = (char*)statlines[14].c_str();
	ProcessStat.m_STime = strtoul(p, 0, 10);

	m_ProcessStatS[Pid] = ProcessStat;
	return 0;
}
int ProcessStatParser::RemovePid(int Pid)
{
	m_ProcessStatS.erase(Pid);
	return 0;
}

int ProcessStatParser::Reflush()
{	
	struct timeval ThisTime, TimeDiff;
	gettimeofday(&ThisTime, 0);
	timersub(&ThisTime, &m_OTime, &TimeDiff);
	m_OTime = ThisTime;

	unsigned long elapsed = TimeDiff.tv_sec * HZ + (TimeDiff.tv_usec * HZ) / 1000000;
    if (elapsed <= 0)
	{
		elapsed = 1;
	}

	char path[256] = "";
	for(std::map<int,stProcessStat>::iterator iter = m_ProcessStatS.begin(); iter!=m_ProcessStatS.end(); ++iter)
	{
		sprintf(path, "/proc/%d/stat", iter->first);
		FILE *fpstat = fopen(path, "r");
		if(fpstat==NULL)
		{
			printf("[ProcessStatParser::Init]fp==NULL %d\n", iter->first);

			//检查存在否，不存在，删除
			continue;
		}

		char s[1024]={0};
		if(fgets(s, 1024, fpstat)==NULL)
		{
			printf("[ProcessStatParser::Init]fgets %d\n", iter->first);
			fclose(fpstat);
			continue;
		}
		
		fclose(fpstat);

		printf("%s\n", s);
		
		std::vector<std::string> statlines;
		char * s1 = s;
		char * t;
		while(t = strsep_s(&s1, " ()"))
		{
			statlines.push_back(t);
		}	

		stProcessStat ProcessStat;

		ProcessStat.m_Name = statlines[1];

		ProcessStat.m_Status = statlines[2][0];

		if(ProcessStat.m_Status=='Z')//僵尸进程了
		{
			printf("[ProcessStatParser::Init]zombie\n");
		}

		char* p = (char*)statlines[13].c_str();
		ProcessStat.m_UTime = strtoul(p, 0, 10);
		p = (char*)statlines[14].c_str();
		ProcessStat.m_STime = strtoul(p, 0, 10);

		printf("[ProcessStatParser::Reflush]m_UTime %ul, m_STime %ul\n", ProcessStat.m_UTime, ProcessStat.m_STime);

		unsigned long TotalTimeUsing = (ProcessStat.m_STime+ProcessStat.m_UTime)- (iter->second.m_STime+iter->second.m_UTime);
	    if((iter->second.m_PCpu = (TotalTimeUsing / (double)elapsed)) < 0.0001)
		{
			iter->second.m_PCpu = 0;
		}

		iter->second.m_Name = ProcessStat.m_Name;
		iter->second.m_UTime = ProcessStat.m_UTime;
		iter->second.m_STime = ProcessStat.m_STime;

		printf("[ProcessStatParser::Reflush]name %s, pid %d, per %f, Stat %c\n"
			, iter->second.m_Name.c_str(), iter->first, iter->second.m_PCpu, iter->second.m_Status);
	}
	
	return 0;
}

int ProcessStatParser::GetStatInfo(int Pid, stProcessStat &Info)
{
	std::map<int,stProcessStat>::iterator iter = m_ProcessStatS.find(Pid);
	if(iter!=m_ProcessStatS.end())
	{
		Info = iter->second;
		return 0;
	}

	return -1;
}




















