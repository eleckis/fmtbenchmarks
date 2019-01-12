/**
 * Copyright (C) 2018-2019, Einars Leckis - All Rights Reserved.
 */
#include <domain.h>
#include <stddef.h>
#include <ndrstandard.h>
#include <ndebug.h>
#include <atmi.h>
#include <ubf.h>
#include <userreg.fd.h>

/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/

#ifdef SYS64BIT
#define OFFSZ(STRUCT,ELM)   ((long) &(((STRUCT *)0)->ELM) )
#else
#define OFFSZ(STRUCT,ELM)   ((const int) &(((STRUCT *)0)->ELM) )
#endif

/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/

typedef struct
{
    BFLDID fid;       /**< UBF field id                                 */
    long    msgoffs;    /**< sub-struct offset in message struct        */
    long    elmoffs;    /**< element offset in sub-struct               */
    long    itmoffs;    /**< item offset in structure for array types   */
    int     elmtyp;     /**< element type to parse/build                */
} msgbuilder_t;

/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/

static msgbuilder_t M_msgflds[] = 
{
    /* customer mappings */
    {U_NAME,    OFFSZ(Message_t, cust), OFFSZ(Customer_t, name), 0, MSG_STRING},
    {U_SURNAME, OFFSZ(Message_t, cust), OFFSZ(Customer_t, surname), 0, MSG_STRING},
    {U_PERS_CODE,    OFFSZ(Message_t, cust), OFFSZ(Customer_t, code), 0, MSG_STRING},
    {U_BIRTH_DATE,OFFSZ(Message_t, cust), OFFSZ(Customer_t, birthDate), 0, MSG_LONG},
    {U_GENDER,  OFFSZ(Message_t, cust), OFFSZ(Customer_t, gender), 0, MSG_STRING},
    {U_NATIONALITY,OFFSZ(Message_t, cust), OFFSZ(Customer_t, natoinality), 0, MSG_STRING},
    {U_COUNTRY, OFFSZ(Message_t, cust), OFFSZ(Customer_t, country), 0, MSG_STRING},
    {U_CITY,    OFFSZ(Message_t, cust), OFFSZ(Customer_t, city), 0, MSG_STRING},
    {U_ADRESS,    OFFSZ(Message_t, cust), OFFSZ(Customer_t, addr), 0, MSG_STRING},
    {U_FAMILY_STATE,  OFFSZ(Message_t, cust), OFFSZ(Customer_t, family), 0, MSG_STRING},
    {U_CHILDS,  OFFSZ(Message_t, cust), OFFSZ(Customer_t, childs), 0, MSG_SHORT},
    {U_DOC_NR,   OFFSZ(Message_t, cust), OFFSZ(Customer_t, docNo), 0, MSG_STRING},
    {U_TEL_NR,   OFFSZ(Message_t, cust), OFFSZ(Customer_t, telNo), 0, MSG_STRING},
    {U_EMAIL,   OFFSZ(Message_t, cust), OFFSZ(Customer_t, email), 0, MSG_STRING},
    {U_PYLDNUM,OFFSZ(Message_t, cust), OFFSZ(Customer_t, pyldnum), 
                OFFSZ(Customer_t, pyldnum_items), MSG_ARRAY_SHORT},
    {U_PYLDSTR, OFFSZ(Message_t, cust), OFFSZ(Customer_t, pyldstr), 
                OFFSZ(Customer_t, pyldstr_items), MSG_ARRAY_STRING},
    /* employee mappings */
    {E_NAME, OFFSZ(Message_t, emp), OFFSZ(Employee_t, empname), 0, MSG_STRING},
    {E_SURNAME, OFFSZ(Message_t, emp), OFFSZ(Employee_t, empsurname), 0, MSG_STRING},
    {E_IDENT, OFFSZ(Message_t, emp), OFFSZ(Employee_t, empcode), 0, MSG_STRING},
    {E_NOTES,OFFSZ(Message_t, emp), OFFSZ(Employee_t, empnotes), 0, MSG_STRING},
    
    /* response mappings */
    {S_REG_MSG,OFFSZ(Message_t, rsp), OFFSZ(Response_t, rsprspmsg), 0, MSG_STRING},
    {S_REG_STATUS,OFFSZ(Message_t, rsp), OFFSZ(Response_t, rspstatus), 0, MSG_SHORT},
    {BBADFLDID}
};


/*---------------------------Prototypes---------------------------------*/

char *msg_service(void)
{
	static char svc[] = "USERREG_UBF";
	return svc;
}

/**
 * Build outgoing message
 * @param[in] msg full message to send
 * @param[out] outbuf output buffer/XATMI allocated
 * @param[out] olen output buffer len
 * @return SUCCEED/FAIL
 */
int msg_build(Message_t *msg, char **outbuf, long *olen)
{
    int ret = SUCCEED;
    msgbuilder_t *p = M_msgflds;
    short *p_short;
    void *fld_ptr;
    char *p_string_el;

    short *p_items;
    int i;
    int		size = 0;
    UBFH *p_ub;
    Bfld_loc_info_t next_fld;
    
    /* Alloc STRING into obuf */
    if (NULL==*outbuf)
    {
        *outbuf = tpalloc("UBF", NULL, MAX_BUFSZ);
    }
    
    if (NULL==*outbuf)
    {
       TP_LOG(log_error, "Failed to alloc %d bytes: %s", 
			MAX_BUFSZ, tpstrerror(tperrno)); 
       ret=FAIL;
       goto out;
    }
    
    /* start UBF */
    p_ub = (UBFH *)*outbuf;
    while (BBADFLDID!=p->fid)
    {
        fld_ptr = (char *)msg + p->msgoffs + p->elmoffs;
                
        switch (p->elmtyp)
        {
            /* data type matches C and fid thus single change/add func */
            case MSG_SHORT:
            case MSG_LONG:    
            case MSG_STRING:
                
                next_fld.last_checked = NULL;
                if (SUCCEED!=Bchg(p_ub, p->fid, 0, (char *)fld_ptr, 0L))
                {
                    TP_LOG(log_error, "Failed to set field %d/%s: %s", 
                            p->fid, Bfname(p->fid), Bstrerror(Berror));
                    ret=FAIL;
                    goto out;
                }
                
                break;
            case MSG_ARRAY_SHORT:
                
                next_fld.last_checked = NULL;
                p_items = (short *)((char *)msg + p->msgoffs + p->itmoffs);
                
                for (i=0; i<*p_items; i++)
                {
                    p_short = (short *)( (char *)fld_ptr + i*sizeof(short));
                    
                    if (SUCCEED!=Bchg(p_ub, p->fid, i, (char *)p_short, 0L))
                    {
                        TP_LOG(log_error, "Failed to set field %d/%s: %s", 
                                p->fid, Bfname(p->fid), Bstrerror(Berror));
                        ret=FAIL;
                        goto out;
                    }
                }
                
                break;
            case MSG_ARRAY_STRING:
                
                p_items = (short *)((char *)msg + p->msgoffs + p->itmoffs);
                
                for (i=0; i<*p_items; i++)
                {
                    /* calculate string array element location */
                    p_string_el = (char *)fld_ptr + i*MAX_STR;
                    
                    if (SUCCEED!=Baddfast(p_ub, p->fid, p_string_el, 0L, &next_fld))
                    {
                        TP_LOG(log_error, "Failed to set field %d/%s: %s", 
                                p->fid, Bfname(p->fid), Bstrerror(Berror));
                        ret=FAIL;
                        goto out;
                    }
                }
                
                break;
            default:
                TP_LOG(log_error, "Unknown element type %d fid: [%d]!", 
                        p->elmtyp, (int)p->fid);
                ret=FAIL;
                goto out;
                break;
        }
        
        p++;
    }
    
    tplogprintubf(log_debug, "Got UBF", p_ub);
    
out:
    return ret;
}

/**
 * get parsing descriptor for tag
 * @param tag tag name
 * @return  descriptor or NULL
 */
static msgbuilder_t *get_tag(BFLDID fid)
{
    msgbuilder_t *p = M_msgflds;
            
    while (BBADFLDID!=p->fid)
    {
        if (p->fid == fid)
        {
            return p;
        }
        
        p++;
    }
    
    return NULL;
}

/**
 * Parse incoming message, send all data to msg
 * @param msg domain model msg
 * @param ibuf input buffer
 * @param ilen input buffer len
 * @return SUCCEED/FAIL
 */
int parse_msg(Message_t *msg, char *ibuf, long ilen)
{
    int ret = SUCCEED;
    msgbuilder_t *descr;
    short *p_short;
    long *p_long;
    void *fld_ptr;
    char *p_string_el;
    
    int len;
    BFLDID bfldid;
    UBFH *p_ub = (UBFH *)ibuf;
    BFLDOCC occ;
    char data_buf[MAX_STR]; /* this stores any bytes, typed incl short, long and string */
    
    /* start UBF parse */
    
    tplogprintubf(log_debug, "Got UBF for parse", p_ub);
    
    
    len = sizeof(data_buf);
    bfldid = BFIRSTFLDID;
    
    while(1==Bnext(p_ub, &bfldid, &occ, data_buf, &len))
    {
        
        TP_DUMP(log_debug, "Filed binary data", data_buf, len);
        TP_LOG(log_debug, "GOT field: %d: ", bfldid);
        
        /* find tag descriptor */
        if (NULL==(descr = get_tag(bfldid)))
        {
            TP_LOG(log_error, "Failed to get tag descr for [%d]", 
                    bfldid);
            ret = FAIL;
            goto out;
        }
        
        /* load the field into struct accordingly */
        fld_ptr = (char *)msg + descr->msgoffs + descr->elmoffs;
        
        switch (descr->elmtyp)
        {
            case MSG_SHORT:
                p_short =  (short *)fld_ptr;
                *p_short = *((short *)data_buf);
                break;
            case MSG_LONG:
                p_long =  (long *)fld_ptr;
                *p_long = *((long *)data_buf);
                break;
            case MSG_STRING:
                strcpy((char *)fld_ptr, data_buf);
                break;
            case MSG_ARRAY_SHORT:
                
                p_short = (short *)( (char *)fld_ptr + occ*sizeof(short));
                *p_short = *((short *)data_buf);
                
                break;
            case MSG_ARRAY_STRING:
                
                p_string_el = ( (char *)fld_ptr + occ*MAX_STR);
                strcpy(p_string_el, data_buf);
                
                break;
            default:
                TP_LOG(log_error, "Unknown element type %d!", descr->elmtyp);
                ret=FAIL;
                goto out;
                break;
        }
        
        len = sizeof(data_buf);
    }
    
out:

    return ret;
}
