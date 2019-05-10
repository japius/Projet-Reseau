

function get_url(extra_url){
	var protocol="ws"+((location.protocol=="https")?'s':'');
	var rs=protocol + "://"+location.host+"/" + extra_url;
	console.log(rs);
	return rs;
}

function new_ws(urlpath,protocol){
	if(typeof MozWebSocket!="undefined")
		return new MozWebSocket(urlpath,protocol);
	return new WebSocket(urlpath,protocol);
}

function clear(){
	$("#msg").val('');
	$("#msg").focus();
}

$(document).ready(function(){
	//window.WebSocket=window.WebSocket || window.MozWebSocket;
	//var ws=nws_ws
	var ws=new_ws(get_url(""),"chat-protocol");
	ws.onopen=function(event){
			$("#connection").html('Connected');
		};
		ws.onclose=function(event){
			$("#connection").html('Disconnected');
		}
		ws.onmessage=function(message){
			console.log(message.data);
			$("#conv").append($('<p>',{text:message.data}));
		};
		ws.onerror=console.error;

	$('#test').submit(function(event){
			event.preventDefault();
			ws.send($('#msg').val());
			clear();
			//window.location.reload();
		});

	$("#file").submit(function(event){
		event.preventDefault();
		//ws.send($("#fupl").val());
		console.log($("#fupl").val());
		 event.preventDefault()
        var data=new FormData()
        console.log($(event.target).get(0))
        console.log(($(event.target).find("input[type=file]"))[0].files[0])
        var file=($(event.target).find("input[type=file]"))[0].files[0];
        	if(file.size>(1<<16)){
        		//mettre fichier trop grand
        	}
        	else ws.send(file);
        /*for(var i=0;i<files.length;i++){
            var file=files[i]*/
            console.log(file);
            //data.append('imgUploader',file,file.name)
	})
});




//ws.send() envoyer des messages au server
//ou du json

//ws.onmessage reception de pacjet