#include "bts_hash.h"
#include "bts_hashtable.h"
#include "bts_linklist.h"
#include "bts_util.h"
#include "bts_debug.h"

#include "dml.h"



bts_hashtable_t dml_table[DML_MAX_INDEX];



uint32_t 
dml_hash_func(void *data)
{
    dml_t *entry = NULL;

    if (NULL == data)
    {
        return 0;
    }

    entry = (dml_t *) data; 

    return bts_hash(entry->host, entry->host_len);
}



int 
dml_cmp_func(void *d1, void *d2)
{
    dml_t *e1, *e2;

    if ((NULL == d1) || (NULL == d2))
    {
        return -2;
    }

    e1 = (dml_t *) d1;
    e2 = (dml_t *) d2;

    return bts_str_cmp((void *)(e1->host), (void *)(e2->host));
}




berr dml_init(void)
{
    int i = 0;

    for (i=0; i<DML_MAX_INDEX; i++)
    {
    
        bts_hashtable_init(&dml_table[i], DML_MAX_EACH, dml_hash_func, dml_cmp_func, NULL);
    }

    return E_SUCCESS;
         
}



berr dml_add(uint32_t  index,  dml_t *entry)
{
    if(index >= DML_MAX_INDEX)
    {
        return  -1;
    }
    
    return bts_hashtable_add(&dml_table[index], (void *) entry);

}



void dml_clear_func(void *data, void *param)
{
    uint32_t index;

    index = *((uint32_t *)param);

    if (NULL == data || param == NULL || index >= DML_MAX_INDEX)
    {
        return;
    }

    bts_hashtable_del(&dml_table[index], data);
}



berr dml_clear(uint32_t  index)
{
    if (index >= DML_MAX_INDEX)
    {
        return -2;
    }
    
    bts_hashtable_iter(&dml_table[index], dml_clear_func, &index);
    
    return E_SUCCESS;
}



dml_t* dml_get(uint32_t index, char *domain_str)
{
	dml_t data;

    if(domain_str == NULL)
    {
        return NULL;
    }

    if(index >= DML_MAX_INDEX)
    {
        return  NULL;
    }

	memset(&data, 0, sizeof(dml_t));
    data.host_len = snprintf(data.host, sizeof(data.host), "%s", domain_str);
    
    return (dml_t *)bts_hashtable_lookup(&dml_table[index], (void*) (&data));
}



int dml_exist(uint32_t index, char * domain_str)
{

    if(dml_get(index, domain_str))
    {
        return  1;
    }
    else
    {
        return  0;
    }

    
}




typedef struct 
{
    uint32_t  index;
    uint32_t  num;
}dml_num_t;


void dml_get_num_func(void *data, void *param)
{
    dml_num_t *par_p = NULL;

    par_p = (dml_num_t *)param;

    if (NULL == data || par_p == NULL)
    {
        return;
    }

    par_p->num ++;
}




int dml_get_num(uint32_t  index)
{
    dml_num_t  par;

    memset (&par, 0, sizeof(par));
    
    par.index = index;
    par.num = 0;
    
    if (index >= DML_MAX_INDEX)
    {
        return -2;
    }
    
    bts_hashtable_iter(&dml_table[index], dml_get_num_func, &par);
    
    return par.num;
}




berr
dml_iter(uint32_t index, bts_iter_func func, void *param)
{
    if (NULL == func || index >= DML_MAX_INDEX)
    {
        BRET(E_PARAM);
    }

    bts_hashtable_iter(&dml_table[index], func, param); 

    return E_SUCCESS;
}




