/************************************************************************
 * File: bp_flowtable.c
 *
 * Purpose:
 *  Source for the flow table
 *
 *************************************************************************/

/************************************************************************
** Includes
*************************************************************************/

#include <stdint.h>

#include "cfe.h"
#include "bp_tbl.h"
#include "bp_platform_cfg.h"
#include "bplib.h"
#include "cfe_tbl_filedef.h"

/************************************************************************
** Data
*************************************************************************/

/*
** Table file header
*/
CFE_TBL_FileDef_t CFE_TBL_FileDef =
{
    "BP_FlowTable",
    "BP.FlowTable",
    "Configuration of bundle flows",
    "bp_flowtable.tbl",
    sizeof(BP_FlowTable_t)
};

/*
** Table contents
*/
BP_FlowTable_t BP_FlowTable =
{
    .LocalNodeIpn = 12,
    .Flows =
    {
        {   /* Flow 0 */
            .Name = "CFDP",
            .Enabled = true,
            .PipeDepth = BP_APP_READ_LIMIT,
            .SrcServ = 1,
            .DstNode = 13,
            .DstServ = 1,
            .Timeout = 0,
            .Lifetime = 86400,
            .Priority = BP_COS_NORMAL,
            .MaxActive = 0,
            .Store = BP_FLASH_STORE,
            .PktTbl = {{ CFE_SB_MSGID_WRAP_VALUE(0x082a), 1, 1, BP_APP_READ_LIMIT }},
            .RecvStreamId = CFE_SB_MSGID_WRAP_VALUE(0x182a)
        }
    }
};

/************************/
/*  End of File Comment */
/************************/
