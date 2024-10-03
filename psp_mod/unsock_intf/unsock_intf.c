/***********************************************************************
 *  Copyright (c) 2017, United States government as represented by the
 *  administrator of the National Aeronautics and Space Administration.
 *  All rights reserved. This software was created at NASA Glenn
 *  Research Center pursuant to government contracts.
 *
 *  \file unsock_intf.c
 *  \author lisa.b.vanderaar@nasa.gov
 *  \date April 3, 2017
 *
 *  \description Serial interface PSP module. Can handle hw serial ports and pseudo-terminals.
 *
 *  This file is a copy of controller_intf.c for eva cws with minor changes.
 *     controller_intf.c
 *     -- Created on: Sep 10, 2015
 *     -- Created by: joseph.p.hickey@nasa.gov
 *
 ***********************************************************************/

#define UNSOCK_INTF_DEBUG


/************************************************************************
 * Includes
 ************************************************************************/

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/un.h>


#include "osapi.h"
#include "cfe_psp.h"
#include "cfe_psp_module.h"
#include "iodriver_impl.h"
#include "iodriver_packet_io.h"

/********************************************************************
 * Global Data
 ********************************************************************/

/* Function that starts up unsock driver. */
static int32 unsockDevCmd(uint32 CommandCode, uint16 InstanceNumber, uint16 SubChannel, CFE_PSP_IODriver_Arg_t Arg);

/* unsock_intf device command that is called by iodriver to start up unsock_intf */
CFE_PSP_IODriver_API_t unsock_intf_DevApi =
{
        .DeviceCommand = unsockDevCmd
};

/* Drive driver name. */
CFE_PSP_MODULE_DECLARE_IODEVICEDRIVER(unsock_intf);


/********************************************************************
 * Local Defines
 ********************************************************************/

#define UNSOCK_MAX_INTERFACE_DEVS 2 /* for now */
#define UNSOCK_SOCKET_NAME_TEMPLATE "/tmp/CFE:%s%u.sock"

/********************************************************************
 * Local Type Definitions
 ********************************************************************/

/* Socket Interface State Information */
typedef struct
{
    char IntfName[OS_MAX_API_NAME];
    CFE_PSP_IODriver_Direction_t ConfigDir;
    uint32 ConfigBaudRate;                   // Data rate
    uint32 SubChannelId;
    struct sockaddr_un LocalAddr;
    int DeviceFd;                            // Socket device file descriptor (link)
} unsock_intf_State_t;

static struct
{
    uint32 MyPspModuleId;
    unsock_intf_State_t State[UNSOCK_MAX_INTERFACE_DEVS];
} UnSock_Global;



/********************************************************************
 * Local Function Prototypes
 ********************************************************************/

int32 unsock_intf_OpenPort    (unsock_intf_State_t *State, uint32 Instance);
int32 unsock_intf_Configure   (unsock_intf_State_t *State, uint32 Instance, const char *ConfigString);
void  unsock_intf_Init        (uint32 PspModuleId);
int32 unsock_intf_ReadPacket  (unsock_intf_State_t *State, CFE_PSP_IODriver_ReadPacketBuffer_t *Dest);
int32 unsock_intf_WritePacket (unsock_intf_State_t *State, CFE_PSP_IODriver_WritePacketBuffer_t *Source);


/***********************************************************************
 * Global Functions
 ********************************************************************/


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * unsock_intf_DebugTrace()
 * ------------------------------------------------------
 *  Debug trace is a no-op unless compiled with debugging enabled
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static inline void unsock_intf_DebugTrace(const char *op, uint8 *BaseAddr, uint32 Size)
{
#ifdef UNSOCK_INTF_DEBUG
    uint32 i;

    for (i=0; i < Size; ++i)
    {
        if ((i & 0xF) == 0)
        {
            printf("\n%s:", op);
        }
        printf(" %02x", (unsigned int)*BaseAddr);
        ++BaseAddr;
    }
    printf("\n");
#endif
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * unsock_intf_OpenPort()
 * ------------------------------------------------------
 *  Opens either a master or slave pseudo-terminal
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int32 unsock_intf_OpenPort(unsock_intf_State_t *State, uint32 Instance)
{
    int32 Result;
    int PendingFd;

    Result = CFE_PSP_ERROR;

 /* ---- Open a master or slave pseudo-terminal ---- */
    snprintf(State->LocalAddr.sun_path, sizeof(State->LocalAddr.sun_path), UNSOCK_SOCKET_NAME_TEMPLATE, State->IntfName, (unsigned int)Instance);
    State->LocalAddr.sun_family = AF_UNIX;

    PendingFd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (PendingFd < 0)
    {
        perror("unsock_intf_OpenPort: socket()");
    }
    else if (State->ConfigDir == CFE_PSP_IODriver_Direction_INPUT_ONLY)
    {
        OS_printf("%s(): Opening input side: %s\n", __func__, State->LocalAddr.sun_path);

       /* Ensure that any old/stale symlink is deleted (failure OK) */
        unlink(State->LocalAddr.sun_path);

        if (bind(PendingFd, (struct sockaddr *)&State->LocalAddr, sizeof(State->LocalAddr)) < 0)
        {
            perror("unsock_intf_OpenPort: bind()");
        }
        else
        {
            Result = CFE_PSP_SUCCESS;
        }
    }
    else
    {
        OS_printf("%s(): Opening output side: %s\n", __func__, State->LocalAddr.sun_path);
        Result = CFE_PSP_SUCCESS;
    }

    /* Error handing for opening either master or slave pseudo-terminal */
    if (PendingFd >= 0)
    {
        if (Result != CFE_PSP_SUCCESS)
        {
            close(PendingFd);
        }
        else
        {
            State->DeviceFd = PendingFd;
            OS_printf("CFE_PSP: Successfully opened socket: %s\n", State->LocalAddr.sun_path);
        }
    }

    return Result;
}

 /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  * unsock_intf_Configure()
  * --------------------------------------------------------
  *  Configures socket
  *
  * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int32 unsock_intf_Configure   (unsock_intf_State_t *State, uint32 Instance, const char *ConfigString)
{
    int32 Result;
    const char *Div;
    size_t KeyLen;

    Result = CFE_PSP_ERROR;

    Div = strchr(ConfigString, '=');
    if (Div != NULL)
    {
        KeyLen = Div - ConfigString;
        ++Div;

        if (KeyLen == 4 && memcmp(ConfigString, "name", KeyLen) == 0)
        {
            strncpy(State->IntfName, Div, sizeof(State->IntfName) - 1);
            State->IntfName[sizeof(State->IntfName) - 1] = 0;
            Result = CFE_PSP_SUCCESS;
        }
    }

    return Result;
}

 /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  * unsock_intf_Init()
  * --------------------------------------------------------
  *  Initializes pseudo-terminal.
  *
  * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void unsock_intf_Init(uint32 PspModuleId)
{
    uint32 i;

    memset(&UnSock_Global, 0, sizeof(UnSock_Global));
    OS_printf("CFE_PSP: Initializing unsock_intf interface\n");
    UnSock_Global.MyPspModuleId = PspModuleId;

    for (i=0; i < UNSOCK_MAX_INTERFACE_DEVS; ++i)
    {
        UnSock_Global.State[i].DeviceFd = -1;
    }
}


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * unsock_intf_ReadPacket()
 * --------------------------------------------------------
 *  Reads packets from pseudo-terminal.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int32 unsock_intf_ReadPacket(unsock_intf_State_t *State, CFE_PSP_IODriver_ReadPacketBuffer_t *Dest)
{
    ssize_t ReadSize;
    int32 Result;

    ReadSize = recv(State->DeviceFd, Dest->BufferMem, Dest->BufferSize, 0);

    if (ReadSize < 0)
    {
        perror("unsock_intf_ReadPacket: recv()");
        Result = CFE_PSP_ERROR;
    }
    else
    {
        /*
        * Output the actual size back to the requester
        * Doing this in all cases so the requester can still identify if any data
        * was received and take appropriate action (e.g. if their buffer was not big enough)
        */
        Dest->BufferSize = ReadSize;
        Result = CFE_PSP_SUCCESS;
    }

    return Result;
}


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * unsock_intf_WritePacket()
 * ------------------------------------------------------
 *  Writes packets to serial interface.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int32 unsock_intf_WritePacket(unsock_intf_State_t *State, CFE_PSP_IODriver_WritePacketBuffer_t *Source)
{
    ssize_t WriteSize;
    int32 Result;

    WriteSize = sendto(State->DeviceFd, Source->BufferMem, Source->OutputSize, 0, (const struct sockaddr *)&State->LocalAddr, sizeof(State->LocalAddr));

    if (WriteSize < 0)
    {
        perror("unsock_intf_WritePacket: send()");
        Result = CFE_PSP_ERROR;
    }
    else
    {
        Source->OutputSize = WriteSize;
        Result = CFE_PSP_SUCCESS;
    }

    return Result;
}



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*    unsockDevCmd()                                         */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/**
 ** \brief This function is called by IO_Driver to run the unsock_intf module.
 **
 ** \par Description
 **          This function is called by IO_Driver to run the unsock_intf module.
 ** \par Assumptions, External Events, and Notes:
 **          None
 **
 ** \param[in] CommandCode  The CFE_PSP_IODriver_xxx command.
 ** \param[in] Instance     Board instance. Set to 1 if there is only one serial card.
 ** \param[in] SubChannel   Subchannel number. Set to 0 for devices that do not have multiple channels.
 ** \param[in] Arg          The arguments for the corresponding command.
 **
 ** \returns
 ** \retcode #CFE_SUCCESS        \retdesc \copydoc CFE_SUCCESS       \endcode
 ** \retcode #CFE_ES_APP_ERROR   \retdesc \copydoc CFE_ES_APP_ERROR  \endcode
 ** \endreturns
 **
 **/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int32 unsockDevCmd(uint32 CommandCode, uint16 Instance, uint16 SubChannel, CFE_PSP_IODriver_Arg_t Arg)
{
    int32 ReturnCode;
    unsock_intf_State_t *InstPtr;

    /* By default return NOT_IMPLEMENTED -
     * Opcodes which are implemented must overwrite this */
    ReturnCode = CFE_PSP_ERROR_NOT_IMPLEMENTED;

    if (Instance > 0 && Instance <= UNSOCK_MAX_INTERFACE_DEVS)
    {
        InstPtr = &UnSock_Global.State[Instance - 1];

        switch(CommandCode)
        {
        case CFE_PSP_IODriver_NOOP:
        case CFE_PSP_IODriver_PACKET_IO_NOOP:
        {
            /* NO-OP should return success -
             * This is a required opcode as "generic" clients may use it to
             * determine if a certain set of opcodes are supported or not
             */
            ReturnCode = CFE_PSP_SUCCESS;
            break;
        }
        /* Start/stop opcodes */
        case CFE_PSP_IODriver_SET_RUNNING:       /**< int32 argument, 0=stop 1=start device */
        {
            if (Arg.U32)
            {
                if (InstPtr->DeviceFd >= 0)
                {
                    ReturnCode = CFE_PSP_SUCCESS;
                }
                else
                {
                    ReturnCode = unsock_intf_OpenPort(InstPtr, Instance);
                }
            }
            else
            {
                if (InstPtr->DeviceFd >= 0)
                {
                    close(InstPtr->DeviceFd);
                    InstPtr->DeviceFd = -1;
                }
                ReturnCode = CFE_PSP_SUCCESS;
            }
            break;
        }
        case CFE_PSP_IODriver_GET_RUNNING:       /**< no argument, returns positive nonzero (true) if running and zero (false) if stopped, negative on error */
        {
            if (InstPtr->DeviceFd >= 0)
            {
                ReturnCode = 1;
            }
            else
            {
                ReturnCode = 0;
            }
            break;
        }
        case CFE_PSP_IODriver_SET_CONFIGURATION: /**< const string argument (device-dependent content) */
        {
            ReturnCode = unsock_intf_Configure(InstPtr, Instance, (const char *)Arg.ConstVptr);
            break;
        }
        case CFE_PSP_IODriver_GET_CONFIGURATION: /**< void * argument (device-dependent content) */
        {
            break;
        }
        case CFE_PSP_IODriver_LOOKUP_SUBCHANNEL: /**< const char * argument, looks up ChannelName and returns positive value for channel number, negative value for error */
        {
            /* Does not support "subchannels"; always return 0 */
            ReturnCode = 0;
            break;
        }
        case CFE_PSP_IODriver_SET_DIRECTION:     /**< CFE_PSP_IODriver_Direction_t argument */
        {
            CFE_PSP_IODriver_Direction_t Dir = (CFE_PSP_IODriver_Direction_t)Arg.U32;
            if (Dir == CFE_PSP_IODriver_Direction_INPUT_ONLY || Dir == CFE_PSP_IODriver_Direction_OUTPUT_ONLY)
            {
                InstPtr->ConfigDir = Dir;
                ReturnCode = CFE_PSP_SUCCESS;
            }
            break;
        }
        case CFE_PSP_IODriver_QUERY_DIRECTION:   /**< CFE_PSP_IODriver_Direction_t argument */
        {
            CFE_PSP_IODriver_Direction_t *DirPtr = (CFE_PSP_IODriver_Direction_t *)Arg.Vptr;
            if (DirPtr != NULL)
            {
                *DirPtr = InstPtr->ConfigDir;
                ReturnCode = CFE_PSP_SUCCESS;
            }
            break;
        }
        case CFE_PSP_IODriver_PACKET_IO_READ:   /**< CFE_PSP_IODriver_ReadPacketBuffer_t argument */
        {
            if (InstPtr->ConfigDir == CFE_PSP_IODriver_Direction_INPUT_ONLY)
            {
                ReturnCode = unsock_intf_ReadPacket(InstPtr, (CFE_PSP_IODriver_ReadPacketBuffer_t *)Arg.Vptr);
            }
            break;
        }
        case CFE_PSP_IODriver_PACKET_IO_WRITE:  /**< CFE_PSP_IODriver_WritePacketBuffer_t argument */
        {
            if (InstPtr->ConfigDir == CFE_PSP_IODriver_Direction_OUTPUT_ONLY)
            {
                ReturnCode = unsock_intf_WritePacket(InstPtr, (CFE_PSP_IODriver_WritePacketBuffer_t *)Arg.Vptr);
            }
            break;
        }
        default:
            break;
        }
    }

    return ReturnCode;
}
