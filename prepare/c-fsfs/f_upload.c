/**
* Copyright (C) 2013 k-49.com
*
* gcc f_upload.c -I /usr/local/fdfs/include/fastdfs/ -I /usr/local/fdfs/include/fastcommon/ -L /usr/local/fdfs/lib/ -l fdfsclient -l fastcommon -I ./ -g -o fb
* fastDFS上传文件，上传库调用实践(直接上传文件)
*
*
*
*
* Copyright (C) 2008 Happy Fish / YuQing
*
* FastDFS may be copied only under the terms of the GNU General
* Public License V3, which may be found in the FastDFS source kit.
* Please visit the FastDFS Home Page http://www.csource.org/ for more detail.
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "fdfs_client.h"
#include "logger.h"

int do_upload(const char *conf_filename, const char *local_filename, const char *pIpAndPort, int store_path_index)
{
     char group_name[FDFS_GROUP_NAME_MAX_LEN + 1];
     ConnectionInfo *pTrackerServer;
     int result;
     ConnectionInfo storageServer;
     char file_id[128];

     log_init();
     g_log_context.log_level = LOG_ERR;

     if ((result=fdfs_client_init(conf_filename)) != 0)
     {
          return result;
     }

     pTrackerServer = tracker_get_connection();
     if (pTrackerServer == NULL)
     {
          fdfs_client_destroy();
          return errno != 0 ? errno : ECONNREFUSED;
     }
     *group_name = '\0';

     if(pIpAndPort != NULL)
     {
          const char *pPort;
          const char *pIpAndPort;

          pPort = strchr(pIpAndPort, ':');
          if (pPort == NULL)
          {
               fdfs_client_destroy();
               fprintf(stderr, "invalid storage ip address and " \
                    "port: %s\n", pIpAndPort);

               return 1;
          }

          storageServer.sock = -1;
          snprintf(storageServer.ip_addr, sizeof(storageServer.ip_addr), \
                "%.*s", (int)(pPort - pIpAndPort), pIpAndPort);
          storageServer.port = atoi(pPort + 1);
     }
     else if ((result=tracker_query_storage_store(pTrackerServer, \
                     &storageServer, group_name, &store_path_index)) != 0)
     {
          fdfs_client_destroy();
          fprintf(stderr, "tracker_query_storage fail, " \
               "error no: %d, error info: %s\n", \
               result, STRERROR(result));
          return result;
     }

     result = storage_upload_by_filename1(pTrackerServer, \
               &storageServer, store_path_index, \
               local_filename, NULL, \
               NULL, 0, group_name, file_id);
     if (result == 0)
     {
          printf("%s\n", file_id);
     }
     else
     {
          fprintf(stderr, "upload file fail, " \
               "error no: %d, error info: %s\n", \
               result, STRERROR(result));
     }

     tracker_disconnect_server_ex(pTrackerServer, true);
     fdfs_client_destroy();

     return result;
}

int main()
{
     char *conf_filename = "/etc/fdfs/client.conf";
     // char *local_filename = "/Users/zhoujing/Pictures/ttt.md";
     char *local_filename = "/Users/zhoujing/Pictures/geek1.png";
     // char *local_filename = "/Users/zhoujing/Pictures/cy.jpeg";
     do_upload(conf_filename, local_filename, NULL, 0);
}