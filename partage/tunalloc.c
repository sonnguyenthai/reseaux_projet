#include "tunalloc.h"

int tun_alloc(char *dev)
{
  struct ifreq ifr;
  int fd, err;

  if( (fd = open("/dev/net/tun", O_RDWR)) < 0 ){
    perror("alloc tun");
    exit(1);
  }

  memset(&ifr, 0, sizeof(ifr));

  /* Flags: IFF_TUN   - TUN device (no Ethernet headers) 
   *        IFF_TAP   - TAP device  
   *
   *        IFF_NO_PI - Do not provide packet information  
   */ 
  ifr.ifr_flags = IFF_TUN; 
  if( *dev )
    strncpy(ifr.ifr_name, dev, IFNAMSIZ);

  if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ){
    close(fd);
    return err;
  }
  strcpy(dev, ifr.ifr_name);
  return fd;
}      
/*
int main (int argc, char** argv){

  if(argc != 2){
    printf("Usage: %s tun_name\n",argv[0]);
    exit(1);
  }

  int interface_tun ;

  char *tun_zero = argv[1] ;

  interface_tun = tun_alloc(tun_zero);
  // tun_alloc return fd = -1 ??

  printf("interface %s : %d\n", tun_zero, interface_tun);
  
  return 0;
}
*/