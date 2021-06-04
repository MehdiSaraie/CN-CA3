#include "functions.h"

struct Server
{
	string ip; //unique
	int main_pipe_w; //to send msg from main to proc
};

int main(){
	mkdir("pipes", 0777);
	struct Server server;
	while(true){
		string input_str;
		getline(cin,input_str);
		char* input = &input_str[0];
		vector<string> tokens = input_tokenizer(input);

		if (tokens[0] == "Server"){
			string server_ip = tokens[1];
			
			server.ip = server_ip;
			int main_pipe[2];
			pipe(main_pipe);
			server.main_pipe_w = main_pipe[1];

			int n1 = fork();
			if (n1 == 0){ //child
				execlp("./client.out", &server_ip[0], &(to_string(main_pipe[0]))[0], NULL); 
				exit(0);
		}
		else if (tokens[0] == "Connect" && tokens[1] == "Router"){
			string router_port = tokens[2];
			write(server.main_pipe_w, input, strlen(input));
		}
	}
	return 0;
}
