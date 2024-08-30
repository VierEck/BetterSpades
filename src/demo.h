#include "stdio.h"
#include "enet/enet.h"

struct Demo {
	FILE* fp;
	float start_time;
};

FILE* create_demo_file(char server_name[64]);
void register_demo_packet(ENetPacket *packet);
void demo_start_record(char server_name[64]);
void demo_stop_record();