#include "functions.h"

struct Device
{
	string ip; //unique for specifiying source
	int main_pipe_w; //to send msg from main to proc
	string name = ""; //unique & used for clients and group servers before setting IP
};

int main(){
	mkdir("pipes", 0777);
	vector<Device> devices;
	while(true){
		string input_str;
		getline(cin,input_str);
		char* input = &input_str[0];
		vector<string> tokens = input_tokenizer(input);
		//////////////server
		if (tokens[0] == "Server"){
			string server_ip = tokens[1];
			
			struct Device new_server;
			new_server.ip = server_ip;
			int main_pipe[2];
			pipe(main_pipe);
			new_server.main_pipe_w = main_pipe[1];
			devices.push_back(new_server);
			int n1 = fork();
			if (n1 == 0){ //child
				execlp("./server.out", &server_ip[0], &(to_string(main_pipe[0]))[0], NULL); 
				exit(0);
			}
		}
		else if (tokens[0] == "Connect" && tokens[1] == "Router"){ //for server & group server
			string ip = tokens[2], router_ip = tokens[3], router_port = tokens[4];
			int i;
			for (i=0; i<devices.size(); i++){
				if (devices[i].ip == ip)
					write(devices[i].main_pipe_w, input, strlen(input));
				if (devices[i].ip == router_ip)
					write(devices[i].main_pipe_w, input, strlen(input));
			}
		}
		//////////////group server
		else if (tokens[0] == "Group_server"){
			string name = tokens[1], serv_ip = tokens[2];

			struct Device new_group_server;
			new_group_server.name = name;
			int main_pipe[2];
			pipe(main_pipe);
			new_group_server.main_pipe_w = main_pipe[1];
			devices.push_back(new_group_server);
			int n1 = fork();
			if (n1 == 0){ //child
				execlp("./groupserver.out", &name[0], &serv_ip[0], &(to_string(main_pipe[0]))[0], NULL); 
				exit(0);
			}
		}
		else if (tokens[0] == "Set" && tokens[1] == "IP" && tokens[2] == "multicast"){
			string name = tokens[3], ip_multicast = tokens[4];
			int i;
			for(i=0; i<devices.size(); i++){
				if(devices[i].name == name){
					break;
				}
			}
			write(devices[i].main_pipe_w, input, strlen(input));
		}
		else if (tokens[0] == "Set" && tokens[1] == "IP"){ //for group server & client
			string name = tokens[2], ip = tokens[3];
			int i;
			for(i=0; i<devices.size(); i++){
				if(devices[i].name == name){
					devices[i].ip = ip;
					break;
				}
			}
			write(devices[i].main_pipe_w, input, strlen(input));
		}
		else if (tokens[0] == "Add" && tokens[1] == "server"){
			string ip = tokens[2];
			int i;
			for (i=0; i<devices.size(); i++){
				if (devices[i].ip == ip)
					break;
			}
			write(devices[i].main_pipe_w, input, strlen(input));
		}
		//////////////client
		else if (tokens[0] == "Client"){
			string name = tokens[1], server_ip = tokens[2], router_ip = tokens[3], router_port = tokens[4];

			struct Device new_client;
			new_client.name = name;
			int main_pipe[2];
			pipe(main_pipe);
			new_client.main_pipe_w = main_pipe[1];
			devices.push_back(new_client);
			int n1 = fork();
			if (n1 == 0){ //child
				execlp("./client.out", &name[0], &server_ip[0], &router_ip[0], &router_port[0], &(to_string(main_pipe[0]))[0], NULL); 
				exit(0);
			}
			/// ???
			int i;
			for (i=0; i<devices.size(); i++){
				if (devices[i].ip == router_ip)
					break;
			}
			string msg = "Connect Router " + name + " " + router_ip + " " + router_port;
			write(devices[i].main_pipe_w, &msg[0], strlen(&msg[0]));
			//cout << msg << devices[i].ip << "\n";
			///
		}
		else if (tokens[0] == "Get" && tokens[1] == "group" && tokens[2] == "list"){
			string ip = tokens[3];
			int i;
			for(i=0; i<devices.size(); i++){
				if(devices[i].ip == ip){
					break;
				}
			}
			write(devices[i].main_pipe_w, input, strlen(input));
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
		else if (tokens[0] == "ChangeCost"){
			string router_ip = tokens[1], router_port = tokens[2], new_cost = tokens[3];
			int i;
			for (i=0; i<devices.size(); i++){
				if (devices[i].ip == router_ip)
					break;
			}
			write(devices[i].main_pipe_w, input, strlen(input));
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
