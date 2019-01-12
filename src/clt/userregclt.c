/**
 * Copyright (C) 2018-2019, Einars Leckis - All Rights Reserved.
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

#include <ndebug.h>
#include <atmi.h>


#include <ubf.h>
#include <Exfields.h>
#include <userreg.fd.h>
#include <nstopwatch.h>
#include <domain.h>
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/

#ifndef SUCCEED
#define SUCCEED			0
#endif

#ifndef	FAIL
#define FAIL			-1
#endif



/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/


/**
 * Initialise the application
 * @param argc	argument count
 * @param argv	argument values
 * @return SUCCEED/FAIL
 */
int init(int argc, char** argv)
{
	int ret = SUCCEED;

	TP_LOG(log_info, "Initializing...");

	if (SUCCEED!=tpinit(NULL)){
		TP_LOG(log_error, "Failed to Initialise: %s", tpstrerror(tperrno));
		ret = FAIL;
		goto out;
	}
	


out:

	
	return ret;
}

/**
 * Terminate the application
 */
int uninit(int status)
{
	int ret = SUCCEED;
	
	TP_LOG(log_info, "Uninitialising...");
	
	ret = tpterm();
	
	return ret;
}

/**
 * Do processing (call some service)
 * @return SUCCEED/FAIL
 */
int process (char proctype)
{
	int ret = SUCCEED;
	long rsplen;
	Message_t msg;
	char *buf = NULL;
	long len;
	ndrx_stopwatch_t timer;
	double d;
	double milis;
	int i, j;
	char *svcname =  msg_service();
	
	memset(&msg, 0, sizeof(msg));
	
	/* Fill customer details */
	strcpy(msg.cust.name,"Einars");
	strcpy(msg.cust.surname,"Leckis");
	strcpy(msg.cust.code,"101288-11141");
	msg.cust.birthDate = 10121988;
	strcpy(msg.cust.gender,"M");
	strcpy(msg.cust.natoinality,"Latvian");
	strcpy(msg.cust.country,"Latvia");
	strcpy(msg.cust.city,"Riga");
	strcpy(msg.cust.addr,"Avotu iela 22 - 12");
	strcpy(msg.cust.family,"Not married");
	msg.cust.childs=0;
	strcpy(msg.cust.docNo,"AA100129911LX");
	strcpy(msg.cust.telNo,"+37122022012");
	strcpy(msg.cust.email,"einars.leckis@test.lv");

	/* Fill employee data */
	strcpy(msg.emp.empname,"Juris");
	strcpy(msg.emp.empsurname,"Parsla");
	strcpy(msg.emp.empcode,"JP1201221UYX2AA");
	strcpy(msg.emp.empnotes,"Agreement No.121418u8232ds and attachment RF1121A");
	
	

	for(i=0; i<MAX_ARR; i+=50)
	{
		if ('D' == proctype)
		{
			msg.cust.pyldnum_items=i;
			for(j=0;j<i;j++)	
			{
				msg.cust.pyldnum[j]=MAX_ARR-i;
			}
		} else if ('S' == proctype)
		{
			msg.cust.pyldstr_items=i;
			for(j=0;j<i;j++)
			{
				sprintf(msg.cust.pyldstr[j],"String with number %d. "
                                        "This is test", MAX_ARR-i);
			}
		}
		
		ndrx_stopwatch_reset(&timer);
		
		for(j=0;j<10000;j++)
		{
                        msg.rsp.rspstatus = 0;
                        msg.rsp.rsprspmsg[0] = 0;
			if (SUCCEED != msg_build(&msg, &buf, &len))
			{
				TP_LOG(log_error, "TESTERROR: Failed build msg");
				ret=FAIL;
				goto out;	
			}
			
			if (FAIL==tpcall(svcname, buf, 0L, &buf, 
				&rsplen, TPNOTIME))
			{
				TP_LOG(log_error, "TPCALL: %s failed: %s", 
					svcname, tpstrerror(tperrno));
				ret=FAIL;
				goto out;
			}
			
			if(SUCCEED != parse_msg(&msg, buf, rsplen))
			{
				TP_LOG(log_error, "TESTERROR: Failed parse msg");
				ret=FAIL;
				goto out;
			}
			
			if(msg.rsp.rspstatus != 100)
			{
				TP_LOG(log_error, "TESTERROR: Invalid status, "
					"expected [100] got [%d]", 
					msg.rsp.rspstatus);
				ret=FAIL;
				goto out;
			}
			
			if(0!=strcmp(msg.rsp.rsprspmsg, "User successfully "
				"registered"))
			{
				TP_LOG(log_error, "TESTERROR: Invalid response "
					"msg, expected "
					"[User successfully registered] got [%s]", 
					msg.rsp.rsprspmsg);
				ret=FAIL;
				goto out;
			}
			
			if(NULL!=buf)
			{
				free(buf);
                                buf = NULL;
			}
		}
		
		milis = (double)ndrx_stopwatch_get_delta(&timer);
		
		if (EXSUCCEED!=ndrx_bench_write_stats((double)i, milis))
		{
			TP_LOG(log_always, "Failed to write stats!");
			ret=FAIL;
			goto out;
		}
	}
	
out:


	/* free data... */
	if (NULL!=buf)
	{
		free(buf);
	}
	
	return ret;
}

/**
 * Main entry of th program
 * @param argc	argument count
 * @param argv	argument values
 * @return SUCCEED/FAIL
 */
int main(int argc, char** argv)
{
	int ret = SUCCEED;
	
	if(argc!=2)
	{
		fprintf(stderr, "Usage %s [C/D]\n", argv[0]);
		exit(-1);
	}

	if (SUCCEED!=init(argc, argv)){
		TP_LOG(log_error, "Failed to Initialise!");
		ret=FAIL;
		goto out;
	}
	
	
	if (SUCCEED!=process(argv[1][0])){
		TP_LOG(log_error, "Process failed!");
		ret=FAIL;
		goto out;
	}
    
out:
	uninit(ret);

	return ret;
}

