

int init_socket_client_udp(char *addr, char *port);
int read_socket(int fd,void *buf, size_t size_buf,struct timeval *timeout);
void random_on_octets(void *var, size_t octets_number);