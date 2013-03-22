--[[
nginx处理上传请求，调用lua-resty-upload模块读取请求体，将读取的文件buffer连接为一个整体，调用fastDFS的client动态链接库，
将文件发送给fastDFS，并返回相应的上传结果remote_filename,ip_addr,port,file_size等.
]]--

local upload = require "resty.upload"
local fbupload = require("fbupload")

local chunk_size = 40960
local form = upload:new(chunk_size)
local function my_get_file_name( res )
    if res[2] and string.find(res[2], 'filename') then
      local filename = string.match(res[2], 'filename="(.*)"')
      if filename then
        return filename
      end
    end
end

local function getextension(filename)
    return filename:match(".+%.(%w+)$")
end

local file_name,content
while true do
    local typ, res, err = form:read()

    if not typ then
         ngx.say("failed to read: ", err)
         return
    end

    if typ == "header" then
        if file_name == nil then
            file_name = my_get_file_name(res)
        end
     elseif typ == "body" then
        -- ngx.say("<br />--------b--------<br />")
        if content == nil then
            content = res
        else
            content = content .. res
        end
    elseif typ == "part_end" then
        -- do nothing
    elseif typ == "eof" then
        ngx.say("ok...")
        break
    else
        -- do nothing
    end
end

if file_name == nil then
    file_name = "vvvvvvvrrrrrrr.jpg"
end

-- local get_pic = io.open('/Users/zjngx/upload/' .. file_name,"w")
-- get_pic:write(content)
-- get_pic:close()
-- ngx.say("<br />done...")

local conf_filename = "/etc/fdfs/client.conf"
local file_ext_name = getextension(file_name)
local file_size = string.len(content)

-- ngx.say(file_ext_name,file_size)
-- ngx.say(content)
local remote_filename,ip_addr,port,file_size = fbupload.fb_upload_lua(conf_filename, content, file_ext_name, file_size)
-- print(fbupload.fb_upload_lua(conf_filename, content, file_ext_name, file_size))
if remote_filename == nil then
    ngx.say('fuck..no...remote_filename...')
else
    ngx.say('<br />-------good------<br />remote_filename is:------<br />')
    ngx.say(remote_filename)
end