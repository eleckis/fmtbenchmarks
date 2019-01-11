/**
 * Domain model
 */

#ifndef DOMAIN_H
#define DOMAIN_H

#ifdef __cplusplus
extern "C" {
#endif

    
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
#define MAX_ARR		500
#define PROC_DEC_ARR	'D'
#define PROC_STR_ARR	'S'
	
#ifndef SUCCEED
#define SUCCEED			0
#endif

#ifndef	FAIL
#define FAIL			-1
#endif
    
#define MSG_SHORT                    1
#define MSG_LONG                     2
#define MSG_STRING                   3
#define MSG_ARRAY_SHORT              4
#define MSG_ARRAY_STRING             5
    
#define MAX_BUFSZ                   2240000
#define MAX_STR                     50
    
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
typedef struct {
    char name[50];
    char surname[50];
    char code[14];
    long birthDate;
    char gender[2];
    char natoinality[30];
    char country[30];
    char city[30];
    char addr[150];
    char family[15];
    short childs;
    char docNo[50];
    char telNo[14];
    char email[50];
    
    short pyldnum[1000];
    short pyldnum_items;
    
    char pyldstr[1000][MAX_STR];
    short pyldstr_items;
    
} Customer_t;

typedef struct {
    char empname[50];
    char empsurname[50];
    char empcode[30];
    char empnotes[255];
} Employee_t;

typedef struct {
    char rsprspmsg[100];
    short rspstatus;
} Response_t;

/**
 * IPC Message
 */
typedef struct {
    Customer_t cust;
    Employee_t emp;
    Response_t rsp;
} Message_t;

/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/
extern int msg_build(Message_t *msg, char **outbuf, long *olen);
extern int parse_msg(Message_t *msg, char *ibuf, long ilen);
extern char *msg_service(void);

#ifdef __cplusplus
}
#endif

#endif /* DOMAIN_H */

