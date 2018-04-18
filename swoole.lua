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