#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  char sndBuf[132] = "hello world!";
  char rcvBuf[132];
  int N = 3;
  struct sockaddr_in addr;
  int fd;

  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
    exit(0);
  }

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(6666);
  if (argc < 1 || inet_pton(AF_INET, argv[1], &addr.sin_addr) <= 0) {
    printf("input remote host");
    exit(0);
  }

  setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (int[]){1}, sizeof(int));
  if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    printf("connect error: %s(errno: %d)\n", strerror(errno), errno);
    exit(0);
  }

  while (1) {

    for (int i = 0; i < N; i++) {
      send(fd, sndBuf, sizeof(sndBuf), 0);
      printf("send time %i\n", i);
    }

    for (int i = 0; i < N; i++) {
      recv(fd, rcvBuf, sizeof(rcvBuf), 0);
      setsockopt(fd, IPPROTO_TCP, TCP_QUICKACK, (int[]){1}, sizeof(int));
      printf("recv time %i\n", i);
    }

    printf("sleep 1\n");
    sleep(1);
  }

  close(fd);
  return 0;
}
