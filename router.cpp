#include "functions.h"
int main(int argc, char* argv[]){
	string router_ip = argv[0];
	int main_pipe_r = atoi(argv[1]);
	
	//int lookup[LENGTH][2];//dest system + port
	//int lookup_size = 0;
	int connection[LENGTH][4];//port + read_fd + write_fd (for all connected switches or systems) + cost
	int connection_size = 0;
	int i, j, k, fd, max_fd, activity;
	fd_set readfds;
	char buffer[LENGTH];
	vector<vector<string>> learning(LENGTH);
	map<string, int> sourceAddr; //group_name + write_fd
	map<string, int>::iterator itr;

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
				cout << router_ip << " :: " << buffer << endl; 
				vector<string> tokens = input_tokenizer(buffer);
				
				if (tokens[0] == "Disconnect"){
					close(connection[i][1]);
					close(connection[i][2]);
					for (j = i+1; j < connection_size; j++){
						for (k = 0; k < 4; k++)
							connection[j-1][k] = connection[j][k];
					}
					connection_size--;
				}
				
				else if (tokens[0] == "datagram"){
					string group_name = tokens[1];
					itr = sourceAddr.find(group_name);
					
					if (itr != sourceAddr.end()){
						itr->second = connection[i][2];
						
						for(j = 0; j < connection_size; j++){
							if (connection[j][1] == src_fd)
								continue;
							int port_num = connection[j][0];
							for(k = 0; k < learning[port_num].size(); k++){
								if(learning[port_num][k] == group_name){
									int dest_fd = connection[j][2];
									write(dest_fd, buffer, strlen(buffer));
								}
							}
						}
					}
					
					else{ //first message of a group received by rooter
						sourceAddr.insert(pair<string, int>(group_name, connection[i][2]));
						for (j = 0; j < connection_size; j++){ //broadcast
							learning[connection[j][0]].push_back(group_name);
							if (connection[j][1] != src_fd){
								int dest_fd = connection[j][2];
								write(dest_fd, buffer, strlen(buffer));
							}
						}
					}
				}
				
				else if (tokens[0] == "prune"){
					string group_name = tokens[1];
					for (auto p = learning[src_port].begin(); p != learning[src_port].end(); ++p){ //find & erase group_name from port
						if (*p == group_name) {
							learning[src_port].erase(p);
							p--;
							break;
						}
					}
					
					int found = false;
					itr = sourceAddr.find(group_name);
					for(j = 0; j < connection_size; j++){ //check if anybody needs message of group
						if (connection[j][2] == itr->second)
							continue;
						int port_num = connection[j][0];
						for(k = 0; k < learning[port_num].size(); k++){
							if(learning[port_num][k] == group_name){
								found = true;
								break;
							}
						}
					}
					if (!found) //send prune message upward
						write(itr->second, buffer, strlen(buffer));
				}
				
			}
		}
	}
	return 0;
}

