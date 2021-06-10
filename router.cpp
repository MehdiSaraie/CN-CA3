#include "functions.h"
int main(int argc, char* argv[]){
	string router_ip = argv[0];
	int main_pipe_r = atoi(argv[1]);
	
	//int lookup[LENGTH][2];//dest system + port
	//int lookup_size = 0;
	int connection[LENGTH][4];//port + read_fd + write_fd (for all connected switches or systems) + cost
	int connection_size = 0;
	int i, j, fd, max_fd, activity;
	fd_set readfds;
	char buffer[LENGTH];
	vector<vector<int>> learning(LENGTH);
	int sourceAddr[LENGTH]; //group_name + write_fd

	while(true){
		FD_ZERO(&readfds);

		FD_SET(main_pipe_r, &readfds);
		max_fd = main_pipe_r;
		for (i = 0; i < connection_size; i++){
			fd = connection[i][1];
			if (fd > 0)
				FD_SET(fd, &readfds);
			if (fd > max_fd)
				max_fd = fd;
		}
		 
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
				string router_ip = tokens[2], router_port = tokens[3];
				int in_fd, out_fd;
				make_pipe(router_ip, router_port, 1, in_fd, out_fd);
				add_connection(connection, connection_size, stoi(router_port), in_fd, out_fd);
			}
			
			else if (tokens[0] == "Connect"){
				string router1_ip = tokens[1], router2_ip = tokens[2], router1_port = tokens[3], router2_port = tokens[4];
				if (router_ip == router1_ip){
					int in_fd, out_fd;
					make_pipe(router1_ip, router1_port, 1, in_fd, out_fd);
					add_connection(connection, connection_size, stoi(router1_port), in_fd, out_fd);
				}
				else if (router_ip == router2_ip){
					int in_fd, out_fd;
					make_pipe(router1_ip, router1_port, 2, in_fd, out_fd);
					add_connection(connection, connection_size, stoi(router2_port), out_fd, in_fd);
				}
			}
			
			else if (tokens[0] == "Disconnect"){
				string router_ip = tokens[1], router_port = tokens[2];
				for (i = 0; i < connection_size; i++){
					if (connection[i][0] == stoi(router_port)){
						string msg = "Disconnect";
						write(connection[i][2], &msg[0], strlen(&msg[0]));
						close(connection[i][1]);
						close(connection[i][2]);
						for (j = i+1; j < connection_size; j++){
							for (int k = 0; k < 4; k++)
								connection[j-1][k] = connection[j][k];
						}
						connection_size--;
						break;
					}
				}
			}
		}
		
		for (i = 0; i < connection_size; i++){ //msg from a device
			int src_fd = connection[i][1];
			int src_port = connection[i][0];
			if (FD_ISSET(src_fd, &readfds)){
				memset(&buffer, 0, LENGTH);
				read(src_fd, buffer, LENGTH);
				puts(buffer);
				vector<string> tokens = input_tokenizer(buffer);
				
				if (tokens[0] == "Disconnect"){
					close(connection[i][1]);
					close(connection[i][2]);
					for (j = i+1; j < connection_size; j++){
						for (int k = 0; k < 4; k++)
							connection[j-1][k] = connection[j][k];
					}
					connection_size--;
				}
				if (tokens[0] == "datagram"){
					int group_name = stoi(tokens[1]);
					sourceAddr[group_name] = connection[i][2];
					for (j = 0; j < connection_size; j++){ //broadcast
						if (connection[j][1] != src_fd){
							int dest_fd = connection[j][2];
							learning[connection[j][0]].push_back(group_name);
							write(dest_fd, buffer, LENGTH);
						}
					}
				}
				if (tokens[0] == "prune"){
					int group_name = stoi(tokens[1]);
					int port = connection[i][0];
					for (auto j = learning[port].begin(); j != learning[port].end(); ++j){
						if (*j == group_name) {
							learning[port].erase(j);
							j--;
						}
					}
					int found = 0;
					for(j =0; j<learning.size(); j++)
						for(int k =0; k<learning[j].size(); k++){
							if(learning[j][k] == group_name){
								found = 1;
								break;
							}
						}
					
					if (found == 0){
						string msg = "prune " + to_string(group_name);
						write(sourceAddr[group_name], &msg[0], LENGTH);
					}
				}
				if (tokens[0] == "SendMsg" || tokens[0] == "SendFile"){
					int group_name = stoi(tokens[1]);
					for(j =0; j<learning.size(); j++)
						for(int k =0; k<learning[j].size(); k++){
							if(learning[j][k] == group_name){
								int dest_fd = connection[j][2];
								write(dest_fd, buffer, LENGTH);
							}
						}
				}
			}
		}
	}
	return 0;
}

