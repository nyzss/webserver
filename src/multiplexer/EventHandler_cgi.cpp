/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler_cgi.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 12:35:44 by bthomas           #+#    #+#             */
/*   Updated: 2024/10/03 21:05:52 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventHandler.hpp"
#include "string.h"
#include <csignal>
#include <cstdlib>
#include <unistd.h>
#include <vector>

static void setPipe(int *fd, int end) {
	if (dup2(fd[end], end) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1) {
		std::cerr << "Error: could not open pipe for cgi output\n";
		throw (1);
	}
}

void	alarm_handler(int _sig) { (void)_sig; throw (1); }

// forks process - child execve's script
// return true if success -> false if failed
bool EventHandler::startCGI(int clientFd, std::vector<std::string> arguments) {
	pid_t pid;
	int fd[2];

	if (pipe(fd) == -1) {
		std::cerr << "Error: could not establish pipe for CGI output\n";
		return false;
	}
	pid = fork();
	if (pid == -1) {
		std::cerr << "Error: could not fork process in CGI output\n";
		close(fd[0]);
		close(fd[1]);
		return false;
	}
	if (pid == 0) {
		setPipe(fd, STDOUT_FILENO);
		char **args = new char*[3];
		args[0] = const_cast<char *>(arguments[0].c_str());
		args[1] = const_cast<char *>(arguments[1].c_str());
		args[2] = NULL;

		signal(SIGALRM, alarm_handler);

		alarm(5);

		execve(arguments.front().c_str(), args, environ);
		delete[] args;
		std::cerr << "Error: could not execute cgi script\n";
		throw (1);
	} else {
		close(fd[1]);
		int status;
		bool code = waitpid(pid, &status, 0) < 0 || !WIFEXITED(status) || WEXITSTATUS(status) != 0;

		std::cout << "CGI EXIT STATUS: " << code << std::endl;
		std::cout << "Response sent to client.\n";

		if (!code)
		{
			addToEpoll(fd[0]);
			_cgiManager.addCgiProcess(clientFd, fd[0], pid);
			_openConns[fd[0]] = EP_CGI;
		}
		else
		{
			close(fd[0]);
			return false;
		}
	}
	return true;
}

