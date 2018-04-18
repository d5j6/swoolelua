
project "luaswoole"
	language "C++"
	kind "SharedLib"
	
	includedirs{
		"/usr/include/glib-2.0/",
		"/usr/local/webserver/qconf/include/",
		"/usr/local/webserver/openssl/include/",
		"./"
	}
	

	files { 
		"SwooleClient.h",
		"SwooleClient.cpp",
		"lua/*.h",
		"lua/*.cpp",		
	}
	links{
		"swoole_cpp",
		"swoole",
		--"lua5.1"
		"luajit-5.1",
	}
	
  	configuration { "linux", "gmake" }
		buildoptions { "`pkg-config --cflags --libs glib-2.0 gthread-2.0`", "-g3 -std=c++11 -Wall -ggdb -O0", "-pedantic -pthread -rdynamic" }