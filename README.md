Name
====

lua-resty-fastdfs

从去年接触到春哥的openresty后，真正体验到了这个高性能，轻量级的生态系统。于是想fastDFS能不能也有个resty模块呢？把自己的想法跟春哥和鱼大交流后，按照自己的想法开始边学习lua和c边做这个模块。

我的想法：lua-resty-upload处理上传请求，获取请求数据，lua-resty-fastdfs通过调用一个fastDFS的动态链接库直接将文件buffer上传到fastDFS。
lua-resty-fastdfs基本是一个动态链接库的封装。---//这个想法显然是阻塞的

“我觉得你可以直接复用 lua-resty-upload 库来读取下游的 POST 请求体，而不用修改它的代码。你同时需要基于 ngx_lua
的 cosocket API 编写一个 lua-resty-fastdfs 库（类似 lua-resty-redis 那样的），用于向
FastDFS 发送数据。最后你自己在 content_by_lua 里写一点 Lua 代码把二者这么一粘接起来，就可以得到一个严格不缓存的
100% 非阻塞的 FastDFS 代理了。”这是春哥当时给我的建议，因为不懂lua和c，显然没有理解使用cosocket API来做。

*Do.* See [MyBlog](http://k-49.com/tag/lua-resty-fastdfs).

Status
======

目前处于学习，实验阶段。

Version
=======

This document describes fastDFS_lua_nginx [v0.0.01](https://github.com/idevz/lua-resty-fastdfs) released on 14 March 2013.

Synopsis
========


