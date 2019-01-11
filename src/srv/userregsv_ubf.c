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
void USERREGSV_UBF (TPSVCINFO *p_svc)
{
    int ret = SUCCEED;
    int rsp = 0;


    UBFH *p_ub = (UBFH *)p_svc->data;

    tplogprintubf(log_info, "Got request", p_ub);


    /* allocate some stuff for more data to put in  */
    if (NULL==(p_ub = (UBFH *)tprealloc((char *)p_ub, 4096)))
    {
        ret=FAIL;
        goto out;
    }

    /* Read fields sent by server and check dynamically for
     * values that were sent by clt. If some field does not match
     * set S_REG_MSG field and skip other field checking 
     */

    /* If all fields were OK, then set response */
    if (SUCCEED == ret)
    {
        if (SUCCEED!=Badd(p_ub, S_REG_STATUS, (char *)&rsp, 0L))
        {
                TP_LOG(log_error, "Failed to set S_REG_STATUS: %s",  
                      Bstrerror(Berror));
                ret=FAIL;
                goto out;
        }
    }

out:

    tpreturn(  ret==SUCCEED?TPSUCCESS:TPFAIL,
            0L,
            (char *)p_ub,
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
	if (SUCCEED!=tpadvertise("USERREGSV_UBF", USERREGSV_UBF))
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

