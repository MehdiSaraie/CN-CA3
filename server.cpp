#include "functions.h"
int main(int argc, char* argv[]){
	string server_ip = argv[0];
	int main_pipe_r = atoi(argv[1]);
	char buffer[LENGTH];
	int from_router_fd = 0, to_router_fd = 0;
	int max_fd, activity;
	fd_set readfds;

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
			
			if(tokens[0] == "Connect" && tokens[1] == "Router"){
				string ip = tokens[2], router_ip = tokens[3], router_port = tokens[4];
				make_pipe(router_ip, router_port, 2, to_router_fd, from_router_fd);
			}
		}
		
		if (FD_ISSET(from_router_fd, &readfds)){ //msg from router
        	memset(&buffer, 0, LENGTH);
			read(from_router_fd, buffer, LENGTH);
			
		}
	}
	return 0;
}

