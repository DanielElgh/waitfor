# Waitfor [![Build Status](https://travis-ci.com/DanielElgh/waitfor.svg?branch=master)](https://travis-ci.com/DanielElgh/waitfor)

**Waitfor** is a basic and unpretentious utility to detect if and when a text string occurs on an input stream.

It is useful for Site Reliability Engineers, DevOps Engineers, and sysadmins when automating tasks with shell scripts, Ansible, or similar tools, as it can delay further execution until a certain string occurs.

Waitfor is compatible with Windows, Linux, and BSD.

The program searches for a text string in an input stream. The input stream can either be a file or standard input (stdin). Waitfor terminates with exit code zero if a match occurs. A non-zero exit code indicates an error, or that the optional timeout was set and reached before a match was found. 

> **Note:** A more in-depth description of exit codes can be found in the [exit codes section](#Understanding-the-Exit-Codes). 


## Using waitfor

The first argument defines a search string. This is the string which the program is searching for in the input stream.

The second argument defines where the input stream should come from. This is usually a file, but it can also be standard input. Set this argument to two consecutive hyphens (--) if you wish to read from standard input instead of a file.

The third argument is optional and defines a timeout. The program will exit when it has been running for longer than the specified timeout and reaches end-of-file (EOF). If no third argument exists, waitfor will continue to scan the input stream until a match occur, which may be indefinitely.

The arguments `--help` and `--version` are also valid as first arguments if used independently. They show a short help section and the program version number, respectively. 

### Examples

1. Wait for the text string “Find me” to appear in data.txt, and for an indefinite amount of time if necessary.

```bash
waitfor 'Find me' data.txt
```

2. Wait for the text string “Save complete” to appear in server.log but wait no longer than 60 seconds.

```bash
waitfor 'Save complete' latest.log 60
```

3. Check if “Rhubarb” appears on standard input and give up immediately when reaching “end-of-file” (EOF). In this case waitfor will terminate with exit code 1.

```bash
echo -e "Flour\nSugar\nStrawberries\nButter\nWhipping cream" | waitfor 'Rhubarb' -- 0
```

4. Ansible job to start an application and wait for it to become ready. Give up and fail the job after 10 minutes unless a match has occurred.

```yaml
- hosts: 'my_app_servers'
  tasks:
    - name: 'Start My Application'
      shell: '/opt/my_app/bin/standalone.sh &'
    - name: 'Delay further execution until My Application is ready'
      command: "waitfor '\"Kenny\" started in' /opt/my_app/standalone/log/server.log 600"
    - name: 'Do something now after Kenny has started on all hosts'
      [...]
```

## Understanding the Exit Codes

The header-file _waitfor.h_ defines all exit codes. The below table gives a brief description of what each exit code means.

| Exit code | Description |
| --------- | ----------- |
| 0         | Found a match. |
| 1         | Timeout reached before a match occurred. |
| 10        | Missing mandatory argument(s). |
| 11        | The timeout argument contains nonnumeric characters. |
| 20        | Unable to allocate memory for the input buffer. |
| 21        | Failed to open input file or set input stream to stdin. |
| 22        | An error occurred while reading the input stream. |
| 255       | Undefined error |

> **Note:** Starting the program with the arguments `--help` or `--version` also leads to termination with exit code zero (Match found). 

## Building waitfor

Waitfor is a C program and conforms to the c99 standard. The configure script and test suite is written in bash. 

### Building with GCC and GNU Make

Building and installing the `waitfor` executable is simple on a system with _GCC_ and _GNU Make_. Running the below commands will build and install the binary executable to the default installation path, which is /usr/local/bin/. Append the `./configure` command with the argument `--prefix=/path/to/install/` if you wish to override the default location.

```bash
./configure
make
make install
```

If you do not wish to install, i.e. only build the binary, run the above sequence of commands but skip the `make install` command.

> **Note:** A nonprivileged user might not be able to run the `make install` command. If it fails, try running the command again with `sudo` or as the root user.

### Building with GCC but without GNU Make

Compiling and linking the source code is a quite simple task even if GNU Make is absent. This is an example working on most Linux distributions as well as on BSD systems.
To compile and link, run:

```bash
gcc --std=c99 --Wall --Wextra --pedantic -O2 -o waitfor *.c
```

Installation can if desired be achieved by copying the binary executable.

```bash
cp waitfor /usr/local/bin/
```

### Building on Windows with Visual Studio

This project includes no Visual Studio project files. However, it is still simple to compile and link waitfor on a Windows PC with Visual Studio. This is an example on how to use Visual Studio’s C compiler to build waitfor on Windows:

```batch
cd <project directory>
cl /nologo /W4 /O2 *.c /link /out:waitfor.exe
```
> **Note:** Setting up a command-line development environment from scratch can be troublesome. Microsoft makes it easier with “Developer Command Prompt for VS” which is included with Visual Studio. More information can be found on [Microsoft Docs](https://docs.microsoft.com/en-us/cpp/build/walkthrough-compile-a-c-program-on-the-command-line "Microsoft Docs").

## Contributing

You are always welcome to contribute if you discover bugs or find areas to improve. Feel free to open issues if you do not know how to solve your discoveries or ideas with code. 
If you contribute with code, please add test cases in the _test_ file and make sure that the test cases cover as much as possible of your newly added code. 

### Run test cases

```bash
./configure
make test
```

### Run coverage tests with gcov

```bash
./configure --gcov
make clean test gcov
less <file changed>.c.gcov
```

## License

This project is licensed under the terms of the MIT license.
