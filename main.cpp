#include "functions.h"

struct Device
{
	string ip; //unique for specifiying source
	int main_pipe_w; //to send msg from main to proc
	string name = ""; //unique & used for clients before setting IP
};

int main(){
	mkdir("pipes", 0777);
	vector<Device> devices;
	while(true){
		string input_str;
		getline(cin,input_str);
		char* input = &input_str[0];
		vector<string> tokens = input_tokenizer(input);
		//////////////client
		if (tokens[0] == "Client"){
			string name = tokens[1], router_ip = tokens[2], router_port = tokens[3];

			struct Device new_client;
			new_client.name = name;
			int main_pipe[2];
			pipe(main_pipe);
			new_client.main_pipe_w = main_pipe[1];
			devices.push_back(new_client);
			int n1 = fork();
			if (n1 == 0){ //child
				execlp("./client.out", &name[0], &router_ip[0], &router_port[0], &(to_string(main_pipe[0]))[0], NULL); 
				exit(0);
			}
			int i;
			for (i=0; i<devices.size(); i++){
				if (devices[i].ip == router_ip)
					break;
			}
			string msg = "Connect Router " + router_ip + " " + router_port;
			write(devices[i].main_pipe_w, &msg[0], strlen(&msg[0]));
		}
		else if ((tokens[0] == "Join" || tokens[0] == "Leave" || tokens[0] == "Select") && tokens[3] == "CN"){
			string ip = tokens[1], group_name = tokens[2];
			int i;
			for (i=0; i<devices.size(); i++){
				if (devices[i].ip == ip)
					break;
			}
			write(devices[i].main_pipe_w, input, strlen(input));
		}
		else if (tokens[0] == "Send" && tokens[1] == "file"){
			string ip = tokens[2], file_name = tokens[3], group_name = tokens[4];
			int i;
			for (i=0; i<devices.size(); i++){
				if (devices[i].ip == ip)
					break;
			}
			write(devices[i].main_pipe_w, input, strlen(input));
		}
		else if (tokens[0] == "Send" && tokens[1] == "message"){
			string ip = tokens[2], message_body = tokens[3], group_name = tokens[4];
			int i;
			for (i=0; i<devices.size(); i++){
				if (devices[i].ip == ip)
					break;
			}
			write(devices[i].main_pipe_w, input, strlen(input));
		}
		else if (tokens[0] == "Show" && tokens[1] == "group"){
			string ip = tokens[2];
			int i;
			for (i=0; i<devices.size(); i++){
				if (devices[i].ip == ip)
					break;
			}
			write(devices[i].main_pipe_w, input, strlen(input));
		}
		else if (tokens[0] == "Sync"){
			string ip = tokens[1];
			int i;
			for (i=0; i<devices.size(); i++){
				if (devices[i].ip == ip)
					break;
			}
			write(devices[i].main_pipe_w, input, strlen(input));
		}
		else if (tokens[0] == "Sign" && tokens[1] == "Out"){
			string ip = tokens[2];
			int i;
			for (i=0; i<devices.size(); i++){
				if (devices[i].ip == ip)
					break;
			}
			write(devices[i].main_pipe_w, input, strlen(input));
		}
		////////////////router
		else if (tokens[0] == "Router"){
			string router_ip = tokens[1];
			
			struct Device new_router;
			new_router.ip = router_ip;
			int main_pipe[2];
			pipe(main_pipe);
			new_router.main_pipe_w = main_pipe[1];
			devices.push_back(new_router);
			int n1 = fork();
			if (n1 == 0){ //child
				execlp("./router.out", &router_ip[0], &(to_string(main_pipe[0]))[0], NULL); 
				exit(0);
			}
		}
		else if (tokens[0] == "Connect"){
			string router1_ip = tokens[1], router2_ip = tokens[2], router1_port = tokens[3], router2_port = tokens[4];
			int i;
			for (i=0; i<devices.size(); i++){
				if (devices[i].ip == router1_ip)
					write(devices[i].main_pipe_w, input, strlen(input));
				if (devices[i].ip == router2_ip)
					write(devices[i].main_pipe_w, input, strlen(input));
			}
		}
		else if (tokens[0] == "Disconnect"){
			string router_ip = tokens[1], router_port = tokens[2];
			int i;
			for (i=0; i<devices.size(); i++){
				if (devices[i].ip == router_ip)
					break;
			}
			write(devices[i].main_pipe_w, input, strlen(input));
		}
		else if (tokens[0] == "Show"){
			string ip = tokens[2];
			int i;
			for (i=0; i<devices.size(); i++){
				if (devices[i].ip == ip)
					break;
			}
			write(devices[i].main_pipe_w, input, strlen(input));
		}
	}
	return 0;
}
