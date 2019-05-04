#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <liwebsockets.h>


static int callback_http(struct libwebsocket_context *context,struct libwebsocket *wsi,enum libwebsocket_callback_reasons reason,void *user,void *in,size_t len){
	switch(reason){
		case LWS_CALLBACK_ESTABLISHED:
			printf("okkaay, connected to the web socket ...");
	}
}