#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <map>

using namespace std;

#define LENGTH 1024

vector<string> input_tokenizer(char* input);
void make_pipe(string router_ip, string router_port, int flag, int& in_fd, int& out_fd);
void add_connection(int connection[][4], int& connection_size, int port_number, int read_fd, int write_fd);
void WriteInFile(string name, string input);
vector<string> read_file_chunk(string file_name);
