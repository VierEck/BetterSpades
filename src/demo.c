/*
Demo recording compatible with aos_replay (https://github.com/BR-/aos_replay)
*/

#include "stdio.h"
#include "demo.h"
#include "time.h"
#include "string.h"
#include "enet/enet.h"
#include "window.h"

struct Demo CurrentDemo;
static const struct Demo ResetStruct;

FILE* create_demo_file(char server_name[64]) {
	char file_name[128];
	
	time_t demo_time;
	time(&demo_time);
	struct tm* tm_info = localtime(&demo_time);
	strftime(file_name, sizeof(file_name), "demos/%Y-%m-%d_%H-%M-%S_", tm_info);

	char* p_char = strchr(server_name, '/');
	while (p_char!=NULL) {
		server_name[p_char-server_name] = ' ';
		p_char = strchr(server_name, '/');
	}

	strncat(file_name, server_name, 64);
	strcat(file_name, ".demo");

	FILE* file;
	file = fopen(file_name, "wb");
	
	// aos_replay version + 0.75 version
	unsigned char value = 1;
	fwrite(&value, sizeof(value), 1, file);

	value = 3;
	fwrite(&value, sizeof(value), 1, file);	
	
	if (file == NULL)
		log_info("Demo failed to create file");

	return file;
}

void register_demo_packet(ENetPacket *packet) {
	if (!CurrentDemo.fp)
		return;

	float c_time = window_time()-CurrentDemo.start_time;
	unsigned short len = packet->dataLength;

	fwrite(&c_time, sizeof(c_time), 1, CurrentDemo.fp);
	fwrite(&len, sizeof(len), 1, CurrentDemo.fp);

	fwrite(packet->data, packet->dataLength, 1, CurrentDemo.fp);
}


void demo_start_record(char server_name[64]) {
	CurrentDemo.fp = create_demo_file(server_name);
	CurrentDemo.start_time = window_time();
	log_info("Demo Recording started.");
}

void demo_stop_record() {
	if(CurrentDemo.fp)
		fclose(CurrentDemo.fp);

	CurrentDemo = ResetStruct;
	log_info("Demo Recording ended.");
}