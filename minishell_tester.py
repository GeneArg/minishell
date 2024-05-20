# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    minishell_tester.py                                :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eagranat <eagranat@student.42bangkok.co    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/19 23:46:49 by eagranat          #+#    #+#              #
#    Updated: 2024/05/20 17:44:38 by eagranat         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

import subprocess
import os
import shutil
import time

# Path to your minishell executable
MINISHELL_PATH = "./minishell"

# ANSI escape codes for colors
RED = '\033[91m'
GREEN = '\033[92m'
YELLOW = '\033[93m'
BLUE = '\033[94m'
CYAN = '\033[96m'
RESET = '\033[0m'

passed_tests = 0
failed_tests = 0

initial_files = set(os.listdir())
initial_dirs = set(d for d in os.listdir() if os.path.isdir(d))

def run_command(command, shell_path):
    """Run a command in the specified shell and return the output, exit status, and error output."""
    result = subprocess.run(shell_path, input=command, text=True, capture_output=True)
    output = result.stdout.strip()
    error = result.stderr.strip()
    return output, result.returncode, error

def run_minishell_command_with_status(command):
    """Run a command in Minishell, then run 'echo $?' to get the exit status."""
    minishell_process = subprocess.Popen(MINISHELL_PATH, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    minishell_process.stdin.write(command + "\n")
    minishell_process.stdin.write("echo $?\n")
    minishell_process.stdin.write("exit\n")  # Ensure minishell exits after running commands
    minishell_output, minishell_err = minishell_process.communicate()

    minishell_output_lines = minishell_output.strip().split('\n')

    # Initialize variables
    exit_status = -1
    output_lines = []

    # Iterate over the output lines to separate actual output and exit status
    for line in minishell_output_lines:
        if line.strip() == "exit":
            continue
        elif line.strip().isdigit() and exit_status == -1:
            exit_status = int(line.strip())
        else:
            output_lines.append(line.strip())

    # Join the output lines into a single string
    output = "\n".join(output_lines).strip()

    # Check if the last character of the output is '0' and treat it as an exit code
    if output.endswith('0') and exit_status == -1:
        output = output[:-1].strip()
        exit_status = 0

    return output, exit_status, minishell_err.strip()


def compare_errors(expected, actual):
    """Compare error messages, ignoring the part before the colon."""
    expected_msg = expected.split(":", 1)[-1].strip().lower()
    actual_msg = actual.split(":", 1)[-1].strip().lower()
    return expected_msg == actual_msg



def run_test(command, description, setup_needed=False, cleanup_needed=False):
    global passed_tests, failed_tests
    """Run a single test case and print the result."""
    if setup_needed:
        setup_files()  # Ensure files are created before relevant tests
        time.sleep(0.5)  # Increase delay for file operations

    minishell_output, minishell_status, minishell_err = run_minishell_command_with_status(command)
    bash_output, bash_status, bash_err = run_command(command, ["/bin/bash", "-c", command])

    # Clean up files after relevant tests
    if cleanup_needed:
        cleanup_files()  # Clean up files after relevant tests
        time.sleep(0.5)  # Increase delay for file operations

    result = "PASSED"
    if minishell_output != bash_output or minishell_status != bash_status or not compare_errors(bash_err, minishell_err):
        result = "FAILED"

    color = GREEN if result == "PASSED" else RED
    print(f"\n{BLUE}{'='*10} {description} {'='*10}{RESET}")
    print(f"{CYAN}Command: {RESET}{command.strip()}")
    print(f"{color}Result: {result}{RESET}")
    if result == "FAILED":
        print(f"{YELLOW}\nExpected Output: {RESET}{bash_output}\n{YELLOW}Minishell Output: {RESET}{minishell_output}")
        print(f"{YELLOW}\nExpected Exit Status: {RESET}{bash_status}\n{YELLOW}Minishell Exit Status: {RESET}{minishell_status}")
        if bash_err or minishell_err:
            print(f"{YELLOW}\nExpected Error Output: {RESET}{bash_err}\n{YELLOW}Minishell Error Output: {RESET}{minishell_err}")
        failed_tests += 1
    else:
        passed_tests += 1
    print(f"{BLUE}{'='*30}{RESET}")

def print_summary():
    print(f"\n{CYAN}{'='*10} Summary {'='*10}{RESET}")
    print(f"{GREEN}Passed tests: {passed_tests}{RESET}")
    print(f"{RED}Failed tests: {failed_tests}{RESET}")
    print(f"{CYAN}{'='*30}{RESET}")

def setup_files():
    # Ensure directories exist
    if not os.path.exists('outfiles'):
        os.makedirs('outfiles')

    # Files to create if they don't exist
    files_content = {
        "infile": "This is the content of infile",
        "outfile": "",
        "testfile": "This is the content of testfile",
        "emptyfile": "",
        "file with spaces": "This file has a name with spaces.",
    }

    for filename, content in files_content.items():
        if not os.path.exists(filename):
            with open(filename, 'w') as f:
                f.write(content)

    # Create bigfile with 1MB of data if it doesn't exist
    if not os.path.exists("bigfile"):
        create_bigfile()

    if not os.path.exists("testdir"):
        os.makedirs("testdir")

def create_bigfile():
    """Create a file called 'bigfile' and populate it with 1MB of data."""
    with open("bigfile", "w") as f:
        f.write("A" * 1024 * 1024)  # Write 1MB of 'A's to the file

def cleanup_files():
    # print(f"{CYAN}Cleaning up test files...{RESET}")  # Comment out or delete this line
    current_files = set(os.listdir())
    current_dirs = set(d for d in os.listdir() if os.path.isdir(d))

    new_files = current_files - initial_files
    new_dirs = current_dirs - initial_dirs

    for file in new_files:
        try:
            os.remove(file)
        except IsADirectoryError:
            shutil.rmtree(file)
        except PermissionError:
            os.chmod(file, 0o644)
            os.remove(file)

    for dir in new_dirs:
        shutil.rmtree(dir, ignore_errors=True)

    # print(f"{GREEN}Test files cleanup completed.{RESET}")  # Comment out or delete this line

def main():
    tests = [
        # Adding tests from the provided files
        {"command": "env | sort | grep -v SHLVL | grep -v ^_", "description": "Pipes: env | sort | grep -v SHLVL | grep -v ^_"},
        {"command": 'cat minishell.h | grep ");"$', "description": 'Pipes: cat minishell.h | grep ");"$'},
        {"command": "export GHOST=123 | env | grep GHOST", "description": "Pipes: export GHOST=123 | env | grep GHOST"},
        {"command": "unset HELLO=", "description": "OS Specific: unset HELLO="},
        {"command": "unset  A-", "description": "OS Specific: unset  A-"},
        {"command": "export HELLO=123 A- WORLD=456", "description": "OS Specific: export HELLO=123 A- WORLD=456"},
        {"command": "unset  HELLO A- WORLD", "description": "OS Specific: unset  HELLO A- WORLD"},
        {"command": "export UNO=1 DOS-2 TRES=3 || env | grep TRES", "description": "OS Specific: export UNO=1 DOS-2 TRES=3 || env | grep TRES"},
        {"command": 'export | sort | grep -v SHLVL | grep -v "declare -x _" | grep -v "PS.="', "description": 'OS Specific: export | sort | grep -v SHLVL | grep -v "declare -x _" | grep -v "PS.="'},
        {"command": "grep hi <./infile", "description": "Redirects: grep hi <./infile"},
        {"command": 'grep hi "<infile" <         ./infile', "description": 'Redirects: grep hi "<infile" <         ./infile'},
        {"command": "echo hi < ./infile bye bye", "description": "Redirects: echo hi < ./infile bye bye"},
        {"command": "grep hi <./bigfile <./infile", "description": "Redirects: grep hi <./bigfile <./infile"},
        {"command": 'echo <"./infile" "bonjour       42"', "description": 'Redirects: echo <"./infile" "bonjour       42"'},
        {"command": 'cat <"./file with spaces"', "description": 'Redirects: cat <"./file with spaces"'},
        {"command": 'cat <"1""2""3""4""5"', "description": 'Redirects: cat <"1""2""3""4""5"'},
        {"command": 'echo <"./infile" <missing <"./infile"', "description": 'Redirects: echo <"./infile" <missing <"./infile"'},
        {"command": 'echo <missing <"./infile" <missing', "description": 'Redirects: echo <missing <"./infile" <missing'},
        {"command": 'cat <"./infile"', "description": 'Redirects: cat <"./infile"'},
        {"command": 'echo <"./bigfile" | cat <"./infile"', "description": 'Redirects: echo <"./bigfile" | cat <"./infile"'},
        {"command": 'echo <"./bigfile" | cat "./infile"', "description": 'Redirects: echo <"./bigfile" | cat "./infile"'},
        {"command": 'echo <"./bigfile" | echo <"./infile"', "description": 'Redirects: echo <"./bigfile" | echo <"./infile"'},
        {"command": 'echo hi | cat <"./infile"', "description": 'Redirects: echo hi | cat <"./infile"'},
        {"command": 'echo hi | cat "./infile"', "description": 'Redirects: echo hi | cat "./infile"'},
        {"command": 'cat <"./infile" | echo hi', "description": 'Redirects: cat <"./infile" | echo hi'},
        {"command": 'cat <"./infile" | grep hello', "description": 'Redirects: cat <"./infile" | grep hello'},
        # {"command": 'cat <"./bigfile" | echo hi', "description": 'Redirects: cat <"./bigfile" | echo hi'},
        {"command": 'cat <missing', "description": 'Redirects: cat <missing'},
        {"command": 'cat <missing | cat', "description": 'Redirects: cat <missing | cat'},
        {"command": 'cat <missing | echo oi', "description": 'Redirects: cat <missing | echo oi'},
        {"command": 'cat <missing | cat <"./infile"', "description": 'Redirects: cat <missing | cat <"./infile"'},
        {"command": 'echo <123 <456 hi | echo 42', "description": 'Redirects: echo <123 <456 hi | echo 42'},
        {"command": 'ls >./outfiles/outfile01', "description": 'Redirects: ls >./outfiles/outfile01'},
        {"command": 'ls >         ./outfiles/outfile01', "description": 'Redirects: ls >         ./outfiles/outfile01'},
        {"command": 'echo hi >         ./outfiles/outfile01 bye', "description": 'Redirects: echo hi >         ./outfiles/outfile01 bye'},
        {"command": 'ls >./outfiles/outfile01 >./outfiles/outfile02', "description": 'Redirects: ls >./outfiles/outfile01 >./outfiles/outfile02'},
        {"command": 'ls >"./outfiles/outfile with spaces"', "description": 'Redirects: ls >"./outfiles/outfile with spaces"'},
        {"command": 'ls >"./outfiles/outfile""1""2""3""4""5"', "description": 'Redirects: ls >"./outfiles/outfile""1""2""3""4""5"'},
        {"command": 'ls >"./outfiles/outfile01" >"./outfiles/outfile02"', "description": 'Redirects: ls >"./outfiles/outfile01" >"./outfiles/outfile02"'},
        {"command": 'cat <"./infile" >"./outfiles/outfile01"', "description": 'Redirects: cat <"./infile" >"./outfiles/outfile01"'},
        {"command": 'cat <minishell.h >./outfiles/outfile', "description": 'Redirects: cat <minishell.h >./outfiles/outfile'},
        {"command": 'cat <minishell.h | ls', "description": 'Redirects: cat <minishell.h | ls'},
        {"command": '|', "description": 'Syntax: |'},
        {"command": '| echo oi', "description": 'Syntax: | echo oi'},
        {"command": '| |', "description": 'Syntax: | |'},
        {"command": '| $', "description": 'Syntax: | $'},
        {"command": '| >', "description": 'Syntax: | >'},
        {"command": '>', "description": 'Syntax: >'},
        {"command": '>>', "description": 'Syntax: >>'},
        {"command": '>>>', "description": 'Syntax: >>>'},
        {"command": '<', "description": 'Syntax: <'},
        {"command": '<<', "description": 'Syntax: <<'},
        {"command": 'echo hi <', "description": 'Syntax: echo hi <'},
        {"command": 'cat    <| ls', "description": 'Syntax: cat    <| ls'},
        {"command": 'echo hi | >', "description": 'Syntax: echo hi | >'},
        {"command": 'echo hi | > >>', "description": 'Syntax: echo hi | > >>'},
        {"command": 'echo hi | < |', "description": 'Syntax: echo hi | < |'},
        {"command": 'echo hi |   |', "description": 'Syntax: echo hi |   |'},
        {"command": 'echo hi |  "|"', "description": 'Syntax: echo hi |  "|"'},
        {"command": 'echo hello world', "description": 'Builtins: echo hello world'},
        {"command": 'echo "hello world"', "description": 'Builtins: echo "hello world"'},
        {"command": "echo 'hello world'", "description": "Builtins: echo 'hello world'"},
        {"command": "echo hello'world'", "description": "Builtins: echo hello'world'"},
        {"command": 'echo hello""world', "description": 'Builtins: echo hello""world'},
        {"command": "echo ''", "description": "Builtins: echo ''"},
        {"command": 'echo "$PWD"', "description": 'Builtins: echo "$PWD"'},
        {"command": "'$PWD'", "description": "Builtins: echo '$PWD'"},
        {"command": 'echo "aspas ->\'"', "description": 'Builtins: echo "aspas ->\'"'},
        {"command": 'echo "aspas -> \' "', "description": 'Builtins: echo "aspas -> \' "'},
        {"command": 'echo \'aspas ->"\'', "description": 'Builtins: echo \'aspas ->"\''},
        {"command": 'echo \'aspas -> " \'', "description": 'Builtins: echo \'aspas -> " \''},
        {"command": 'echo "> >> < * ? [ ] | ; [ ] || && ( ) & # $ \\ <<"', "description": 'Builtins: echo "> >> < * ? [ ] | ; [ ] || && ( ) & # $ \\ <<"'},
        {"command": 'echo \'> >> < * ? [ ] | ; [ ] || && ( ) & # $ \\ <<\'', "description": 'Builtins: echo \'> >> < * ? [ ] | ; [ ] || && ( ) & # $ \\ <<\''},
        {"command": 'echo "exit_code ->$? user ->$USER home -> $HOME"', "description": 'Builtins: echo "exit_code ->$? user ->$USER home -> $HOME"'},
        {"command": "echo 'exit_code ->$? user ->$USER home -> $HOME'", "description": "Builtins: echo 'exit_code ->$? user ->$USER home -> $HOME'"},
        {"command": 'echo "$"', "description": 'Builtins: echo "$"'},
        {"command": "'$'", "description": "Builtins: echo '$'"},
        {"command": "$", "description": "Builtins: echo $"},
        {"command": "$?", "description": "Builtins: echo $?"},
        {"command": "$?HELLO", "description": "Builtins: echo $?HELLO"},
        {"command": "pwd", "description": "Builtins: pwd"},
        {"command": "pwd oi", "description": "Builtins: pwd oi"},
        {"command": "export hello", "description": "Builtins: export hello"},
        {"command": "export HELLO=123", "description": "Builtins: export HELLO=123"},
        {"command": "export A-", "description": "Builtins: export A-"},
        {"command": "export HELLO=123 A", "description": "Builtins: export HELLO=123 A"},
        {"command": 'export HELLO="123 A-"', "description": 'Builtins: export HELLO="123 A-"'},
        {"command": "export hello world", "description": "Builtins: export hello world"},
        {"command": "export HELLO-=123", "description": "Builtins: export HELLO-=123"},
        {"command": "export =", "description": "Builtins: export ="},
        {"command": "export 123", "description": "Builtins: export 123"},
        {"command": "unset", "description": "Builtins: unset"},
        {"command": "unset HELLO", "description": "Builtins: unset HELLO"},
        {"command": "unset HELLO1 HELLO2", "description": "Builtins: unset HELLO1 HELLO2"},
        {"command": "unset HOME", "description": "Builtins: unset HOME"},
        {"command": "unset PATH", "description": "Builtins: unset PATH"},
        {"command": "unset SHELL", "description": "Builtins: unset SHELL"},
        {"command": "cd $PWD", "description": "Builtins: cd $PWD"},
        {"command": "cd $PWD hi", "description": "Builtins: cd $PWD hi"},
        {"command": "cd 123123", "description": "Builtins: cd 123123"},
        {"command": "exit 123", "description": "Builtins: exit 123"},
        {"command": "exit 298", "description": "Builtins: exit 298"},
        {"command": "exit +100", "description": "Builtins: exit +100"},
        {"command": 'exit "+100"', "description": 'Builtins: exit "+100"'},
        {"command": 'exit +"100"', "description": 'Builtins: exit +"100"'},
        {"command": "exit -100", "description": "Builtins: exit -100"},
        {"command": 'exit "-100"', "description": 'Builtins: exit "-100"'},
        {"command": 'exit -"100"', "description": 'Builtins: exit -"100"'},
        {"command": "exit hello", "description": "Builtins: exit hello"},
        {"command": "exit 42 world", "description": "Builtins: exit 42 world"},
        {"command": "$PWD", "description": "Extras: $PWD"},
        {"command": "$EMPTY", "description": "Extras: $EMPTY"},
        {"command": "$EMPTY echo hi", "description": "Extras: $EMPTY echo hi"},
        {"command": "./missing.out", "description": "Extras: ./missing.out"},
        {"command": "missing.out", "description": "Extras: missing.out"},
        {"command": '"aaa"', "description": 'Extras: "aaa"'},
        {"command": "minishell.h", "description": "Extras: minishell.h"},
        {"command": "$", "description": "Extras: $"},
        {"command": "$?", "description": "Extras: $?"},
        {"command": "README.md", "description": "Extras: README.md"},
    ]

    for test in tests:
        setup_needed = "infile" in test["command"] or "outfile" in test["command"] or "testfile" in test["command"] or "emptyfile" in test["command"] or "file with spaces" in test["command"] or "bigfile" in test["command"]
        cleanup_needed = setup_needed
        run_test(test["command"], test["description"], setup_needed, cleanup_needed)

    # Print summary
    print_summary()

if __name__ == "__main__":
    main()
