#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char * * argv)
{
    struct sockaddr_in cli;
  char host[128];
  char service[128];
  int flags;
  int err;
    
  cli.sin_family = AF_INET; /* IPv4 */
  cli.sin_port = htons(2049); /* the port of NFS */
  inet_pton(AF_INET, "127.0.0.1", &cli.sin_addr.s_addr); /* "220.181.111.86" is the address of baidu.com */
  flags = 0; /* NI_NUMERICHOST | NI_NUMERICSERV */
    
  err = getnameinfo((struct sockaddr *)(&cli), sizeof(struct sockaddr),
                  host, sizeof(host), service, sizeof(service), flags);
  if(err != 0)
  {
      gai_strerror(err);
      exit(-1);
  }
  printf("host=%s, serv=%s\n", host, service);
        
    return 0;
}
