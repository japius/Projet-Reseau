int init_socket_client_udp(char *addr, char *port){
	struct addrinfo h = {0};
	struct addrinfo *r = {0};
	int rc = 0;

	h.ai_family = AF_UNSPEC;
	h.ai_socktype = SOCK_DGRAM;
	h.ai_flags = 0;
	h.ai_protocol = 0;

	rc = getaddrinfo(addr, port, &h, &r);
	if(rc < 0){
		fprintf(stderr, "Erreur lors de getaddrinfo\n");
		return -1;
	}

	int soc = -1;
	rc=-1;
	for(struct addrinfo *p =r; p!=NULL; p = p->ai_next){
		soc = socket(p->ai_family, p->ai_socktype,p->ai_protocol);
		if(soc>=0){
			rc = connect(soc,p->ai_addr,p->ai_addrlen);
			if(rc >= 0) break;
				close(soc);
		}
	}
	freeaddrinfo(r);
	if(rc < 0 ){
		fprintf(stderr, "Ne se connecte à aucune addresse de addrinfo\n");
		return -2;
	}
	return soc;
}

int read_socket(int fd,void *buf, size_t size_buf,struct timeval *timeout){
	fd_set tmp;
	FD_ZERO(&tmp);
	FD_SET(fd,&tmp);
	for(unsigned int count = 0;count<size_buf;){
		if(select(fd+1,&tmp,NULL,NULL,timeout)==0)
			return 0;
		count+=read(fd,buf+count,size_buf-count);
	}
	return 1;
}
