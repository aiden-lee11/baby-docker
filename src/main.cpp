#include <cstddef>
#include <iostream>
#include <sched.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <cstring>

const size_t STACK_SIZE = 1024 * 1024;

int child_proc(void *args){ 
	char* filename = (char*)args;
	char* argv[] = {filename, nullptr};

	int exec_ret = execv(filename, argv);
	if (exec_ret == -1) {
		std::cerr << "execv() failed (" << errno << "): "
			<< std::strerror(errno) << '\n';
		return -1;
	}

	return 0;
}


int main(int argc, char** argv) { 
	std::cout << "hello from main proc with pid " << getpid() << "\n";

	char* child_stack = new char[STACK_SIZE];

	if (child_stack == nullptr) {
		std::cout << "somehow nulled malloc lol";
		return 1;
	}

	if (argc < 2) {
		std::cout << "need command to run";
		return -1;
	}

	pid_t pid = clone(
		child_proc, 
		child_stack + 1024 * 1024,
		CLONE_NEWPID | CLONE_NEWNS | SIGCHLD,
		argv[1]
	);

	if (pid == -1) {
		std::cerr << "clone() failed (" << errno << "): "
			<< std::strerror(errno) << '\n';
		delete[] child_stack;
		return 1;
	}

	if (waitpid(pid, nullptr, 0) == -1) {
		std::cerr << "error with waitpid\n";
		delete[] child_stack;
		return 1;
	}

	delete[] child_stack;
	return 0;
}
