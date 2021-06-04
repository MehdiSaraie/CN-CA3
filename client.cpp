#include "functions.h"
int main(int argc, char* argv[]){
	string name = argv[0], server_ip = argv[1], router_ip = argv[2], router_port = argv[3];
	int main_pipe_r = atoi(argv[4]);
	char buffer[LENGTH];
	vector<string> group_names;
	int from_router_fd = 0, to_router_fd = 0;
	int max_fd, activity;
	fd_set readfds;
	/// ???
	make_pipe(router_ip, router_port, 2, to_router_fd, from_router_fd);
	///
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
			
			if (tokens[0] == "Show" && tokens[1] == "group"){
				string ip = tokens[2];
				for (int i = 0; i< group_names.size(); i++)
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

