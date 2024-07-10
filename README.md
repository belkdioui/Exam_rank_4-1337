# Exam Rank 4 Solutions - 1337

## Table of Contents

- [Overview](#overview)
- [Usage](#usage)
  - [Compilation](#compilation)
  - [Execution](#execution)
  - [Example Output](#example-output)
- [Features](#features)
- [Implementation Details](#implementation-details)
  - [Allowed Functions](#allowed-functions)
  - [Error Handling](#error-handling)
- [Limitations](#limitations)

## Overview

This repository contains the implementation of Microshell, an exam project from school 1337. Microshell is a program designed to behave like a basic shell, capable of executing shell commands provided as arguments to the program. It supports pipe (`|`) and sequential command (`;`) execution similar to bash.

## Usage

### Compilation

Compile the program using the following command:

```bash
gcc -Wall -Wextra -Werror microshell.c -o microshell
```

## Execution
Run the microshell program with shell commands as arguments. For example:

```bash
Copy code
./microshell /bin/ls "|" /usr/bin/grep microshell ";" /bin/echo i love my microshell
```
### Example Output
The output will be similar to executing the commands in sequence:

```
microshell
i love my microshell
```

## Features

- Executes shell commands provided as arguments.
- Supports pipe (`|`) for chaining commands' output.
- Supports sequential execution (`;`) of multiple commands.
- Implements built-in `cd` command with error handling:
- Outputs errors to STDERR for invalid arguments or failed directory changes.
- Handles system call errors and exits gracefully with appropriate error messages.

## Implementation Details

### Allowed Functions

The implementation uses the following allowed functions:

`malloc, free, write, close, fork, waitpid, signal, kill, exit, chdir, execve, dup, dup2, pipe, strcmp, strncmp`

### Error Handling

- Prints `"error: fatal"` followed by a newline and exits if any system call (except `execve` and `chdir`) fails.
- Prints `"error: cannot execute <executable_path>"` if `execve` fails.
- Handles file descriptor management to prevent leaks.
- Validates and handles errors for the `cd` built-in command as specified.

## Limitations

- Does not support wildcards (`*`, `~`) or environment variables (`$BLA`).
- Manages more than hundreds of pipes (`|`) gracefully, even with a limit on open files.
