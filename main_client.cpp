#include "functions.h"

struct Client
{
	string name; //unique
	int main_pipe_w; //to send msg from main to proc
};

int main()
{
	vector<Client> clients;
	while(true){
		string input_str;
		getline(cin,input_str);
		char* input = &input_str[0];
		vector<string> tokens = input_tokenizer(input);

		if (tokens[0] == "Client"){
			string name = tokens[1], server_ip = tokens[2], router_ip = tokens[3], router_port = tokens[4];

			struct Client new_client;
			new_client.name = name;
			int main_pipe[2];
			pipe(main_pipe);
			new_client.main_pipe_w = main_pipe[1];
			clients.push_back(new_client);

			int n1 = fork();
			if (n1 == 0){ //child
				execlp("./client.out", &name[0], &server_ip[0], &router_ip[0], &router_port[0], &(to_string(main_pipe[0]))[0], NULL); 
				exit(0);
			}
		}
		else if (tokens[0] == "Set" && tokens[1] == "IP"){
			string name = tokens[2], ip = tokens[3];
			int i;
			for(i=0; i<clients.size(); i++){
				if(clients[i].name == name){
					break;
				}
			}
			write(clients[i].main_pipe_w, input, strlen(input));
		}
		//
	}
	return 0;
}
