'''
c编写的lua接口，依赖libswoole.so  libswoole_cpp.so
/usr/local/lib/lua/5.1
/usr/lib64/libnghttp2.so.14.14.0 /usr/lib64/libnghttp2.so.14

/usr/local/lib/libswoole_cpp.so -> libswoole_cpp.so.1.9.0
/usr/lib/libswoole.so -> /usr/lib/libswoole.so.1.9.5

[root@File 5.1]# ln -s libswoole.so.1.9.5 libswoole.so
[root@File 5.1]# ln -s libswoole_cpp.so.1.9.0 libswoole_cpp.so

'''

local swoole=require "luaswoole" --luaswoole.so
local client = swoole.new()
local err = client:connect("127.0.0.1",1234)
if err then
	print(err)
	return
end

local data = client:sendRecv("3213213123213") 
if data then
	
end

client:close()

