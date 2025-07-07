/*
 * NASA Docket No. GSC-18,587-1 and identified as “The Bundle Protocol Core Flight
 * System Application (BP) v6.5”
 *
 * Copyright © 2020 United States Government as represented by the Administrator of
 * the National Aeronautics and Space Administration. All Rights Reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

/**
 * \file
 *   This file contains the source code for a UDP socket interface PSP module.
 */


/*
** Include Files
*/

#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

#include "osapi.h"
#include "cfe_psp.h"
#include "cfe_psp_module.h"
#include "iodriver_impl.h"
#include "iodriver_packet_io.h"


/*
** Global Data
*/

/* Function that starts up udpsock driver. */
static int32 udpsockDevCmd(uint32 CommandCode, uint16 InstanceNumber, uint16 SubChannel, 
                                                            CFE_PSP_IODriver_Arg_t Arg);

/* udpsock_intf device command that is called by iodriver to start up udpsock_intf */
CFE_PSP_IODriver_API_t udpsock_intf_DevApi =
{
        .DeviceCommand = udpsockDevCmd
};

/* Drive driver name. */
CFE_PSP_MODULE_DECLARE_IODEVICEDRIVER(udpsock_intf);


/* 
** Macro Definitions
*/

#define UDPSOCK_MAX_INTERFACE_DEVS 2 /* For now */

#define UDPSOCK_INTF_DEBUG      /* Turn debug prints on for now */

#define UDPSOCK_RECV_TIMEOUT_MS  10

/*
** Local Data Definitions
*/

/* Socket Interface State Information */
typedef struct
{
    char IntfName[OS_MAX_API_NAME];
    CFE_PSP_IODriver_Direction_t ConfigDir;
    uint32 ConfigBaudRate;                   // Data rate
    uint32 SubChannelId;
    struct sockaddr_in LocalAddr;
    int DeviceFd;                            // Socket device file descriptor (link)
} udpsock_intf_State_t;

static struct
{
    uint32 MyPspModuleId;
    udpsock_intf_State_t State[UDPSOCK_MAX_INTERFACE_DEVS];
} UdpSock_Global;



/*
** Local Function Definitions
*/

int32 udpsock_intf_OpenPort    (udpsock_intf_State_t *State, uint32 Instance);
int32 udpsock_intf_Configure   (udpsock_intf_State_t *State, uint32 Instance, const char *ConfigString);
void  udpsock_intf_Init        (uint32 PspModuleId);
int32 udpsock_intf_ReadPacket  (udpsock_intf_State_t *State, CFE_PSP_IODriver_ReadPacketBuffer_t *Dest);
int32 udpsock_intf_WritePacket (udpsock_intf_State_t *State, CFE_PSP_IODriver_WritePacketBuffer_t *Source);


/*
** Global Function DefinitionsS
*/

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * udpsock_intf_DebugTrace()
 * ------------------------------------------------------
 *  Debug trace is a no-op unless compiled with debugging enabled
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static inline void udpsock_intf_DebugTrace(const char *op, uint8 *BaseAddr, uint32 Size)
{
#ifdef UDPSOCK_INTF_DEBUG
    uint32 i;

    for (i = 0; i < Size; ++i)
    {
        if ((i & 0xF) == 0)
        {
            OS_printf("\n%s:", op);
        }

        OS_printf(" %02x", (unsigned int)*BaseAddr);
        ++BaseAddr;
    }

    OS_printf("\n");

#endif
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * udpsock_intf_OpenPort()
 * ------------------------------------------------------
 *  Opens either a master or slave pseudo-terminal
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int32 udpsock_intf_OpenPort(udpsock_intf_State_t *State, uint32 Instance)
{
    int32 Result = CFE_PSP_ERROR;
    int PendingFd;

    State->LocalAddr.sin_family = AF_INET;

    PendingFd = socket(AF_INET, SOCK_DGRAM, 0);
    
    if (PendingFd < 0)
    {
        perror("udpsock_intf_OpenPort: socket()");
    }
    else if (State->ConfigDir == CFE_PSP_IODriver_Direction_INPUT_ONLY)
    {
        OS_printf("%s(): Opening input side: %s\n", __func__, inet_ntoa(State->LocalAddr.sin_addr));

        if (bind(PendingFd, (struct sockaddr *)&State->LocalAddr, sizeof(State->LocalAddr)) < 0)
        {
            perror("udpsock_intf_OpenPort: bind()");
        }
        else
        {
            Result = CFE_PSP_SUCCESS;
        }
    }
    else
    {
        OS_printf("%s(): Opening output side: %s\n", __func__, inet_ntoa(State->LocalAddr.sin_addr));
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
            OS_printf("CFE_PSP: Successfully opened socket: %s\n", inet_ntoa(State->LocalAddr.sin_addr));
        }
    }

    return Result;
}

 /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  * udpsock_intf_Configure()
  * --------------------------------------------------------
  *  Configures socket
  *
  * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int32 udpsock_intf_Configure(udpsock_intf_State_t *State, uint32 Instance, const char *ConfigString)
{
    int32 Result = CFE_PSP_ERROR;
    const char *Div;
    size_t KeyLen;

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
        else if(KeyLen == 4 && memcmp(ConfigString, "port", KeyLen) == 0)
        {
            State->LocalAddr.sin_port = htons(atoi(Div));
            Result = CFE_PSP_SUCCESS;
        }
        else if(KeyLen == 6 && memcmp(ConfigString, "IpAddr", KeyLen) == 0)
        {
            State->LocalAddr.sin_addr.s_addr = inet_addr(Div);
            Result = CFE_PSP_SUCCESS;
        }
    }

    return Result;
}

 /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  * udpsock_intf_Init()
  * --------------------------------------------------------
  *  Initializes pseudo-terminal.
  *
  * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void udpsock_intf_Init(uint32 PspModuleId)
{
    uint32 i;

    memset(&UdpSock_Global, 0, sizeof(UdpSock_Global));
    OS_printf("CFE_PSP: Initializing udpsock_intf interface\n");
    UdpSock_Global.MyPspModuleId = PspModuleId;

    for (i = 0; i < UDPSOCK_MAX_INTERFACE_DEVS; ++i)
    {
        UdpSock_Global.State[i].DeviceFd = -1;
    }
}


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * udpsock_intf_ReadPacket()
 * --------------------------------------------------------
 *  Reads packets from pseudo-terminal.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int32 udpsock_intf_ReadPacket(udpsock_intf_State_t *State, CFE_PSP_IODriver_ReadPacketBuffer_t *Dest)
{
    ssize_t ReadSize;
    int32 Result;
    int32 PollRc;
    struct pollfd pfd;

    pfd.fd = State->DeviceFd;
    pfd.events = POLLIN;
    PollRc = poll(&pfd, 1, UDPSOCK_RECV_TIMEOUT_MS);
    if (PollRc < 0)
    {
        perror("udpsock_intf_ReadPacket: poll()");
        return CFE_PSP_ERROR;
    }
    else if (PollRc == 0)
    {
        return CFE_PSP_ERROR_TIMEOUT;
    }

    ReadSize = recv(State->DeviceFd, Dest->BufferMem, Dest->BufferSize, 0);

    if (ReadSize < 0)
    {
        perror("udpsock_intf_ReadPacket: recv()");
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
 * udpsock_intf_WritePacket()
 * ------------------------------------------------------
 *  Writes packets to serial interface.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int32 udpsock_intf_WritePacket(udpsock_intf_State_t *State, CFE_PSP_IODriver_WritePacketBuffer_t *Source)
{
    ssize_t WriteSize;
    int32 Result;

    WriteSize = sendto(State->DeviceFd, Source->BufferMem, Source->OutputSize, 0, 
                    (const struct sockaddr *)&State->LocalAddr, sizeof(State->LocalAddr));

    if (WriteSize < 0)
    {
        perror("udpsock_intf_WritePacket: send()");
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
/*    udpsockDevCmd()                                         */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/**
 ** \brief This function is called by IO_Driver to run the udpsock_intf module.
 **
 ** \par Description
 **          This function is called by IO_Driver to run the udpsock_intf module.
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
int32 udpsockDevCmd(uint32 CommandCode, uint16 Instance, uint16 SubChannel, 
                    CFE_PSP_IODriver_Arg_t Arg)
{
    int32 ReturnCode;
    udpsock_intf_State_t *InstPtr;

    /* By default return NOT_IMPLEMENTED -
     * Opcodes which are implemented must overwrite this */
    ReturnCode = CFE_PSP_ERROR_NOT_IMPLEMENTED;

    if (Instance > 0 && Instance <= UDPSOCK_MAX_INTERFACE_DEVS)
    {
        InstPtr = &UdpSock_Global.State[Instance - 1];

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
            case CFE_PSP_IODriver_SET_RUNNING: /**< int32 argument, 0=stop 1=start device */
            {
                if (Arg.U32)
                {
                    if (InstPtr->DeviceFd >= 0)
                    {
                        ReturnCode = CFE_PSP_SUCCESS;
                    }
                    else
                    {
                        ReturnCode = udpsock_intf_OpenPort(InstPtr, Instance);
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
                ReturnCode = udpsock_intf_Configure(InstPtr, Instance, (const char *)Arg.ConstVptr);
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
                if (Dir == CFE_PSP_IODriver_Direction_INPUT_ONLY || 
                    Dir == CFE_PSP_IODriver_Direction_OUTPUT_ONLY)
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
                    ReturnCode = udpsock_intf_ReadPacket(InstPtr, (CFE_PSP_IODriver_ReadPacketBuffer_t *)Arg.Vptr);
                }
                break;
            }
            case CFE_PSP_IODriver_PACKET_IO_WRITE:  /**< CFE_PSP_IODriver_WritePacketBuffer_t argument */
            {
                if (InstPtr->ConfigDir == CFE_PSP_IODriver_Direction_OUTPUT_ONLY)
                {
                    ReturnCode = udpsock_intf_WritePacket(InstPtr, (CFE_PSP_IODriver_WritePacketBuffer_t *)Arg.Vptr);
                }
                break;
            }
            default:
                break;
        }
    }

    return ReturnCode;
}

