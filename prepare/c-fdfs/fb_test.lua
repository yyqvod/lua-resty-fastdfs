--[[
lua 调用fastDFS动态链接库进行图片上传demo
]]--

require "fbupload"
local conf_filename = "/etc/fdfs/client.conf"
-- local local_filename = "/Users/zhoujing/Pictures/ttt.md"
local local_filename = "/Users/zhoujing/Pictures/cy.jpg"

-- local w_filename = "./a.jpg"

local function getextension(filename)
	return filename:match(".+%.(%w+)$")
end


local file = io.open(local_filename, "r")
-- local w_file = io.open(w_filename, "w")

local file_content = file:read("*all")

local file_size = file:seek("end")
local file_ext_name = getextension(local_filename)

assert(file:close())

-- print(type(file_content),file_content)
-- w_file:write(file_content)
-- print("\n---------\n",file_size,"\n---------\n",file_ext_name, "\n---------\n",file_content)
-- print(fbupload.fb_upload_lua(conf_filename, file_content, file_ext_name, file_size))
remote_filename,ip_addr,port,file_size = fbupload.fb_upload_lua(conf_filename, file_content, file_ext_name, file_size)
print(remote_filename,"------ok------",ip_addr,"---------------",port,"--------ggg-------",file_size)


-- require "fbupload"
-- local conf_filename = "/etc/fdfs/client.conf"
-- local local_filename = "/Users/zhoujing/Pictures/ttt.md"

-- local function getextension(filename)
-- 	return filename:match(".+%.(%w+)$")
-- end


-- local file = io.open(local_filename, "r")

-- local file_content = file:read("*all")

-- local file_size = file:seek("end")
-- local file_ext_name = getextension(local_filename)

-- assert(file:close())

-- print(fbupload.fb_upload_lua(conf_filename, file_content, file_ext_name, file_size))