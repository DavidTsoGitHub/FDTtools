/********************************* Includes **********************************/
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "jansson.h"
#include "PC_BSP.h"
#include "Logger.h"

/**************************** Constants and Types ****************************/
#define DEFAULT_PORT 7085
#define BUFSIZE 4096
#define COMPONENTNAME "SIMULATOR"

/********************************* Variables *********************************/
static SOCKET serverSocket;
static SOCKET clientSocket;

/************************* Local Function Prototypes *************************/
static int SIM_SetupSocket();
static int SIM_CleanUp();
static int SIM_ParseJSON(const char* buf);
static void SIM_SendMessage(const char* messageBuf, int messageLength);

/************************* Main program **************************************/
int main(int argc, char* argv[])
{
    char in_buf[BUFSIZE];

    Logger_Info(COMPONENTNAME, "Starting...");
    if(SIM_SetupSocket(DEFAULT_PORT) > 0) exit(-1);
    Logger_Info(COMPONENTNAME, "TCP Server up and running");

    bool running = true;
    /*** Connection accept loop ***/
    while(running)
    {
        Logger_Info(COMPONENTNAME, "Waiting for client connection on port %d", DEFAULT_PORT);
        struct sockaddr_in client_addr;
        int addr_len = sizeof(client_addr);
        clientSocket = accept(serverSocket, (struct sockaddr *)&client_addr, &addr_len);

        if (INVALID_SOCKET == clientSocket)
        {
            Logger_Error(COMPONENTNAME, "*** ERROR - accept() failed (Error: %d)", WSAGetLastError());
        }
        else
        {
            struct in_addr client_ip_addr;
            memcpy(&client_ip_addr, &client_addr.sin_addr.s_addr, 4);
            Logger_Info(COMPONENTNAME, "Connection established with client %s:%d", inet_ntoa(client_ip_addr), ntohs(client_addr.sin_port));

            PCIC_SetMessageCallback(SIM_SendMessage);
            int result;
            do
            {
                result = recv(clientSocket, in_buf, BUFSIZE, 0);

                if (result > 0)
                {
                    SIM_ParseJSON(in_buf);
                }
                else if (result == 0)
                {
                    PCIC_SetMessageCallback(NULL);
                    closesocket(clientSocket);
                    clientSocket = 0;
                    running = false;
                    Logger_Info(COMPONENTNAME, "Client disconnected gracefully, closing socket");
                }
                else
                {
                    PCIC_SetMessageCallback(NULL);
                    closesocket(clientSocket);
                    clientSocket = 0;
                    int error = WSAGetLastError();
                    Logger_Error(COMPONENTNAME, "*** ERROR - recv() failed (Error: %d)", error);
                    switch (error)
                    {
                        /* Recoverable errors */
                        case WSAENOTCONN: // The socket is not connected.
                        case WSAENOTSOCK: // The descriptor is not a socket.
                        case WSAENETRESET: // For a connection-oriented socket, this error indicates that the connection has been broken due to keep-alive activity that detected a failure while the operation was in progress.
                        case WSAESHUTDOWN: // The socket has been shut down; it is not possible to receive on a socket after shutdown has been invoked with how set to SD_RECEIVE or SD_BOTH.
                        case WSAECONNABORTED: // The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable.
                        case WSAETIMEDOUT: // The connection has been dropped because of a network failure or because the peer system failed to respond.
                        case WSAECONNRESET: // The virtual circuit was reset by the remote side executing a hard or abortive close. The application should close the socket as it is no longer usable. On a UDP-datagram socket, this error would indicate that a previous send operation resulted in an ICMP "Port Unreachable" message.
                        case 0: // This isn't even an error but can occur sometimes if the send function fails

                        break;
                        /* Unrecoverable errors */
                        case WSAEMSGSIZE: // The message was too large to fit into the specified buffer and was truncated.
                        case WSANOTINITIALISED: // A successful WSAStartup call must occur before using this function.
                        case WSAENETDOWN: // The network subsystem has failed.
                        case WSAEFAULT: // The buf parameter is not completely contained in a valid part of the user address space.
                        case WSAEINTR: // The (blocking) call was canceled through WSACancelBlockingCall.
                        case WSAEINPROGRESS: // A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.
                        case WSAEOPNOTSUPP: // MSG_OOB was specified, but the socket is not stream-style such as type SOCK_STREAM, OOB data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only send operations.
                        case WSAEWOULDBLOCK: // The socket is marked as nonblocking and the receive operation would block.
                        case WSAEINVAL: // The socket has not been bound with bind, or an unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled or (for byte stream sockets only) len was zero or negative.
                        default:
                            running = false;
                        break;
                    }
                }
            } while (result > 0);
        }
    }

    Logger_Info(COMPONENTNAME, "Shutting down!");
    fflush(stdout);
    return (SIM_CleanUp());
}

static int SIM_SetupSocket(int portNr)
{
    struct sockaddr_in server_addr;

    WORD wVersionRequested = MAKEWORD(2,2);
    WSADATA wsaData;
    WSAStartup(wVersionRequested, &wsaData);

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == serverSocket)
    {
        Logger_Error(COMPONENTNAME, "%s *** ERROR - socket() failed (error: %ld)", __FUNCTION__, WSAGetLastError());
        WSACleanup();
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNr);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (SOCKET_ERROR == bind(serverSocket, (struct sockaddr *)&server_addr, sizeof(server_addr)))
    {
        Logger_Error(COMPONENTNAME, "%s *** ERROR - bind() failed (error: %ld)", __FUNCTION__, WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (SOCKET_ERROR == listen(serverSocket, 200))
    {
        Logger_Error(COMPONENTNAME, "%s *** ERROR - listen() failed (error: %ld)", __FUNCTION__, WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    return 0;
}

static int SIM_CleanUp()
{
    int retcode = closesocket(serverSocket);
    if (retcode < 0)
    {
        Logger_Error(COMPONENTNAME, "%s *** ERROR - closesocket() failed", __FUNCTION__);
        return 1;
    }
    WSACleanup();
    return 0;
}

static void SIM_SendMessage(const char* messageBuf, int messageLength)
{
    if(clientSocket > 0)
    {
        int bytesSent = send(clientSocket, messageBuf, messageLength, 0);
        if (bytesSent != messageLength)
        {
            Logger_Error(COMPONENTNAME, "*** ERROR - send() failed (expected bytes to send: %d, actual sent: %d)", messageLength, bytesSent);
        }
    }
    else
    {
        Logger_Error(COMPONENTNAME, "SendMessage requested without active client socket!");
    }
}

static int SIM_ParseJSON(const char* messageBuf)
{
    json_error_t error;
    json_t* root = json_loads(messageBuf, JSON_DISABLE_EOF_CHECK, &error);
    if(!root)
    {
        Logger_Error(COMPONENTNAME, "%s *** ERROR - json_loads(..) failed. Line %d: %s\n", __FUNCTION__, error.line, error.text);
        return 1;
    }

    json_t *functionObject, *indexParameterObject, *valueParameterObject;
    functionObject = json_object_get(root, "function_name");
    if(!json_is_string(functionObject))
    {
        Logger_Error(COMPONENTNAME, "%s *** ERROR - function name is not a string\n", __FUNCTION__);
        json_decref(root);
    }

    indexParameterObject = json_object_get(root, "index_parameter");
    if(!json_is_integer(indexParameterObject))
    {
        Logger_Error(COMPONENTNAME, "%s *** ERROR - index parameter is not an integer\n", __FUNCTION__);
        json_decref(root);
        return 1;
    }

    valueParameterObject = json_object_get(root, "value_parameter");
    if(!json_is_string(valueParameterObject))
    {
        Logger_Error(COMPONENTNAME, "%s *** ERROR - value parameter is not a string\n", __FUNCTION__);
        json_decref(root);
        return 1;
    }

    const char* functionName = json_string_value(functionObject);
    int index = (int)json_integer_value(indexParameterObject);
    const char* valueString = json_string_value(valueParameterObject);
    if(NULL != strstr("PCIC_Log", functionName))
    {
        PCIC_Log(valueString);
    }
    else if (NULL != strstr("PCIC_SendFramebufferContents", functionName))
    {
        PCIC_SendFramebufferContents();
    }
    else
    {
        int value = 0;
        sscanf(valueString, "%d", &value);
        PCIC_CallFunction(functionName, index, value);
    }

    return 0;
}
