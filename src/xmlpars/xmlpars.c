#include <domain.h>
#include <stddef.h>
#include <ndrstandard.h>
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
    char tag[40];       /**< xml tag                                    */
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
    {"name",    OFFSZ(Message_t, cust), OFFSZ(Customer_t, name), 0, MSG_STRING},
    {"surname", OFFSZ(Message_t, cust), OFFSZ(Customer_t, surname), 0, MSG_STRING},
    {"code",    OFFSZ(Message_t, cust), OFFSZ(Customer_t, code), 0, MSG_STRING},
    {"birthDate",OFFSZ(Message_t, cust), OFFSZ(Customer_t, birthDate), 0, MSG_LONG},
    {"gender",  OFFSZ(Message_t, cust), OFFSZ(Customer_t, gender), 0, MSG_STRING},
    {"natoinality",OFFSZ(Message_t, cust), OFFSZ(Customer_t, natoinality), 0, MSG_STRING},
    {"country", OFFSZ(Message_t, cust), OFFSZ(Customer_t, country), 0, MSG_STRING},
    {"city",    OFFSZ(Message_t, cust), OFFSZ(Customer_t, city), 0, MSG_STRING},
    {"addr",    OFFSZ(Message_t, cust), OFFSZ(Customer_t, addr), 0, MSG_STRING},
    {"family",  OFFSZ(Message_t, cust), OFFSZ(Customer_t, family), 0, MSG_STRING},
    {"childs",  OFFSZ(Message_t, cust), OFFSZ(Customer_t, childs), 0, MSG_SHORT},
    {"docNo",   OFFSZ(Message_t, cust), OFFSZ(Customer_t, docNo), 0, MSG_STRING},
    {"telNo",   OFFSZ(Message_t, cust), OFFSZ(Customer_t, telNo), 0, MSG_STRING},
    {"email",   OFFSZ(Message_t, cust), OFFSZ(Customer_t, email), 0, MSG_STRING},
    {"pyldnum",OFFSZ(Message_t, cust), OFFSZ(Customer_t, pyldnum), 
                OFFSZ(Customer_t, pyldnum_items), MSG_ARRAY_SHORT},
    {"pyldstr", OFFSZ(Message_t, cust), OFFSZ(Customer_t, pyldstr), 
                OFFSZ(Customer_t, pyldstr_items), MSG_ARRAY_STRING},
    /* employee mappings */
    {"empname", OFFSZ(Message_t, emp), OFFSZ(Employee_t, empname), 0, MSG_STRING},
    {"empsurname", OFFSZ(Message_t, emp), OFFSZ(Employee_t, empsurname), 0, MSG_STRING},
    {"empcode", OFFSZ(Message_t, emp), OFFSZ(Employee_t, empcode), 0, MSG_STRING},
    {"empnotes",OFFSZ(Message_t, emp), OFFSZ(Employee_t, empnotes), 0, MSG_STRING},
    
    /* response mappings */
    {"rsprspmsg",OFFSZ(Message_t, emp), OFFSZ(Response_t, rsprspmsg), 0, MSG_STRING},
    {"rspstatus",OFFSZ(Message_t, emp), OFFSZ(Response_t, rspstatus), 0, MSG_SHORT},
    {0}
};


/*---------------------------Prototypes---------------------------------*/


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
    
    /* TODO: Alloc STRING into obuf */
    
    
    /* */
    
    /* start libxml2 XML doc */
    while (0!=p->tag)
    {
        
        switch (p->elmtyp)
        {
            case MSG_SHORT:
                break;
            case MSG_LONG:
                break;
            case MSG_STRING:
                break;
            case MSG_ARRAY_SHORT:
                break;
            case MSG_ARRAY_STRING:
                break;
            default:
                break;
        }
        
        p++;
    }
    
    
    /* build xmldoc, copy to outbuf, specify size */
    
out:
    return ret;
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
    
    
out:
    return ret;
}
