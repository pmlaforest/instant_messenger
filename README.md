# instant_messenger

This is my implementation of an instant_messenger software. 

This sotfware is made of three programs : 

- client
- srv
- conv_window

Every user connects to the server software (srv) with a local copy of a client software (client). Once connected, they can create an account
or just sign-in. The goal of all this is to chat with friends, so users can add friends too.  Conv_window is just the way I found to separate the components of this project. When a user initiates a conversation
with another user (provided that they both are friends), a conv_window process is created. Messages are passed from client to 
server with a socket and from client to conv_window with a pipe. The server's sotfware uses a sqlite database to store 
information.



