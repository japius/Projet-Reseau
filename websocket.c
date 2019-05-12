#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libwebsockets.h>
#include <signal.h>
#include <fcntl.h>
#include "peer.h"

#define LWS_PLUGIN_STATIC


static int interrupted;

static const struct lws_http_mount mount = {
	/* .mount_next */		NULL,		/* linked-list "next" */
	/* .mountpoint */		"/",		/* mountpoint URL */
	/* .origin */			".", /* serve from dir */
	/* .def */			"test.html",	/* default filename */
	/* .protocol */			NULL,
	/* .cgienv */			NULL,
	/* .extra_mimetypes */		NULL,
	/* .interpret */		NULL,
	/* .cgi_timeout */		0,
	/* .cache_max_age */		0,
	/* .auth_mask */		0,
	/* .cache_reusable */		0,
	/* .cache_revalidate */		0,
	/* .cache_intermediaries */	0,
	/* .origin_protocol */		LWSMPRO_FILE,	/* files in a dir */
	/* .mountpoint_len */		1,		/* char count */
	/* .basic_auth_login_file */	NULL,
};

struct myupd_fd{
	int filefd;
};


void sigint_handler(int sig)
{
	interrupted = 1;
}

static int callback_chat(struct lws *wsi, enum lws_callback_reasons reason,
		   void *user, void *in, size_t len){
	unsigned char buffer[LWS_PRE+500];
	struct myupd_fd *vhd=(struct myupd_fd *)lws_protocol_vh_priv_get(lws_get_vhost(wsi),lws_get_protocol(wsi));
	lws_sock_file_fd_type u;
	switch(reason){
		case LWS_CALLBACK_ESTABLISHED:
			vhd=lws_protocol_vh_priv_zalloc(lws_get_vhost(wsi),lws_get_protocol(wsi),sizeof(struct myupd_fd));
			vhd->filefd=FD_MAGIC_READ;
			u.filefd=(lws_filefd_type)(long long)vhd->filefd;
			if (!lws_adopt_descriptor_vhost(lws_get_vhost(wsi),LWS_ADOPT_RAW_FILE_DESC, u,
						"chat-protocol", wsi)) {
				lwsl_err("Failed to adopt descriptor\n");
				close(vhd->filefd);
				vhd->filefd = -1;
				return 1;
			}
			printf("%d",vhd->filefd);
			printf("Connection established, with not a http ? \n");
		break;


		case LWS_CALLBACK_RECEIVE:
			printf("");
			unsigned char *buf=(unsigned char *)malloc(LWS_PRE+len);
			if(buf==NULL){
				return 1;
			}
			memset(buf,0,LWS_PRE+len);
			memcpy(buf+LWS_PRE,(char *)in,len);
			free(buf);
			write(FD_MAGIC_WRITE,(char *)in,len);
		break;

		case LWS_CALLBACK_RAW_ADOPT_FILE:
			lwsl_notice("LWS_CALLBACK_RAW_ADOPT_FILE\n");
		break;

		//Quand on a une écriture sur FD_MAGIC_READ
		case LWS_CALLBACK_RAW_RX_FILE:
			memset(buffer,0,LWS_PRE+len);
			struct lws *pwsi=lws_get_parent(wsi);
			lwsl_notice("LWS_CALLBACK_RAW_RX_FILE\n");
			int n=read(vhd->filefd,buffer+LWS_PRE,sizeof(buffer));
			if(n<0){
				lwsl_err("Reading from my fd failed\n");
				return 1;
			}
			lws_write(pwsi,buffer+LWS_PRE,n,LWS_WRITE_TEXT);
		break;

		//case LWS_CALLBACK_RAW_WRITEABLE_FILE:
		//	lwsl_notice("LWS_CALLBACK_RAW_WRITEABLE_FILE");


		/*case LWS_CALLBACK_PROTOCOL_DESTROY:
			if (vhd && vhd->filefd != -1)
				close(vhd->filefd);
		break;*/

		default:

			printf("unhandled\n");
			break;
	}
	return 0;
}


//list of supported protocols and callbacks
static struct lws_protocols protocols[] = {
	//always HTTP handler
	{
		"http-only",
		lws_callback_http_dummy,
		0,0
	},
	{
		"chat-protocol",
		callback_chat,
		0,0
	},

	{
		NULL,NULL,0,0
	},
};


void handle_gui()
{
	int fd[2],fd2[2];
	pipe(fd);
	pipe(fd2);
	if(fork()==0){
		close(fd[0]);
		close(fd2[1]);
		FD_MAGIC_READ=fd2[0];
		FD_MAGIC_WRITE=fd[1];
		return ;
	}
	close(fd[1]);
	close(fd2[0]);
	FD_MAGIC_READ=fd[0];
	FD_MAGIC_WRITE=fd2[1];
	struct lws_context_creation_info info;
	struct lws_context *context;
	int n = 0, logs = LLL_USER | LLL_ERR | LLL_WARN | LLL_NOTICE;
			/* for LLL_ verbosity above NOTICE to be built into lws,
			 * lws must have been configured and built with
			 * -DCMAKE_BUILD_TYPE=DEBUG instead of =RELEASE */
			/* | LLL_INFO */ /* | LLL_PARSER */ /* | LLL_HEADER */
			/* | LLL_EXT */ /* | LLL_CLIENT */ /* | LLL_LATENCY */
			///* | LLL_DEBUG */;

	//signal(SIGINT, sigint_handler);

	//if ((p = lws_cmdline_option(argc, argv, "-d")))
	//	logs = atoi(p);

	lws_set_log_level(logs, NULL);
	lwsl_user("LWS minimal http server | visit http://localhost:8000\n");

	memset(&info, 0, sizeof info); /* otherwise uninitialized garbage */
	info.port = 8000;
	info.mounts = &mount;
	info.protocols=protocols;
	info.vhost_name="localhost";
	info.options =
		LWS_SERVER_OPTION_HTTP_HEADERS_SECURITY_BEST_PRACTICES_ENFORCE;

	context = lws_create_context(&info);
	if (!context) {
		lwsl_err("lws init failed\n");
		return ;
	}
	while (n >= 0 && !interrupted)
		n = lws_service(context,1000 );
	lws_context_destroy(context);

}

