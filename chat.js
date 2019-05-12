

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

function required(fields){//Checks whether every field was filled
        var correct=true;
        $.each(fields,function (index){
        	var field=$(this);
            console.log(field.val())
            if($.trim(field.val())==""){
                field.css({
                    borderColor:'red',
                    color:'black',
                });
                
                /*var errfield=$("#err"+field.attr("name"));
                errfield.html("This field is required");
                errfield.css({
                    display:"inline",
                })*/
                correct=false;
            }
            else{
                field.css({
                    borderColor:'black',
                    borderRadius:'12%'
                });
            }
            console.log(field.val())
        });
        return correct;
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
			console.log(message.data.charAt(0));
			if(message.data.charAt(0)=='0'){
				console.log(message.data.substring(1));
				$("#conv").append("<p>"+message.data.substring(1)+"</p>");
			}
			else{
				$("#conv").append("<img src='"+message.data.substring(1)+"'>");
			}

		};
		ws.onerror=console.error;

	$('#test').submit(function(event){
			event.preventDefault();
			if(required($("#msg, #nn"))){
				var msg=$("#nn").val()+" : "+$('#msg').val();
				if(msg.length>(1<<16)){
					alert("Your message is too long !");
				}
				else{
					ws.send(0+msg);
				}
				clear();
				
			}
			//window.location.reload();
		});

	$("#file").submit(function(event){
		event.preventDefault();
		//ws.send($("#fupl").val());
		console.log($("#fupl").val());
		 event.preventDefault()
        console.log($(event.target).get(0))
        console.log(($(event.target).find("input[type=file]"))[0].files[0])
        var file=($(event.target).find("input[type=file]"))[0].files[0];
        console.log(file.size);
        	if(file.size>(65536)){
        		alert("Your file is too big !");
        		//mettre fichier trop grand
        	}
        	else{
        		imgType=file.type.split('/');
        		imgType=imgType[imgType.length-1];
        		console.log(imgType);
	        	var ext='2';
	        	if(imgType=="jpeg") ext='3';
	        	else if(imgType=="png") ext='4';
	        	else if(imgType=="svg") ext='5';
	        	ws.send(ext+file);
        /*for(var i=0;i<files.length;i++){
            var file=files[i]*/
            console.log(file);
            //data.append('imgUploader',file,file.name)
        	}
        })
        	
});




//ws.send() envoyer des messages au server
//ou du json

//ws.onmessage reception de pacjet