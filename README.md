# Simple POSIX Shell

## Overview
This project implements a simple POSIX/Unix shell in C++. 

### Features
- Execute external commands 
- Built-in support for:
  - `cd` command to change the current directory.
  - `exit` command to terminate the shell.
- Input validation:
  - Enforces a maximum input size of 1000 characters.
  - Handles mismatched quotes in input.
  - Error handling for invalid commands, failed processes, and other runtime issues.

---

## Getting Started

### Compilation
To compile the shell program, use the following command:
```
g++ -o Matic matic.cpp
```
This generates an executable named `matic`.

### Running the Shell
Run the shell by executing:
```
./bash
```
You will see a prompt (`$`) where you can enter commands.

---

## Usage

### Built-in Commands
1. **`cd`**: Changes the current working directory.
   - Usage: `cd <directory>`
   - Example:
     ```
     cd /
     ```
   - If the directory does not exist, an error message will be displayed.

2. **`exit`**: Exits the shell.
     ```
     exit
     ```

### Input Size Limit
- The shell enforces a maximum input size of **1000 characters**.
- If the input exceeds this limit, an error message is displayed:
  ```
  error: Input exceeds maximum allowed length of 1000 characters
  ```

## Implementation Details

### Key Functions
1. **`parseInput`**:
   - Splits the user's input string into arguments.,
   - Handles quoted strings (`'` and `"`) and ensures proper matching.
   - Detects and reports mismatched quotes.

2. **`executeCommand`**:
   - Forks a new process and executes the given command using `execvp`.
   - Waits for the child process to complete and handles errors gracefully.

3. **`handleCd`**:
   - Implements the `cd` command by calling `chdir`.
   - Ensures correct argument usage and reports errors if the directory is invalid.

4. **`printError`**:
   - Prints error messages to `stderr` in a consistent format.

### Error Handling
- Comprehensive checks for errors such as:
  - Fork failure.
  - Invalid or missing arguments for built-in commands.
  - Failed execution of external commands.

---

## Testing
### Manual Testing
1. **Basic Commands**:
   - Run commands like `ls`, `pwd`, and `echo`.
   - Verify the output matches expected behavior.

2. **Built-in Commands**:
   - Test `cd` with valid and invalid directories.
   - Test `exit` to ensure the shell terminates correctly.

3. **Input Validation**:
   - Enter inputs longer than 1000 characters.
   - Use mismatched quotes.

4. **Error Scenarios**:
   - Run invalid commands.
   - Test edge cases for all features.

## Future Enhancements
- Add support for piping (`|`) and redirection (`>`, `<`).
- Support background processes (`&`).
- Implement signal handling for `Ctrl+C`.
- Enhance parsing to handle complex commands.

---
