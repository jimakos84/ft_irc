# ft_irc

A collaborative implementation of an IRC (Internet Relay Chat) server written in C++ as part of the Hive Helsinki curriculum.

## Overview

The goal of this project was to implement the core IRC protocol and build a server capable of handling multiple simultaneous clients communicating over TCP. Along the way we learned modern C++, networking, protocol design, debugging and collaborative development.

## Features

- Multiple simultaneous client connections
- User registration and authentication
- Channel creation and management
- Private and channel messaging
- Invitations and channel modes
- Robust command parsing and protocol handling

## Tech Stack

- C++
- TCP/IP sockets
- Event-driven I/O
- Git for collaborative development

## My Contribution

I worked primarily on implementing and improving core server functionality, debugging protocol behaviour, command handling and integration with teammates' work. The project also gave me practical experience collaborating through Git, pull requests and code reviews.

## Challenges

One of the biggest challenges was keeping client state consistent while handling multiple users simultaneously and ensuring behaviour matched the IRC specification. Debugging protocol edge cases and integrating changes from multiple contributors were valuable learning experiences.

## What I Learned

- Object-oriented design in C++
- TCP/IP networking fundamentals
- Building software collaboratively with Git
- Debugging complex systems
- Reading and implementing a network protocol specification

## Build

```bash
make
./ircserv <port> <password>
```
