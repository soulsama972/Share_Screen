# ShareScreen

## Description
Simple share screen software to share the main screen. at full screen size.
currently work for windows only.


## dependencies:
opencv any verion above 3.0.0

## Build on Window:
* you will need g++ and make install on your machine.

build the server and client:
run the build.bat

## run server:
`-ip`(Optional). A public ip address which is being used allow specific remote connection. (default anyone can access).

`-port`(Optional). which port to listen to. (default 8888).

## run client:
`-ip` Remote machine ip (default 127.0.0.1 - local machine).

`-port` Same as the remote machine port (default 8888).

## exmples:
```server.exe -ip YOUR_BIND_IP -port  YOUR_LISTEN_PORT```

```client.exe -ip YOUR_SERVER_IP -port SAME_AS_SERVER_LISTEN_PORT```

## todo list:
- [] linux support / mac.
- [] let the client control the remote screen with privilege.
- [] maintain 60fps (currently about 30fps).
- [] optimize CPU usage.
- [] h.264 support for better network and lentncy.
- [] multi user share using UDP

