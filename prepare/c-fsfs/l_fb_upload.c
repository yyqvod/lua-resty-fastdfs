/**
* Copyright (C) 2013 k-49.com
*
* fastDFS上传文件，上传库调用实践(上传文件file_buffer-生成动态链接库，供lua调用)
* gcc l_fb_upload.c -I /usr/local/fdfs/include/fastcommon/ -I /usr/local/fdfs/include/fastdfs/ -I /usr/local/lua/include/ -L /usr/local/fdfs/lib/ -L /usr/local/lua/lib/ -l fastcommon -l fdfsclient -l lua -g -mstackrealign -fPIC -shared -o fupload.so
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

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "fdfs_client.h"
#include "fdfs_global.h"
#include "base64.h"
#include "sockopt.h"
#include "logger.h"
#include "fdfs_http_shared.h"

int do_bf_upload(char *conf_filename, char *file_content, int64_t file_size, const char *file_ext_name)
{
     ConnectionInfo *pTrackerServer;
     ConnectionInfo *pStorageServer;
     ConnectionInfo storageServer;
     FDFSMetaData meta_list[32];
     int meta_count;
     int store_path_index;
     int storage_count;
     char remote_filename[256];
     char file_id[128];
     char group_name[FDFS_GROUP_NAME_MAX_LEN + 1];
     FDFSFileInfo file_info;
     char szDatetime[20];
     int result;

     log_init();
     g_log_context.log_level = LOG_DEBUG;

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

     pStorageServer = NULL;
     *group_name = '\0';
     store_path_index = 0;

     {
          ConnectionInfo storageServers[FDFS_MAX_SERVERS_EACH_GROUP];
          ConnectionInfo *pServer;
          ConnectionInfo *pServerEnd;
          int storage_count;

          if ((result=tracker_query_storage_store_list_without_group( \
               pTrackerServer, storageServers, \
               FDFS_MAX_SERVERS_EACH_GROUP, &storage_count, \
               group_name, &store_path_index)) == 0)
          {
               printf("tracker_query_storage_store_list_without_group: \n");
               pServerEnd = storageServers + storage_count;
               for (pServer=storageServers; pServer<pServerEnd; pServer++)
               {
                    printf("\tserver %d. group_name=%s, " \
                           "ip_addr=%s, port=%d\n", \
                         (int)(pServer - storageServers) + 1, \
                         group_name, pServer->ip_addr, pServer->port);
               }
               printf("\n");
          }
     }

     if ((result=tracker_query_storage_store(pTrackerServer, \
                     &storageServer, group_name, &store_path_index)) != 0)
     {
          fdfs_client_destroy();
          printf("tracker_query_storage fail, " \
               "error no: %d, error info: %s\n", \
               result, STRERROR(result));
          return result;
     }

     printf("group_name=%s, ip_addr=%s, port=%d\n", \
          group_name, storageServer.ip_addr, \
          storageServer.port);

     if ((pStorageServer=tracker_connect_server(&storageServer, \
          &result)) == NULL)
     {
          fdfs_client_destroy();
          return result;
     }

     memset(&meta_list, 0, sizeof(meta_list));
     meta_count = 0;
     strcpy(meta_list[meta_count].name, "ext_name");
     strcpy(meta_list[meta_count].value, "jpg");
     meta_count++;
     strcpy(meta_list[meta_count].name, "width");
     strcpy(meta_list[meta_count].value, "160");
     meta_count++;
     strcpy(meta_list[meta_count].name, "height");
     strcpy(meta_list[meta_count].value, "80");
     meta_count++;
     strcpy(meta_list[meta_count].name, "file_size");
     strcpy(meta_list[meta_count].value, "115120");
     meta_count++;

     printf("------pTrackerServer-------%s\n", pTrackerServer->ip_addr);
     printf("------pStorageServer-------%s\n", pStorageServer->ip_addr);
     printf("------group_name-------%s\n", group_name);
     printf("------store_path_index-------%d\n", store_path_index);
     printf("------file_size-------%d\n", file_size);
     printf("------file_ext_name-------%s\n", file_ext_name);
     printf("------remote_filename-------%s\n", remote_filename);
     printf("------file_content-------%s\n", file_content);

     result = storage_upload_by_filebuff(pTrackerServer, \
          pStorageServer, store_path_index, \
          file_content, file_size, file_ext_name, \
          meta_list, meta_count, \
          group_name, remote_filename);
     // free(file_content);

     printf("-------------%s\n", "go...");

     printf("storage_upload_by_filebuff\n");
     sprintf(file_id, "%s/%s", group_name, remote_filename);

     if (result != 0)
     {
          printf("upload file fail, " \
               "error no: %d, error info: %s\n", \
               result, STRERROR(result));
          tracker_disconnect_server_ex(pStorageServer, true);
          fdfs_client_destroy();
          return result;
     }

     printf("group_name=%s, remote_filename=%s\n", \
               group_name, remote_filename);


     fdfs_get_file_info(group_name, remote_filename, &file_info);
     printf("source ip address: %s\n", file_info.source_ip_addr);
     printf("file timestamp=%s\n", formatDatetime(
          file_info.create_timestamp, "%Y-%m-%d %H:%M:%S", \
          szDatetime, sizeof(szDatetime)));
     printf("file size="INT64_PRINTF_FORMAT"\n", file_info.file_size);
     printf("file crc32=%u\n", file_info.crc32);


     tracker_disconnect_server_ex(pStorageServer, true);
     tracker_disconnect_server_ex(pTrackerServer, true);

     fdfs_client_destroy();

     return result;
}

int fb_upload_lua(lua_State* L)
{
     char *conf_filename = luaL_checkstring(L,1);
     char *file_content = luaL_checkstring(L,2);
     const char *file_ext_name = luaL_checkstring(L,3);
     int64_t file_size = luaL_checknumber(L,4);

     do_bf_upload(conf_filename, file_content, file_size, file_ext_name);
     return 1;
}

static luaL_Reg mylibs[] = {
     {"fb_upload_lua", fb_upload_lua},
     {NULL, NULL}
};

int luaopen_fbupload(lua_State* L)
{
     const char* libName = "fbupload";
     luaL_register(L,libName,mylibs);
     return 1;
}