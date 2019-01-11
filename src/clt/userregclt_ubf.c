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
struct Customer {
	char name[50];
	char surname[50];
	char code[13];
	long birthDate;
	char gender[1];
	char natoinality[30];
	char country[30];
	char city[30];
	char addr[150];
	char family[15];
	short childs;
	char docNo[50];
	char telNo[14];
	char email[50];
	char type[10];
	char additdata[255];
	char level[30];
	char rigths[50];
	short status;

};
struct Employee {
	char name[50];
	char surname[50];
	char code[30];
	char notes[255];
};
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
int process (void)
{
	int ret = SUCCEED;
	UBFH *p_ub = NULL;
	long rsplen;
	char retbuf[2]={0};
	short resp;
	BFLDLEN sz;
	struct Customer *cust = (struct Customer *)malloc(sizeof(*cust));
	struct Employee *emp = (struct Employee *)malloc(sizeof(*emp));
	long sysdate = 10122018;


	/* Fill customer details */
	strcpy(cust->name,"Einars");
	strcpy(cust->surname,"Leckis");
	strcpy(cust->code,"101288-11141");
	cust->birthDate = 10121988;
	strcpy(cust->gender,"M");
	strcpy(cust->natoinality,"Latvian");
	strcpy(cust->country,"Latvia");
	strcpy(cust->city,"Riga");
	strcpy(cust->addr,"Avotu iela 22 - 12");
	strcpy(cust->family,"Not married");
	cust->childs=0;
	strcpy(cust->docNo,"AA100129911LX");
	strcpy(cust->telNo,"+37122022012");
	strcpy(cust->email,"einars.leckis@test.lv");
	strcpy(cust->type,"Customer");
	strcpy(cust->level,"New");
	strcpy(cust->rigths,"1001001110111011110011100");
	strcpy(cust->additdata,"I do not want receive any news on email");

	/* Fill employee data */
	strcpy(emp->name,"Juris");
	strcpy(emp->surname,"Parsla");
	strcpy(emp->code,"JP1201221UYX2AA");
	strcpy(emp->notes,"Agreement No.121418u8232ds and attachment RF1121A");

	/* Call sample server... */
	
	TP_LOG(log_info, "Processing registration...");
	/* Loop registration process */
	for (int i = 0 ; i < 2000; i++)
	{
		/* Allocate UBF buffer */
		if (NULL==(p_ub = (UBFH *)tpalloc("UBF", NULL, 4096))){
			TP_LOG(log_error, "Failed to tpalloc: %s",  tpstrerror(tperrno));
			ret=FAIL;
			goto out;
		}

		/* Add fields to UBF */
		if (SUCCEED!=Badd(p_ub, U_NAME, cust->name, 0L)){
			TP_LOG(log_error, "Failed to set U_NAME: %s", Bstrerror(Berror));
			ret=FAIL;
			goto out;
		}

		if (SUCCEED!=Badd(p_ub, U_SURNAME, cust->surname, 0L)){
			TP_LOG(log_error, "Failed to set U_SURNAME: %s", Bstrerror(Berror));
			ret=FAIL;
			goto out;
		}

		if (SUCCEED!=Badd(p_ub, U_PERS_CODE, cust->code, 0L)){
			TP_LOG(log_error, "Failed to set U_PERS_CODE: %s", Bstrerror(Berror));
			ret=FAIL;
			goto out;
		}

		if (SUCCEED!=Badd(p_ub, U_BIRTH_DATE, (char *)&cust->birthDate, 0L)){
			TP_LOG(log_error, "Failed to set U_BIRTH_DATE: %s", Bstrerror(Berror));
			ret=FAIL;
			goto out;
		}

		if (SUCCEED!=Badd(p_ub, U_GENDER, cust->gender, 0L)){
			TP_LOG(log_error, "Failed to set U_GENDER: %s", Bstrerror(Berror));
			ret=FAIL;
			goto out;
		}

		if (SUCCEED!=Badd(p_ub, U_NATIONALITY, cust->natoinality, 0L)){
			TP_LOG(log_error, "Failed to set U_NATIONALITY: %s", Bstrerror(Berror));
			ret=FAIL;
			goto out;
		}

		if (SUCCEED!=Badd(p_ub, U_COUNTRY, cust->country, 0L)){
			TP_LOG(log_error, "Failed to set U_COUNTRY: %s", Bstrerror(Berror));
			ret=FAIL;
			goto out;
		}

		if (SUCCEED!=Badd(p_ub, U_CITY, cust->city, 0L)){
			TP_LOG(log_error, "Failed to set U_CITY: %s", Bstrerror(Berror));
			ret=FAIL;
			goto out;
		}

		if (SUCCEED!=Badd(p_ub, U_ADRESS, cust->addr, 0L)){
			TP_LOG(log_error, "Failed to set U_ADRESS: %s", Bstrerror(Berror));
			ret=FAIL;
			goto out;
		}

		if (SUCCEED!=Badd(p_ub, U_FAMILY_STATE, cust->family, 0L)){
			TP_LOG(log_error, "Failed to set U_FAMILY_STATE: %s", Bstrerror(Berror));
			ret=FAIL;
			goto out;
		}

		if (SUCCEED!=Badd(p_ub, U_CHILDS, (char *)&cust->childs, 0L)){
			TP_LOG(log_error, "Failed to set U_CHILDS: %s", Bstrerror(Berror));
			ret=FAIL;
			goto out;
		}

		if (SUCCEED!=Badd(p_ub, U_DOC_NR, cust->docNo, 0L)){
			TP_LOG(log_error, "Failed to set U_DOC_NR: %s", Bstrerror(Berror));
			ret=FAIL;
			goto out;
		}

		if (SUCCEED!=Badd(p_ub, U_TEL_NR, cust->telNo, 0L)){
			TP_LOG(log_error, "Failed to set U_TEL_NR: %s", Bstrerror(Berror));
			ret=FAIL;
			goto out;
		}

		if (SUCCEED!=Badd(p_ub, U_EMAIL, cust->email, 0L)){
			TP_LOG(log_error, "Failed to set U_EMAIL: %s", Bstrerror(Berror));
			ret=FAIL;
			goto out;
		}

		if (SUCCEED!=Badd(p_ub, U_TYPE, cust->type, 0L)){
			TP_LOG(log_error, "Failed to set U_TYPE: %s", Bstrerror(Berror));
			ret=FAIL;
			goto out;
		}

		if (SUCCEED!=Badd(p_ub, U_ADDITDATA, cust->additdata, 0L)){
			TP_LOG(log_error, "Failed to set U_ADDITDATA: %s", Bstrerror(Berror));
			ret=FAIL;
			goto out;
		}

		if (SUCCEED!=Badd(p_ub, U_LEVEL, cust->level, 0L)){
			TP_LOG(log_error, "Failed to set U_LEVEL: %s", Bstrerror(Berror));
			ret=FAIL;
			goto out;
		}

		if (SUCCEED!=Badd(p_ub, U_PERMISSIONS, cust->rigths, 0L)){
			TP_LOG(log_error, "Failed to set U_PERMISSIONS: %s", Bstrerror(Berror));
			ret=FAIL;
			goto out;
		}

		if (SUCCEED!=Badd(p_ub, U_STATUS, (char *)&cust->status, 0L)){
			TP_LOG(log_error, "Failed to set U_STATUS: %s", Bstrerror(Berror));
			ret=FAIL;
			goto out;
		}

		if (SUCCEED!=Badd(p_ub, S_DATE, (char *)&sysdate, 0L)){
			TP_LOG(log_error, "Failed to set S_DATE: %s", Bstrerror(Berror));
			ret=FAIL;
			goto out;
		}

		if (SUCCEED!=Badd(p_ub, S_UNIQUE_NR, "R91GGA7110A89121", 0L)){
			TP_LOG(log_error, "Failed to set S_UNIQUE_NR: %s", Bstrerror(Berror));
			ret=FAIL;
			goto out;
		}

		if (SUCCEED!=Badd(p_ub, S_REG_TYPE, "Filiale", 0L)){
			TP_LOG(log_error, "Failed to set S_REG_TYPE: %s", Bstrerror(Berror));
			ret=FAIL;
			goto out;
		}

		if (SUCCEED!=Badd(p_ub, S_DEVICE_NR, "PC-112", 0L)){
			TP_LOG(log_error, "Failed to set S_DEVICE_NR: %s", Bstrerror(Berror));
			ret=FAIL;
			goto out;
		}

		if (SUCCEED!=Badd(p_ub, S_APP_VERSION, "v5.121.29931", 0L)){
			TP_LOG(log_error, "Failed to set S_APP_VERSION: %s", Bstrerror(Berror));
			ret=FAIL;
			goto out;
		}

		if (SUCCEED!=Badd(p_ub, E_NAME, emp->name, 0L)){
			TP_LOG(log_error, "Failed to set E_NAME: %s",  
			      Bstrerror(Berror));
			ret=FAIL;
			goto out;
		}

		if (SUCCEED!=Badd(p_ub, E_SURNAME, emp->surname, 0L)){
			TP_LOG(log_error, "Failed to set E_SURNAME: %s", Bstrerror(Berror));
			ret=FAIL;
			goto out;
		}

		if (SUCCEED!=Badd(p_ub, E_IDENT, emp->code, 0L)){
			TP_LOG(log_error, "Failed to set E_IDENT: %s", Bstrerror(Berror));
			ret=FAIL;
			goto out;
		}

		if (SUCCEED!=Badd(p_ub, E_NOTES, emp->notes, 0L)){
			TP_LOG(log_error, "Failed to set E_NOTES: %s", Bstrerror(Berror));
			ret=FAIL;
			goto out;
		}

		/* Call registration service */
		if (FAIL==tpcall("USERREGSV_UBF", (char *)p_ub, 0L, (char **)&p_ub, &rsplen, TPNOTIME)){
			TP_LOG(log_error, "Failed to call USERREGSV_UBF: %s", tpstrerror(tperrno));
			ret=FAIL;
			goto out;
		}
		
		//tplogprintubf(log_debug, "Got response", p_ub);
		
		
		sz = sizeof(retbuf);
		if (SUCCEED!=CBget(p_ub, S_REG_STATUS, 0, (char *)&resp, &sz, BFLD_SHORT)){
			TP_LOG(log_error, "Failed to get S_REG_STATUS: %s", Bstrerror(Berror));
			ret=FAIL;
			goto out;
		}

		if (0 == resp) {
			TP_LOG(log_info, "Registration successful!");
		} else {
			/* Read S_REG_MSG and print in TP_LOG 
			TP_LOG(log_info, "Registration failed! %s", msg);
			*/
			goto out;
		}
		
		//TP_LOG(log_info, "Got response from server: [%s]", retbuf);

		if (NULL!=p_ub){
			tpfree((char *)p_ub);
		}
	}
out:


	/* free data... */
	if (NULL!=p_ub){
		tpfree((char *)p_ub);
	}

	free(cust);
	free(emp);
	
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

	if (SUCCEED!=init(argc, argv)){
		TP_LOG(log_error, "Failed to Initialise!");
		ret=FAIL;
		goto out;
	}
	
	
	if (SUCCEED!=process()){
		TP_LOG(log_error, "Process failed!");
		ret=FAIL;
		goto out;
	}
    
out:
	uninit(ret);

	return ret;
}

