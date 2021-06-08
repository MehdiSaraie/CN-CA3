#include "functions.h"
int main(int argc, char* argv[]){
	string name = argv[0], router_ip = argv[1], router_port = argv[2];
	int main_pipe_r = atoi(argv[3]);
	char buffer[LENGTH];
	vector<string> group_names;
	int from_router_fd = 0, to_router_fd = 0;
	int max_fd, activity;
	fd_set readfds;
	
	make_pipe(router_ip, router_port, 2, to_router_fd, from_router_fd);
	cout << "yes\n";

	while(true){
		FD_ZERO(&readfds);

		FD_SET(main_pipe_r, &readfds);
		max_fd = main_pipe_r;
		if (from_router_fd > 0)
			FD_SET(from_router_fd, &readfds);
		if (from_router_fd > max_fd)
			max_fd = from_router_fd;
		 
		activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
		if ((activity < 0) && (errno!=EINTR)){
			cerr << ("select error\n");
		}

		if (FD_ISSET(main_pipe_r, &readfds)){ //msg from main
			memset(&buffer, 0, LENGTH);
			read(main_pipe_r, buffer, LENGTH);
			puts(buffer);
			vector<string> tokens = input_tokenizer(buffer);
			
			if (tokens[0] == "Join"){
				string ip = tokens[1], group_name = tokens[2];
				group_names.push_back(group_name);
			}
			
			else if (tokens[0] == "Leave"){
				string ip = tokens[1], group_name = tokens[2];
				for (int i = 0; i < group_names.size(); i++){
					if (group_names[i] == group_name){
						for (int j = i+1; j < group_names.size(); j++)
							group_names[j-1] = group_names[j];
						break;
					}
				}
			}
			
			else if (tokens[0] == "Select"){ //replace oldest group by new group
				string ip = tokens[1], group_name = tokens[2];
				if (group_names.size() == 0)
					group_names.push_back(group_name);
				else{
					int i = 1;
					for (i = 1; i < group_names.size(); i++)
						group_names[i-1] = group_names[i];
					group_names[i-1] = group_name;
				}
			}
			
			else if (tokens[0] == "Show" && tokens[1] == "group"){
				string ip = tokens[2];
				for (int i = 0; i < group_names.size(); i++)
					cout << group_names[i] << "    ";
				cout << endl;
			}
		}
		
		if (FD_ISSET(from_router_fd, &readfds)){ //msg from router
        	memset(&buffer, 0, LENGTH);
			read(from_router_fd, buffer, LENGTH);
			
		}
	}
	return 0;
}

