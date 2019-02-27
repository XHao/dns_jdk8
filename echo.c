#include <arpa/inet.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int main() {
  int server_fd, client_fd;
  struct sockaddr_in server, client;
  char buf[132];
  int N = 3;
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
    exit(0);
  }

  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port = htons(6666);

  if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
    printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
    exit(0);
  }
  if (listen(server_fd, 128) == -1) {
    printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
    exit(0);
  }

  while (1) {
    socklen_t client_len = sizeof(client);
    client_fd = accept(server_fd, (struct sockaddr *)&client, &client_len);
    if (client_fd < 0)
      printf("Could not establish new connection\n");

    while (1) {
      for (int i = 0; i < N; i++) {
        recv(client_fd, buf, sizeof(buf), 0);
      }
      for (int i = 0; i < N; i++) {
        send(client_fd, buf, sizeof(buf), 0);
      }
    }
  }
  return 0;
}
