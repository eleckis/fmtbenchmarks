/**
 * Copyright (C) 2018-2019, Einars Leckis - All Rights Reserved.
 */
#include <domain.h>
#include <stddef.h>
#include <ndrstandard.h>
#include <ndebug.h>
#include <atmi.h>

#include <jansson.h>
#include <xatmi.h>

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
    char tag[40];       /**< json tag                                   */
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
    {"rsprspmsg",OFFSZ(Message_t, rsp), OFFSZ(Response_t, rsprspmsg), 0, MSG_STRING},
    {"rspstatus",OFFSZ(Message_t, rsp), OFFSZ(Response_t, rspstatus), 0, MSG_SHORT},
    {0}
};


/*---------------------------Prototypes---------------------------------*/

char *msg_service(void)
{
	static char svc[] = "USERREG_JSON";
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
    long *p_long;
    void *fld_ptr;
    char *p_string_el;
    char tmpbuf[64];
    short *p_items;
    int i;
    int	size = 0;
    json_t *json = NULL;
    json_t *value;
    char *jsonbuf = NULL;
    
    /* Alloc STRING into obuf */
    if (NULL==*outbuf)
    {
        *outbuf = tpalloc("STRING", NULL, MAX_BUFSZ);
    }
    
    if (NULL==*outbuf)
    {
       TP_LOG(log_error, "Failed to alloc %d bytes: %s", 
			MAX_BUFSZ, tpstrerror(tperrno)); 
       ret=FAIL;
       goto out;
    }
    
    /* start JSON */
    json = json_object();
            
    while (0!=p->tag[0])
    {
        fld_ptr = (char *)msg + p->msgoffs + p->elmoffs;
                
        switch (p->elmtyp)
        {
            case MSG_SHORT:
                p_short =  (short *)fld_ptr;
                value = json_integer((json_int_t)*p_short);
                if (SUCCEED != json_object_set(json, p->tag, value)){
                    TP_LOG(log_error, "Failed to set %s", p->tag);
                    ret = FAIL;
                    goto out;
		}
                json_decref(value);
                break;
            case MSG_LONG:
                p_long =  (long *)fld_ptr;
                value = json_integer((json_int_t)*p_long);
                if (SUCCEED != json_object_set(json, p->tag, value)){
                    TP_LOG(log_error, "Failed to set %s", p->tag);
                    ret = FAIL;
                    goto out;
		}
                json_decref(value);
                break;
            case MSG_STRING:
                value = json_string((char *)fld_ptr);
                if (SUCCEED != json_object_set(json, p->tag, value)){
                    TP_LOG(log_error, "Failed to set %s", p->tag);
                    ret = FAIL;
                    goto out;
		}
                json_decref(value);
                break;
            case MSG_ARRAY_SHORT:
                
                p_items = (short *)((char *)msg + p->msgoffs + p->itmoffs);
                
                for (i=0; i<*p_items; i++)
                {
                    p_short = (short *)( (char *)fld_ptr + i*sizeof(short));
                    
                    snprintf(tmpbuf, sizeof(tmpbuf), "%s_%d", p->tag, i);
                    value = json_integer((json_int_t)*p_short);
                    if (SUCCEED != json_object_set(json, tmpbuf, value)){
                        TP_LOG(log_error, "Failed to set %s", tmpbuf);
                        ret = FAIL;
                        goto out;
                    }
                    json_decref(value);
                }
                
                break;
            case MSG_ARRAY_STRING:
                
                p_items = (short *)((char *)msg + p->msgoffs + p->itmoffs);
                
                for (i=0; i<*p_items; i++)
                {
                    /* calculate string array element location */
                    p_string_el = (char *)fld_ptr + i*MAX_STR;
                    
                    snprintf(tmpbuf, sizeof(tmpbuf), "%s_%d", p->tag, i);
                    value = json_string(p_string_el);
                    if (SUCCEED != json_object_set(json, tmpbuf, value)){
                        TP_LOG(log_error, "Failed to set %s", tmpbuf);
                        ret = FAIL;
                        goto out;
                    }
                    json_decref(value);
                }
                
                break;
            default:
                TP_LOG(log_error, "Unknown element type %d tag: [%s]!", 
                        p->elmtyp, p->tag);
                ret=FAIL;
                goto out;
                break;
        }
        
        p++;
    }
    
    /* build json, copy to outbuf, specify size */
    if(NULL == (jsonbuf = json_dumps(json, 0)))
    {
        TP_LOG(log_debug, "Failed represent JSON as string");
        ret=FAIL;
        goto out;
    }
    
    size = strlen(jsonbuf);
    strncpy(*outbuf, jsonbuf, size);
    (*outbuf)[size] = 0;
    *olen = size+1;
    
    TP_LOG(log_debug, "got JSON [%s]", *outbuf);
    
out:
    if (NULL != jsonbuf)
    {
        free(jsonbuf);
    }
    if (NULL != json)
    {
        json_object_clear(json);
        json_decref(json);
    }

    return ret;
}

/**
 * get parsing descriptor for tag
 * @param tag tag name
 * @return  descriptor or NULL
 */
static msgbuilder_t *get_tag(const char *tag)
{
    char tmp_tag[40];
    char *cp;
    msgbuilder_t *p = M_msgflds;
    
    strcpy(tmp_tag, tag);
    
    /* strip _ from arrays... */
    
    cp = strchr(tmp_tag, '_');
    if (NULL!=cp)
    {
        *cp = 0; /* put EOS at the end */
    }
            
    while (0!=p->tag[0])
    {
        if (0==strcmp(p->tag, tmp_tag))
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
    
    char *cp;
    
    void *fld_ptr;
    char *p_string_el;
    short item;
    const char * key;
    json_t *value;
    json_t *json = json_object();
    json_error_t error;
    
    /* start JSON */
    json = json_loads(ibuf, 0, &error);
	
    if ( NULL == json )
    {
        TP_LOG(log_error, "Failed to read JSON!");
        return FAIL;
    }

    json_object_foreach(json, key, value)
    {
        /* find tag descriptor */
        if (NULL==(descr = get_tag(key)))
        {
            TP_LOG(log_error, "Failed to get tag descr for [%s]", key);
            ret = FAIL;
            goto out;
        }
        
        switch (json_typeof(value))
        {
            case JSON_STRING:
                TP_LOG(log_debug, "got tag [%s] value [%s]", key, json_string_value(value));
                break;
            case JSON_INTEGER:
                TP_LOG(log_debug, "got tag [%s] value [%d]", key, json_integer_value(value));
                break;
                
        }
        
        
        /* load the field into struct accordingly */
        fld_ptr = (char *)msg + descr->msgoffs + descr->elmoffs;
        
        switch (descr->elmtyp)
        {
            case MSG_SHORT:
                p_short =  (short *)fld_ptr;
                *p_short = json_integer_value(value);
                break;
            case MSG_LONG:
                p_long =  (long *)fld_ptr;
                *p_long = json_integer_value(value);
                break;
            case MSG_STRING:
                strcpy((char *)fld_ptr, json_string_value(value));
                break;
            case MSG_ARRAY_SHORT:
                
                /* get item number from tag */
                cp = strchr(key, '_');
                cp++;
                
                item = atoi(cp);
                p_short = (short *)( (char *)fld_ptr + item*sizeof(short));
                *p_short = json_integer_value(value);
                
                break;
            case MSG_ARRAY_STRING:
                
                /* get item number from tag */
                cp = strchr(key, '_');
                cp++;
                
                item = atoi(cp);
                p_string_el = ( (char *)fld_ptr + item*MAX_STR);
                strcpy(p_string_el, json_string_value(value));
                
                break;
            default:
                TP_LOG(log_error, "Unknown element type %d!", descr->elmtyp);
                ret=FAIL;
                goto out;
                break;
        }
        
        json_decref(value);
        
    }
    
out:
    if (NULL != json)
    {
        json_object_clear(json);
        json_decref(json);
    }

    return ret;
}
