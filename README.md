
# CSHELL - Custom Shell Implementation 🐚

A feature-rich Unix shell implementation built in C as part of the Operating Systems and Networks course. This shell provides essential command-line functionalities with custom built-in commands and advanced features like piping, redirection, and background process management.


## ✨ Features

### 🔧 Core Shell Functionality
- **Interactive Command Line**: Custom shell prompt with user and system information
- **Command Parsing**: Robust tokenization and argument parsing
- **Process Management**: Execute system commands with proper process handling
- **Background Processes**: Support for running commands in background using `&`
- **Signal Handling**: Proper handling of CTRL+C, CTRL+Z signals

### 🚀 Advanced Features
- **Piping**: Chain multiple commands using `|` operator
- **I/O Redirection**: 
  - Input redirection (`<`)
  - Output redirection (`>`, `>>`)
  - Error redirection (`2>`)
- **Command History**: Persistent command history with search functionality
- **Job Control**: Foreground and background job management

### 📏 General Assumptions
- **Maximum Input Length**: 4096 characters
- **Exit Command**: Entering `exit` as command will close the terminal
- **Error Handling**: All errors are printed to stdout

## 🛠️ Built-in Commands

### `hop [directory]` - Directory Navigation (Spec 3)
Enhanced directory navigation command with support for multiple directories and previous directory tracking.

#### Usage
```bash
hop                    # Go to home directory
hop ..                 # Go to parent directory
hop /path/to/dir      # Go to specified directory
hop -                 # Go to previous directory
hop dir1 dir2         # Navigate to multiple directories
```

#### 🔍 Assumptions
- If `hop dir1 dir2` is given and `dir2` is invalid, directory will first change to `dir1` and then print error message for `dir2`
- If only `hop` is given, it will directly go to home directory (where shell is invoked)
- If `hop dir1 -` is given, it will return to the same directory it started
- Only `hop -` will work. Not handling `hop -/dir` cases

### `reveal [flags] [path]` - Directory Listing (Spec 4)
Display directory contents with various formatting options and color-coded output.

#### Usage
```bash
reveal                 # List current directory
reveal -l              # Long format listing
reveal -a              # Show hidden files
reveal -la /path       # Long format with hidden files
reveal -              # Go to previous directory and list
```

#### 🔍 Assumptions
- If `reveal -` is given as first prompt (i.e., `prev_dir` is not set), it will print warning "OLD_PWD not set"
- Maximum number of entries in a directory is assumed to be 1024
- If `reveal -l` is given, color change is reflected only on file/directory names

### `log [subcommand]` - Command History (Spec 5)
Command history management with persistent storage and execution capabilities.

#### Usage
```bash
log                    # Show command history (last 15 commands)
log purge             # Clear command history
log execute <index>   # Execute command from history
```

#### 🔍 Assumptions
- Erroneous commands will also get stored in log
- Always an absolute path in the home directory is given to store commands in `history.txt` file
- Any input having "log" as substring in it won't be stored even if it is a file name

### `proclore [pid]` - Process Information (Spec 6)
Display detailed process information with execution time tracking for foreground processes.

#### Usage
```bash
proclore              # Show shell process info
proclore 1234         # Show info for PID 1234
```

#### 🔍 Assumptions
- If input has multiple foreground processes with execution time more than 2 seconds, then in the next prompt, it will display the most recent one
- For an invalid background command, it will print PID and then 'execvp() failed' and 'exited normally'
- Background won't work for custom functions. If given, it will print 'execvp() failed'
- Maximum number of background processes that can be handled is 1024

### `seek [flags] [target] [search_directory]` - File Search (Spec 10)
Advanced file and directory search with various filtering options.

#### Usage
```bash
seek file.txt         # Search for file.txt
seek -d dirname       # Search for directory
seek -f filename      # Search for file only
seek -e pattern       # Execute if single match found
```

#### 🔍 Assumptions
- All errors will be printed to stdout
- Assuming filename only at the end of input

### `activities` - Background Process Management (Spec 13)
Display all currently running background processes with their status.

#### Usage
```bash
activities            # Show background processes
```

#### 🔍 Assumptions
- If an error command is executed, it will still be stored in activities

### `ping <pid> <signal_number>` - Signal Management
Send signals to processes for job control.

#### Usage
```bash
ping 1234 9          # Send SIGKILL to PID 1234
ping 1234 15         # Send SIGTERM to PID 1234
```

### `fg <pid>` / `bg <pid>` - Job Control (Spec 15)
Bring background processes to foreground or vice versa.

#### Usage
```bash
fg 1234              # Bring PID 1234 to foreground
bg 1234              # Send PID 1234 to background
```

#### 🔍 Assumptions
- If `sleep 5` is in foreground, then after 5 seconds only Ctrl+D will be accepted and then terminal closes
- After converting background to foreground, if it takes more than 2 seconds, prompt will change

### `neonate [flags]` - Process Information (Spec 16)
Display information about newborn processes.

#### Usage
```bash
neonate -n           # Show process information
```

#### 🔍 Assumptions
- If neonate with time interval is given, it will print "invalid command"

### `iMan <command>` - Manual Pages
Fetch and display manual pages for commands.

#### Usage
```bash
iMan ls              # Show manual for ls command
iMan grep            # Show manual for grep command
```

## 🔧 Advanced Features

### Piping (Spec 11)
Chain multiple commands using the `|` operator for inter-process communication.

#### Usage
```bash
ls -la | grep "txt"
cat file.txt | sort | uniq
```

#### 🔍 Assumptions
- Assuming `hop .. | wc` won't change the directory but will count words from the output of `hop ..`

### I/O Redirection (Spec 12)
Redirect input and output using `<`, `>`, and `>>` operators.

#### Usage
```bash
echo "Hello" > file.txt
cat < input.txt > output.txt
command >> append.txt
```

#### 🔍 Assumptions
- If `&` is given, it will do background only for the preceding command

### Background Processes
Execute commands in the background using the `&` operator.

#### Usage
```bash
sleep 100 &
long_running_command &
```

### Signal Handling (Spec 14)
Proper handling of CTRL+C, CTRL+Z, and CTRL+D signals.

#### 🔍 Assumptions
- If Ctrl+C is given when no foreground processes are running, it will remain on the same line and still accept input (not new prompt)
- If Ctrl+D is given, then it will print all the PIDs and print them as killed

##  Installation

### Prerequisites
- GCC compiler
- Linux/Unix environment
- Make utility

### Build Instructions
```bash
# Clone the repository
git clone https://github.com/yourusername/cshell.git
cd cshell

# Compile the shell
make

# Run the shell
./cshell
```

### Manual Compilation
```bash
gcc -o cshell main.c execute.c hop.c reveal.c log.c proclore.c seek.c activities.c signals.c fg_bg.c neonate.c iMan.c pipes.c redirection.c token.c queue.c display.c -lm
```


## 📁 Project Structure

```
cshell/
├── main.c              # Main shell loop and initialization
├── execute.c           # Command execution logic
├── execute.h           # Execution function declarations
├── hop.c               # Directory navigation implementation
├── hop.h               # Hop function declarations
├── reveal.c            # Directory listing implementation
├── reveal.h            # Reveal function declarations
├── proclore.c          # Process information display
├── proclore.h          # Proclore function declarations
├── seek.c              # File search implementation
├── seek.h              # Seek function declarations
├── activities.c        # Background process management
├── activities.h        # Activities function declarations
├── signals.c           # Signal handling implementation
├── signals.h           # Signal handling declarations
├── fg_bg.c             # Foreground/background job control
├── fg_bg.h             # Job control declarations
├── neonate.c           # Newborn process information
├── neonate.h           # Neonate function declarations
├── iMan.c              # Manual page fetcher
├── iMan.h              # iMan function declarations
├── pipes.c             # Pipe implementation
├── pipes.h             # Pipe function declarations
├── redirection.c       # I/O redirection implementation
├── redirection.h       # Redirection function declarations
├── token.c             # Command tokenization
├── token.h             # Tokenization declarations
├── queue.c             # Queue data structure for processes
├── queue.h             # Queue declarations
├── display.c           # Display and formatting utilities
├── display.h           # Display function declarations
├── history.txt         # Command history storage
├── .myshrc             # Shell configuration file
├── makefile            # Build configuration
└── README.md           # This file
```
##  Examples

### Complex Command Combinations
```bash
# Multiple pipes with redirection
ls -la | grep "txt" | sort > sorted_files.txt

# Background process with redirection
sleep 100 > /dev/null 2>&1 &

# Search and execute
seek -e "main.c" ./src

# Process management
ping 1234 15
fg 1234
```


## 🐛 Known Issues and Limitations

### Current Limitations
* Signal handling might need refinement for some edge cases
* Tab completion is not implemented
* Some built-in commands may need additional error checking
* Background processes don't work for custom functions

## Edge Cases
* Complex directory navigation patterns may not be fully supported
* Error handling for invalid process IDs needs improvement
* Memory management in case of very long command chains
