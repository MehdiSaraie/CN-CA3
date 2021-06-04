#include "functions.h"

struct GroupServer
{
	string name; //unique
	int main_pipe_w; //to send msg from main to proc
};

int main()
{
	vector<GroupServer> group_servers;
	//mkdir("files", 0777);
	while(true){
		string input_str;
		getline(cin,input_str);
		char* input = &input_str[0];
		vector<string> tokens = input_tokenizer(input);

		if (tokens[0] == "Group_server"){
			string name = tokens[1], server_ip = tokens[2];

			struct GroupServer new_group_server;
			new_group_server.name = name;
			int main_pipe[2];
			pipe(main_pipe);
			new_group_server.main_pipe_w = main_pipe[1];
			group_servers.push_back(new_group_server);

			int n1 = fork();
			if (n1 == 0){ //child
				execlp("./groupserver.out", &name[0], &server_ip[0], &(to_string(main_pipe[0]))[0], NULL); 
				exit(0);
			}
		}
		else if (tokens[0] == "Set" && tokens[1] == "IP"){
			string name = tokens[2], ip = tokens[3];
			int i;
			for(i=0; i<group_servers.size(); i++){
				if(group_servers[i].name == name){
					break;
				}
			}
			write(group_servers[i].main_pipe_w, input, strlen(input));
		}
		//
	}
	return 0;
}
