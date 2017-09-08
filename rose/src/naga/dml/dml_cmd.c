
#include "zebra.h"
#include "getopt.h"
#include "command.h"

#include "boots.h"
#include "bts_debug.h"

#include "naga_types.h"
#include "naga_util.h"
#include "naga_cmd.h"


#include "version.h"
#include "command.h"
#include "filter.h"
#include "prefix.h"
#include "privs.h"
#include "dml_cmd.h"

#include "dml.h"


#define   DML_STR   "Domain library\n"


static int cmd_dml_add(struct vty *vty, const char* index_str, const char* domain_str)
{
    uint32_t index = 0;
    berr ret = 0;
	dml_t *entry = NULL;

    index = atoi(index_str);

    entry = malloc(sizeof(dml_t));
    if(entry == NULL)
    {
        return CMD_ERR_NO_MATCH;
    }

    
    if(index >= DML_MAX_INDEX)
    {
        vty_out(vty, "Index out of %d!%s", DML_MAX_INDEX, VTY_NEWLINE);
        return -2;
    }


    if(index == 0)
    {
        vty_out(vty, "Index must start up from 1!%s", VTY_NEWLINE);
        return -2;
    }


    if(domain_str == NULL)
    {
        vty_out(vty, "Error domain param!!%s", VTY_NEWLINE);
        return -2;
    }

    memset(entry, 0, sizeof(*entry));
    entry->host_len = snprintf(entry->host, sizeof(entry->host), "%s", domain_str);
    
    ret = dml_add(index, entry);

    if (ret)
    {
        vty_out(vty, "dml add fail: ret(%s)%s", berr_msg(ret), VTY_NEWLINE);
        return CMD_WARNING;
    }

    return CMD_SUCCESS;
}




static int cmd_dml_clear(struct vty *vty, const char* index_str)
{
    uint32_t index = 0;
    berr ret = 0;
	dml_t *entry = NULL;

    index = atoi(index_str);
    
    if(index >= DML_MAX_INDEX)
    {
        vty_out(vty, "Index out of %d!%s", DML_MAX_INDEX, VTY_NEWLINE);
        return -2;
    }

    
    ret = dml_clear(index);

    if (ret)
    {
        vty_out(vty, "dml add fail: ret(%s)%s", berr_msg(ret), VTY_NEWLINE);
        return CMD_WARNING;
    }

    return CMD_SUCCESS;
}


void dml_dump_vty(void *data, void *param)
{
    dml_t *entry = NULL;
    struct vty *vty = NULL;
    
    if ((NULL == param) || (NULL == data))
    {
        return;
    }

    entry = (dml_t *) data;
    vty   = (struct vty *) param;

    vty_out(vty, "    %s %s", entry->host, VTY_NEWLINE);
}


static int cmd_dml_show_all(struct vty *vty)
{
    uint32_t index = 0;

    for (index = 0; index < DML_MAX_INDEX; index ++)
    {
        int num = dml_get_num(index);
        if(num)
        {

            vty_out(vty, "index:%d, num:%d %s", index, num, VTY_NEWLINE);
            vty_out(vty, "------------------------------------------------%s", VTY_NEWLINE);
        
            dml_iter(index, dml_dump_vty, vty);        

            vty_out(vty, "\n\n");
        }
    }


    return CMD_SUCCESS;
}



DEFUN(dml_add_f, 
      dml_add_cmd,
      "dml add  INDEX    DOMAIN",
      DML_STR
      "Add \n"
      "Index of dml\n"
      "Domain string\n")
{
    return cmd_dml_add(vty, argv[0], argv[1]);
}



DEFUN(dml_clear_f, 
      dml_clear_cmd,
      "dml clear  INDEX",
      DML_STR
      "Clear \n"
      "Index of dml\n")
{
    return cmd_dml_clear(vty, argv[0]);
}





DEFUN(dml_show_f, 
      dml_show_cmd,
      "show  dml  all",
      "Show configure\n"
      DML_STR
      "All \n")
{
    return cmd_dml_show_all(vty);
}


void cmdline_dml_init(void)
{
	install_element(CMD_NODE, &dml_add_cmd);
	install_element(CMD_NODE, &dml_clear_cmd);
    install_element(CMD_NODE, &dml_show_cmd);

    return ;
}




