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
 * Service entry
 * @return SUCCEED/FAIL
 */
void USERREGSV (TPSVCINFO *p_svc)
{
    int ret = SUCCEED;
    int rsp = 0;
    Message_t msg;
    long len;
    char * buf = p_svc->data;

    memset(&msg, 0, sizeof(msg));
    
    /* allocate some stuff for more data to put in  */
    
    len = tptypes(p_svc->data, NULL, NULL) + 1024;
    
    if (NULL==(buf = tprealloc(buf, len)))
    {
	TP_LOG(log_error, "Failed reallocate buffer to size %d: %s", 
                len, tpstrerror(tperrno));
	ret=FAIL;
	goto out;
    }

    if (SUCCEED != parse_msg(&msg, buf, len))
    {
	TP_LOG(log_error, "Failed to parse msg");
	ret=FAIL;
	goto out;
	    
    }
    
    msg.rsp.rspstatus = 100;
    strcpy(msg.rsp.rsprspmsg, "User successfully registered");
    
    if(SUCCEED != msg_build(&msg, &buf, &len))
    {
	TP_LOG(log_error, "Failed to build msg");
	ret=FAIL;
	goto out;
    }

out:

    tpreturn(  ret==SUCCEED?TPSUCCESS:TPFAIL,
            0L,
            buf,
            0L,
            0L);


}

/**
 * Initialize the application
 * @param argc	argument count
 * @param argv	argument values
 * @return SUCCEED/FAIL
 */
int init(int argc, char** argv)
{
	int ret = SUCCEED;

	TP_LOG(log_info, "Initialising...");

	if (SUCCEED!=tpinit(NULL))
	{
		TP_LOG(log_error, "Failed to Initialise: %s", 
			tpstrerror(tperrno));
		ret = FAIL;
		goto out;
	}
	

	
	/* Advertise our service */
	if (SUCCEED!=tpadvertise(msg_service(), USERREGSV))
	{
		TP_LOG(log_error, "Failed to initialise USERREGSV_UBF!");
		ret=FAIL;
		goto out;
	}	

out:

	
	return ret;
}

/**
 * Terminate the application
 */
void uninit(void)
{
	TP_LOG(log_info, "Uninitialising...");
}

/**
 * Server program main entry
 * @param argc	argument count
 * @param argv	argument values
 * @return SUCCEED/FAIL
 */
int main(int argc, char** argv)
{
	/* Launch the Enduro/x thread */
	return ndrx_main_integra(argc, argv, init, uninit, 0);
}

