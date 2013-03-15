require "ok"
local conf_filename = "/etc/fdfs/client.conf"
local local_filename = "/Users/zhoujing/Pictures/cy.png"
print("\n---------\n")
print(ok.upload_lua(conf_filename,local_filename))
