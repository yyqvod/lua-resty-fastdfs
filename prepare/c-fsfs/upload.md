#For upload

##变量定义
	char *conf_filename;
	char *local_filename;
	char group_name[FDFS_GROUP_NAME_MAX_LEN + 1];
	ConnectionInfo *pTrackerServer;
>
	typedef struct
	{
		int sock;
		int port;
		char ip_addr[IP_ADDRESS_SIZE];
	} ConnectionInfo;	
>
	int result;
	int store_path_index;
	ConnectionInfo storageServer;
	char file_id[128];