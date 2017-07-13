#ifndef _SESSION_H_
#define _SESSION_H_

#include "common.h"

typedef struct session
{
	// 控制连接
	uid_t uid;
	int ctrl_fd;//跟客户端建立控制连接的套接字，main accept得到的套接字
	char cmdline[MAX_COMMAND_LINE];//命令行
	char cmd[MAX_COMMAND];
	char arg[MAX_ARG];//a对应ascii,i binary mode,

	// 数据连接
	struct sockaddr_in *port_addr;//客户端发送过来的套接字，用来连接客户端用的（主动模式）
	int pasv_listen_fd;//保存被动模式的套接字（nobody进程中产生），判断是否被动连接，初始值赋为-1
	int data_fd;//ftp服务套接字(ip和端口，用来和客户端进行数据通道连接)(ftp服务进程从nobody进程中得到)
	int data_process;//是否处于数据传输状态，用于时钟超时判断

	// 限速
	unsigned int bw_upload_rate_max;
	unsigned int bw_download_rate_max;
	long bw_transfer_start_sec;
	long bw_transfer_start_usec;


	// 父子进程通道
	int parent_fd;//nobody进程中使用，跟ftp服务进程通信;
	int child_fd;//子进程ftp服务进程使用，跟nobody进程通信;

	// FTP协议状态
	int is_ascii;
	long long restart_pos;//断点续传的位置
	char *rnfr_name;
	int abor_received;

	// 连接数限制
	unsigned int num_clients;
	unsigned int num_this_ip;
} session_t;

void begin_session(session_t *sess);

#endif /* _SESSION_H_ */
