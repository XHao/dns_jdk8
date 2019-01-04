/*
 * =====================================================================================
 *
 *       Filename:  host.c
 *
 *    Description: print ip by host name
 *
 *        Version:  1.0
 *        Created:  2018/09/20 12:03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  xie hao (xh), hao.xie@ele.me
 *   Organization:  eleme jarch
 *
 * =====================================================================================
 */
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * ===  FUNCTION
 * ====================================================================== Name:
 * main Description:
 * =====================================================================================
 */
int main(int argc, char *argv[]) {

  struct addrinfo hints, *res = NULL;
  char *host = argv[1];
  memset(&hints, 0, sizeof(hints));
  hints.ai_flags = AI_CANONNAME;
  hints.ai_family = AF_INET;

  int getaddrinfo_error = getaddrinfo(host, NULL, &hints, &res);
  if (getaddrinfo_error) {
    printf("fail %s", gai_strerror(getaddrinfo_error));
  } else {
    struct addrinfo *iterator = res;
    char prev[100], addrstr[100];
    void *ptr;
    while (iterator != NULL) {
      if (iterator->ai_family == AF_INET) { /* AF_INET */
        ptr = &((struct sockaddr_in *)iterator->ai_addr)->sin_addr;
      } else {
        ptr = &((struct sockaddr_in6 *)iterator->ai_addr)->sin6_addr;
      }
      inet_ntop(iterator->ai_family, ptr, addrstr, 100);
      if (!strcmp(prev, addrstr))
        printf("Ipv%d is %s/%s\n", iterator->ai_family == AF_INET ? 4 : 6,
               iterator->ai_canonname, addrstr);
      strncpy(prev, addrstr, 100);
      iterator = iterator->ai_next;
    }
  }

  return EXIT_SUCCESS;
} /* ----------  end of function main  ---------- */
