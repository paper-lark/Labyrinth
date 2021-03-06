/**********************************************************************************
 * Copyright (c) 2017 Max Zhuravsky                                               *
 *                                                                                *
 * Permission is hereby granted, free of charge, to any person obtaining a copy   *
 * of this software and associated documentation files (the "Software"), to deal  *
 * in the Software without restriction, including without limitation the rights   *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      *
 * copies of the Software, and to permit persons to whom the Software is          *
 * furnished to do so, subject to the following conditions:                       *
 *                                                                                *
 * The above copyright notice and this permission notice shall be included in all *
 * copies or substantial portions of the Software.                                *
 *                                                                                *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  *
 * SOFTWARE.                                                                      *
 **********************************************************************************/

/**********************************************************************************
 *                                 Labyrinth Game                                 *
 *                  Author:  Max Zhuravsky <paperlark@yandex.ru>                  *
 **********************************************************************************/

/* Headers */
#include "../connect.h"
#include "../menu.h"
#include <stdio.h>
#include <string.h>

/* Implementation */
void WSAShowLastError(WINDOW *info_scene) {
    wchar_t *msg = NULL;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
                   NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&msg, 0, NULL);
    wmove(info_scene, 0, 0);
    wprintf(L"ERROR: %S\n", msg);
    LocalFree(msg);
}

USock create_server(WINDOW *info_scene) {
    /* Init winsock */
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        show_info(info_scene, Left, Error, "WSAStartup() error");
        return USOCK_ERROR;
    }

    /* Create info */
    struct addrinfo hints, *serverinfo;
    memset(&hints, 0, sizeof hints);
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo(NULL, PORT, &hints, &serverinfo) != 0) {
        show_info(info_scene, Left, Error, "getaddrinfo() error");
        return USOCK_ERROR;
    }

    /* Create socket and bind */
    USock serverfd = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol);
    if (serverfd == INVALID_SOCKET) {
        show_info(info_scene, Left, Error, "socket() error");
        return USOCK_ERROR;
    }
    if (bind(serverfd, serverinfo->ai_addr, serverinfo->ai_addrlen) == SOCKET_ERROR) { //::ToDo Iterate through all entries
        show_info(info_scene, Left, Error, "bind() error");
        close_usocket(serverfd);
        return USOCK_ERROR;
    }
    freeaddrinfo(serverinfo);

    /* Listen */
    struct sockaddr_storage clientinfo;
    int size = sizeof clientinfo;
    listen(serverfd, 1);
    show_info(info_scene, Center, Message, "Waiting for incoming connection...");

    /* Accept */
    USock clientfd = accept(serverfd, (struct sockaddr *)&clientinfo, &size);
    if (clientfd == INVALID_SOCKET) {
        show_info(info_scene, Left, Error, "accept() error");
        close_usocket(serverfd);
        return USOCK_ERROR;
    }
    closesocket(serverfd);

    return clientfd;
}

USock create_client(char *ip, WINDOW *info_scene) {
    /* Init winsock */
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        show_info(info_scene, Left, Error, "WSAStartup() error");
        return USOCK_ERROR;
    }

    /* Create info */
    show_info(info_scene, Center, Message, "Connecting...");
    struct addrinfo hints, *serverinfo;
    memset(&hints, 0, sizeof hints);
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;
    if (getaddrinfo(ip, PORT, &hints, &serverinfo) != 0) {
        show_info(info_scene, Left, Error, "getaddrinfo() error");
        return USOCK_ERROR;
    }

    /* Create socket */
    USock sockfd = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol);
    if (sockfd == INVALID_SOCKET) {
        show_info(info_scene, Left, Error, "socket() error");
        return USOCK_ERROR;
    }

    /* Connect */
    if (connect(sockfd, serverinfo->ai_addr, serverinfo->ai_addrlen) == SOCKET_ERROR) {
        show_info(info_scene, Left, Error, "connect() error");
        close_usocket(sockfd);
        return USOCK_ERROR;
    }
    freeaddrinfo(serverinfo);

    return sockfd;
}

void close_usocket(USock sockfd) {
    closesocket(sockfd);
    WSACleanup();
}