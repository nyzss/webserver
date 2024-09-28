/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bthomas <bthomas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:21:12 by bthomas           #+#    #+#             */
/*   Updated: 2024/09/28 16:06:59 by bthomas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef EVENTHANDLER_HPP 
# define EVENTHANDLER_HPP 

#include "Server.hpp"
#include "ClientConnection.hpp"
#include <fcntl.h>
#include <map>
#include <utility>
#include <cerrno> //temp, just for testing
#include <cstdlib>
#include <sys/wait.h>

#define BUFFER_SIZE 30720

class ClientConnection;

enum responseType {
	TEXT,
	NONTEXT
};

struct responseContent {
	responseType type;
	void * content;
};

class EventHandler
{
	private:
		int _epollFd;
		std::map<int, ClientConnection*> _clients;
		char** _av;
		char** _env;

	public:
		class epollInitFailure;
		void setNonBlock(int fd);
		bool addToEpoll(int fd);
		bool deleteFromEpoll(int fd);
		void addClient(int clientFd);
		void handleNewConnection(Server & s);
		void handleClientRequest(int clientFd);
		void handleResponse(int clientFd);
		void epollLoop(Server & s);
		void changeToWrite(int clientFd);
		void changeToRead(int clientFd);
		bool isResponseComplete(int clientFd);
		void cgiOut(int clientFd, std::string fname = "cgi_bin/tester.cgi");
		void setPipe(int *fd, int end);

	public:
		class epollWaitFailure;

	public:
		EventHandler (char **av, char **env);
		~EventHandler ();
};

#endif /* EVENTHANDLER_HPP */
