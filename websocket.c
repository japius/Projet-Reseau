#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libwebsockets.h>
#include <signal.h>

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


void sigint_handler(int sig)
{
	interrupted = 1;
}

static int callback_chat(struct lws *wsi, enum lws_callback_reasons reason,
		   void *user, void *in, size_t len){
	switch(reason){

		case LWS_CALLBACK_ESTABLISHED_CLIENT_HTTP:
			printf("je suis bien passé par le formulaire \n");
			break;
		case LWS_CALLBACK_RECEIVE_CLIENT_HTTP_READ:
			printf("Le formulaire a envoyé une donnée");
			//unsigned char *buf=(unsigned char *)malloc(LWS_PRE+len);
			//memset(buf,0,LWS_PRE+len);
			//ON va meme copy ce qu"on veut renvoyer
			//memcpy(buf+LWS_PRE,(char *)in,len);
			//unsigned char *msg=(char *)in;
			//printf("Typed message : %s\n",msg);


		/*case LWS_CALLBACK_HTTP_BODY:

			break;
		case LWS_CALLBACK_HTTP_BODY_COMPLETION:
			break;
		case LWS_CALLBACK_HTTP_DROP_PROTOCOL:
			break;*/
		case LWS_CALLBACK_ESTABLISHED:
			printf("Connection established, with not a http ? \n");
			break;

		case LWS_CALLBACK_RECEIVE:
			printf("Le formulaire a envoyé une donnée");
			unsigned char *buf=(unsigned char *)malloc(LWS_PRE+len);
			memset(buf,0,LWS_PRE+len);
			memcpy(buf+LWS_PRE,(char *)in,len);
			printf("Typed message : %s\n",buf+LWS_PRE);
			lws_write(wsi,buf+LWS_PRE,len,LWS_WRITE_TEXT);


			//ici il faut créer le tlv data et l'envoyer


			
			//ON va meme copy ce qu"on veut renvoyer
			//memcpy(buf+LWS_PRE,(char *)in,len);
			//unsigned char *msg=(char *)in;
			


				//on envoie par le client c;
				break;
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


int main(int argc, const char **argv)
{
	struct lws_context_creation_info info;
	struct lws_context *context;
	const char *p;
	int n = 0, logs = LLL_USER | LLL_ERR | LLL_WARN | LLL_NOTICE;
			/* for LLL_ verbosity above NOTICE to be built into lws,
			 * lws must have been configured and built with
			 * -DCMAKE_BUILD_TYPE=DEBUG instead of =RELEASE */
			/* | LLL_INFO */ /* | LLL_PARSER */ /* | LLL_HEADER */
			/* | LLL_EXT */ /* | LLL_CLIENT */ /* | LLL_LATENCY */
			///* | LLL_DEBUG */;

	signal(SIGINT, sigint_handler);

	if ((p = lws_cmdline_option(argc, argv, "-d")))
		logs = atoi(p);

	lws_set_log_level(logs, NULL);
	lwsl_user("LWS minimal http server | visit http://localhost:7681\n");

	memset(&info, 0, sizeof info); /* otherwise uninitialized garbage */
	info.port = 7681;
	info.mounts = &mount;
	info.protocols=protocols;
	info.vhost_name="localhost";
	info.options =
		LWS_SERVER_OPTION_HTTP_HEADERS_SECURITY_BEST_PRACTICES_ENFORCE;

	context = lws_create_context(&info);
	if (!context) {
		lwsl_err("lws init failed\n");
		return 1;
	}

	while (n >= 0 && !interrupted)
		n = lws_service(context, 1000);

	lws_context_destroy(context);

	return 0;
}





/*int main(void){
	//int port=9000;
  	struct lws_context *context;
  	// we're not using ssl
  	// no special options
  	int opts = 0;
    struct lws_context_creation_info info;
    memset(&info,0,sizeof(info));

    info.port=9000;
    //info.iface=interface;
    info.protocols=protocols;
    info.extensions=NULL;
    //info.ssl_cert_filepath=cert_KLpath;
    //info.ssl_private_key_filepath=key_path;
    info.options=LWS_SERVER_OPTION_HTTP_HEADERS_SECURITY_BEST_PRACTICES_ENFORCE;

  	// create libwebsocket context representing this server
  	context = lws_create_context(&info);
    
  	if (context == NULL) {
    	lwsl_err("libwebsocket init failed\n");
    	return -1;
  	}
    
  	printf("starting server...\n");
    
  	// infinite loop, to end this server send SIGTERM. (CTRL+C)
  	while (1) {
    	lws_service(context, 1000);
    	// libwebsocket_service will process all waiting events with
    	// their callback functions and then wait 50 ms.
    	// (this is a single threaded web server and this will keep
    	// our server from generating load while there are not
    	// requests to process)
  	}
  	  
  	lws_context_destroy(context);
    
  	return 0;
}
*/