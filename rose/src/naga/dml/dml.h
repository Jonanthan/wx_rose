#ifndef __HIJACK_H__
#define __HIJACK_H__



#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "naga_types.h"
#include "boots.h"


#define   DML_MAX_INDEX    50
#define   DML_MAX_EACH     20000



typedef  struct {
    char host[MAX_HOST_LEN];
	uint32_t  host_len;
}dml_t;


berr dml_init(void);
berr dml_add(uint32_t  index,  dml_t *entry);
berr dml_clear(uint32_t  index);
int dml_get_num(uint32_t  index);
int dml_exist(uint32_t index, char * domain_str);


#endif

