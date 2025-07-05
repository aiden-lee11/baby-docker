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
	std::cout << "hello from inner clone proc\n" << " with pid " << getpid() << "\n";
	return 0;
}


int main() { 
	std::cout << "hello from main proc with pid " << getpid() << "\n";

	char* child_stack = new char[STACK_SIZE];

	if (child_stack == nullptr) {
		std::cout << "somehow nulled malloc lol";
		return 1;
	}

	pid_t pid = clone(
		child_proc, 
		child_stack + 1024 * 1024,
		CLONE_NEWPID | SIGCHLD,
		nullptr
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
