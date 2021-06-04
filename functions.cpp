#include "functions.h"
vector<string> input_tokenizer(char* input){
	istringstream line(input);
	string token;
	vector<string> tokens;
	while(line >> token)
		tokens.push_back(token);
	return tokens;
}

void make_pipe(string router_ip, string router_port, int flag, int& in_fd, int& out_fd){
	const char* myfifo1 = &("pipes/" + router_ip + "-" + router_port + "-in")[0];
	mkfifo(myfifo1, 0666);
	if (flag == 1)
		in_fd = open(myfifo1, O_RDONLY);
	else if (flag == 2)
		in_fd = open(myfifo1, O_WRONLY);
	const char* myfifo2 = &("pipes/" + router_ip + "-" + router_port + "-out")[0];
	mkfifo(myfifo2, 0666);
	if (flag == 1)
		out_fd = open(myfifo2, O_WRONLY);
	else if (flag == 2)
		out_fd = open(myfifo2, O_RDONLY);
}

void add_connection(int connection[][4], int& connection_size, int port_number, int read_fd, int write_fd){
	connection[connection_size][0] = port_number;
	connection[connection_size][1] = read_fd;
	connection[connection_size][2] = write_fd;
	connection[connection_size][3] = 1;
	connection_size++;
}
