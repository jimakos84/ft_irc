===ft_irc===

A minimal IRC (Internet Relay Chat) server implemented in C++ as part of the 42 curriculum.
The project focuses on TCP socket programming, client-server architecture, and protocol handling using low-level Unix system calls.

===Features===

TCP socket communication

Multi-client handling using poll()

IRC command parsing and execution

Channel creation and management

Message broadcasting between clients

Graceful connection and disconnection handling

===Architecture Overview===

The server uses an event-driven model built around poll() to monitor multiple client sockets simultaneously.

Core flow:

Create and bind server socket

Listen for incoming connections

Accept new clients

Use poll() to monitor:

New incoming connections

Incoming client messages

Parse IRC commands and execute corresponding server logic

Broadcast messages to relevant channels or users

This structure allows the server to handle multiple clients efficiently without blocking.

The implementation focuses on:

Efficient socket handling

Proper memory management

Robust parsing of client input

Handling edge cases and invalid commands

===Installation & Usage===

Compile:

make

Run:

./ircserv <port> <password>

Connect using an IRC client (e.g., irssi or another IRC client application).

===Technical Highlights===

Implemented low-level socket communication using socket, bind, listen, accept

Managed multiple client states and message routing

Designed structured command parsing system

Focused on avoiding memory leaks and undefined behavior

===What I Learned===

Practical TCP/IP networking fundamentals

Designing a structured server architecture

Handling concurrent client interactions

Debugging complex runtime issues

Writing stable C++ code under system-level constraints
