/**
 * Copyright (C) 2018-2019, Einars Leckis - All Rights Reserved.
 */
#include <domain.h>
#include <stddef.h>
#include <ndrstandard.h>
#include <ndebug.h>
#include <atmi.h>


#include <libxml/parser.h>
#include <libxml/entities.h>
#include <libxml/tree.h>
#include <xatmi.h>

/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/

#ifdef SYS64BIT
#define OFFSZ(STRUCT,ELM)   ((long) &(((STRUCT *)0)->ELM) )
#else
#define OFFSZ(STRUCT,ELM)   ((const int) &(((STRUCT *)0)->ELM) )
#endif

#define XML_VERSION             "1.0"

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
    {"rsprspmsg",OFFSZ(Message_t, rsp), OFFSZ(Response_t, rsprspmsg), 0, MSG_STRING},
    {"rspstatus",OFFSZ(Message_t, rsp), OFFSZ(Response_t, rspstatus), 0, MSG_SHORT},
    {0}
};


/*---------------------------Prototypes---------------------------------*/

char *msg_service(void)
{
	static char svc[] = "USERREG_XML";
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
    char tmpbuf2[64];
    xmlDocPtr   newDoc;
    xmlNodePtr  rootNode;
    short *p_items;
    int i;
    xmlChar *xmlDocInMemory = NULL;
    int		size = 0;
    
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
    
    /* start libxml2 XML doc */
    
    newDoc = xmlNewDoc( BAD_CAST XML_VERSION );
    rootNode = xmlNewNode(NULL, BAD_CAST "user");
    xmlDocSetRootElement(newDoc, rootNode);
            
    while (0!=p->tag[0])
    {
        fld_ptr = (char *)msg + p->msgoffs + p->elmoffs;
                
        switch (p->elmtyp)
        {
            case MSG_SHORT:
                p_short =  (short *)fld_ptr;
                snprintf(tmpbuf, sizeof(tmpbuf), "%hd", *p_short);
                xmlNewTextChild( rootNode, NULL, BAD_CAST p->tag, BAD_CAST tmpbuf );
                break;
            case MSG_LONG:
                p_long =  (long *)fld_ptr;
                snprintf(tmpbuf, sizeof(tmpbuf), "%ld", *p_long);
                xmlNewTextChild( rootNode, NULL, BAD_CAST p->tag, BAD_CAST tmpbuf );
                break;
            case MSG_STRING:
                xmlNewTextChild( rootNode, NULL, BAD_CAST p->tag, 
                        BAD_CAST ((char *)fld_ptr) );
                break;
            case MSG_ARRAY_SHORT:
                
                p_items = (short *)((char *)msg + p->msgoffs + p->itmoffs);
                
                for (i=0; i<*p_items; i++)
                {
                    p_short = (short *)( (char *)fld_ptr + i*sizeof(short));
                    snprintf(tmpbuf, sizeof(tmpbuf), "%hd", *p_short);
                    
                    snprintf(tmpbuf2, sizeof(tmpbuf), "%s_%d", p->tag, i);
                    xmlNewTextChild( rootNode, NULL, BAD_CAST tmpbuf2, 
                            BAD_CAST tmpbuf );
                }
                
                break;
            case MSG_ARRAY_STRING:
                
                p_items = (short *)((char *)msg + p->msgoffs + p->itmoffs);
                
                for (i=0; i<*p_items; i++)
                {
                    /* calculate string array element location */
                    p_string_el = (char *)fld_ptr + i*MAX_STR;
                    
                    snprintf(tmpbuf2, sizeof(tmpbuf), "%s_%d", p->tag, i);
                    xmlNewTextChild( rootNode, NULL, BAD_CAST tmpbuf2, 
                            BAD_CAST p_string_el );
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
    
    /* build xmldoc, copy to outbuf, specify size */
    xmlDocDumpMemory( newDoc, &xmlDocInMemory, &size );
    strncpy(*outbuf, xmlDocInMemory, size);
    (*outbuf)[size] = 0;
    *olen = size+1;
    xmlFree(xmlDocInMemory);
    
    TP_LOG(log_debug, "got XML [%s]", *outbuf);
    
out:
    if (NULL != newDoc)
    {
        xmlFreeDoc( newDoc );
    }
    return ret;
}

/**
 * get parsing descriptor for tag
 * @param tag tag name
 * @return  descriptor or NULL
 */
static msgbuilder_t *get_tag(char *tag)
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
    char *buf;
    
    short *p_short;
    long *p_long;
    
    char *cp;
    
    void *fld_ptr;
    char *p_string_el;
    xmlDocPtr   doc;
    xmlNodePtr  rootNode;
    xmlNode *currentNode = NULL;
    short item;
    
    /* start libxml2 XML doc */
    
    doc = xmlReadMemory( ibuf, strlen(ibuf), NULL, NULL, 0 );
	
    if ( NULL == doc )
    {
        TP_LOG(log_error, "Failed to read XML document!");
        return FAIL;
    }

    rootNode = xmlDocGetRootElement( doc );
    
    currentNode = rootNode->children;
            
    /* loop over all tags */
    for (; currentNode;
                    currentNode = currentNode->next )
    {
        /* find tag descriptor */
        if (NULL==(descr = get_tag((char *)currentNode->name)))
        {
            TP_LOG(log_error, "Failed to get tag descr for [%s]", 
                    currentNode->name);
            ret = FAIL;
            goto out;
        }
        
        /* get the content of the tag */
        if (NULL==(buf = (char *)xmlNodeGetContent(currentNode)))
        {
            TP_LOG(log_error, "NULL tag: [%s]", currentNode->name);
            ret = FAIL;
            goto out;
        }
        
        TP_LOG(log_debug, "got tag [%s] value [%s]", currentNode->name, 
                buf?buf:"(null)");
        
        /* load the field into struct accordingly */
        fld_ptr = (char *)msg + descr->msgoffs + descr->elmoffs;
        
        switch (descr->elmtyp)
        {
            case MSG_SHORT:
                p_short =  (short *)fld_ptr;
                *p_short = atoi(buf);
                break;
            case MSG_LONG:
                p_long =  (long *)fld_ptr;
                *p_long = atol(buf);
                break;
            case MSG_STRING:
                strcpy((char *)fld_ptr, buf);
                break;
            case MSG_ARRAY_SHORT:
                
                /* get item number from tag */
                cp = strchr(currentNode->name, '_');
                cp++;
                
                item = atoi(cp);
                p_short = (short *)( (char *)fld_ptr + item*sizeof(short));
                *p_short = atoi(buf);
                
                break;
            case MSG_ARRAY_STRING:
                
                /* get item number from tag */
                cp = strchr(currentNode->name, '_');
                cp++;
                
                item = atoi(cp);
                p_string_el = ( (char *)fld_ptr + item*MAX_STR);
                strcpy(p_string_el, buf);
                
                break;
            default:
                TP_LOG(log_error, "Unknown element type %d!", descr->elmtyp);
                ret=FAIL;
                goto out;
                break;
        }
        
         xmlFree(buf);
         
    }
    
out:
    if(doc)
    {
        xmlFreeDoc(doc);
    }

    return ret;
}
