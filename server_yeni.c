#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

 #define MYPORT "3490"
 #define BACKLOG 10

 int main()
 {
 
    struct addrinfo *res, *p;
    struct addrinfo hints;
    struct sockaddr_in *ipv4; 
    void* addr;
    int status;
    char ipstr[INET_ADDRSTRLEN];

   memset(&hints, 0, sizeof(hints));
   hints.ai_family = AF_UNSPEC;
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_flags = AI_PASSIVE;

   //getaddrinfo() ile struct'ın adres bilgilerini doldurma

    if( (status = getaddrinfo(NULL, MYPORT, &hints, &res)) != 0){

      fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
      exit(1);
    }

for(p = res; p != NULL; p = p->ai_next){

   if(p->ai_family == AF_INET){

      ipv4 = (struct sockaddr_in *)p->ai_addr;
      addr = &(ipv4->sin_addr);
      //printf("Host IPv4 -> %s \n", inet_ntop(AF_INET, addr, ipstr, sizeof(ipstr)));

   }

}

//socket() ile socket file descriptor oluşturma

int soketfd;

if((soketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){

   fprintf(stderr, "socket error\n");
   exit(1);
}

//bind() ile soketi bir porta bağlama

int baglan = 0;

if( (baglan = bind(soketfd,res->ai_addr,res->ai_addrlen)) == -1){

   fprintf(stderr, "bind error\n");
   exit(1);

}

//listen() ile gelen istekleri bekletme
int dinle;
if( (dinle = listen(soketfd, BACKLOG)) == -1){

   fprintf(stderr, "listen error\n");
   exit(1);

}

//accept() ile gelen istekleri kabul edip her biri için yeni soket oluşturma

int new_socket;
struct sockaddr_storage *new_addr;
socklen_t addr_size = sizeof(struct sockaddr_in);


if( (new_socket = accept(soketfd, (struct sockaddr*)&new_addr, &addr_size)) == -1){

   fprintf(stderr, "accept error \n");
   exit(1);

}


//send() ve recv() ile haberleşme

int gelen;
int giden_bytelar;
char mesaj[900];
int giden_uzunluk;
int uzunluk = 1000;
char buffer[1000];

while(1){

   printf("Mesaj göndermek için '1' e bas. \n");
   scanf("%s", mesaj);
   if(strcmp(mesaj,"1") == 0){ 
      printf("Lütfen bir mesaj girin: ");
      scanf("%s", mesaj);
      giden_uzunluk = strlen(mesaj);
      giden_bytelar = send(new_socket, mesaj, giden_uzunluk, 0);
      printf("Giden byte boyutu: %d \n", giden_bytelar);
   }

   gelen = recv(new_socket, buffer, uzunluk,0);
   if(gelen == 0){
      printf("Session has been closed by the client. \n");
      break;
   }

   printf("Gelen mesaj: %s \n", buffer);
   memset(buffer, 0, sizeof(buffer));
}

freeaddrinfo(res);
 return 0;
 }
