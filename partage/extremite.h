#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h> 
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <netdb.h>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <pwd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <netdb.h>
#include "tunalloc.h"

#define MAXLIGNE 80
#define MAX_MTU 1500
#define MAX_LENGTH_IP 60


void ext_out();
void ext_in(int tun_fd_in);