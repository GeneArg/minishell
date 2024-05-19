# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    minishell_tester.py                                :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eagranat <eagranat@student.42bangkok.co    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/19 23:46:49 by eagranat          #+#    #+#              #
#    Updated: 2024/05/19 23:48:13 by eagranat         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

import subprocess
import os

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
    minishell_output, minishell_err = minishell_process.communicate()

    minishell_output_lines = minishell_output.strip().split('\n')
    if len(minishell_output_lines) > 1 and minishell_output_lines[-2].isdigit() and minishell_output_lines[-1] == 'exit':
        exit_status = int(minishell_output_lines[-2])
        output = '\n'.join(minishell_output_lines[:-2]).strip()
    else:
        exit_status = -1  # Set a default error exit status if parsing fails
        output = minishell_output.strip()

    return output, exit_status, minishell_err.strip()

def run_test(command, description):
    global passed_tests, failed_tests
    """Run a single test case and print the result."""
    minishell_output, minishell_status, minishell_err = run_minishell_command_with_status(command)
    bash_output, bash_status, bash_err = run_command(command, ["/bin/bash", "-c", command])

    result = "PASSED" if minishell_output == bash_output and minishell_status == bash_status and minishell_err == bash_err else "FAILED"
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

def run_file_redirection_test(command, input_file, expected_output_file, description):
    global passed_tests, failed_tests
    """Run a file redirection test case."""
    if not os.path.exists(input_file):
        print(f"{RED}FAILED: {description}{RESET}")
        print(f"Input file '{input_file}' does not exist.")
        failed_tests += 1
        return

    with open(input_file, 'r') as infile:
        minishell_output, minishell_status, minishell_err = run_minishell_command_with_status(command)
    with open(input_file, 'r') as infile:
        bash_output, bash_status, bash_err = run_command(command, ["/bin/bash", "-c", command])
    with open(expected_output_file, 'r') as outfile:
        expected_output = outfile.read().strip()

    result = "PASSED" if minishell_output == expected_output and bash_output == expected_output else "FAILED"
    color = GREEN if result == "PASSED" else RED
    print(f"\n{BLUE}{'='*10} {description} {'='*10}{RESET}")
    print(f"{CYAN}Command: {RESET}{command.strip()}")
    print(f"{color}Result: {result}{RESET}")
    if result == "FAILED":
        print(f"{YELLOW}\nExpected Output: {RESET}{expected_output}\n{YELLOW}Minishell Output: {RESET}{minishell_output}")
        if bash_output != expected_output:
            print(f"{YELLOW}\nBash Output: {RESET}{bash_output}")
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
    with open("infile", "w") as f:
        f.write("This is the content of infile")

    with open("outfile", "w") as f:
        f.write("")

    with open("testfile", "w") as f:
        f.write("This is the content of testfile")

def cleanup_files():
    os.remove("infile")
    os.remove("outfile")
    os.remove("testfile")

def main():
    setup_files()

    tests = [
        {"command": "echo Hello, World!\n", "description": "Basic echo command"},
        {"command": "ls\n", "description": "Listing files"},
        {"command": "cat non_existent_file\n", "description": "Cat non-existent file"},
        {"command": "echo Hello > testfile\n", "description": "Redirection to file"},
        {"command": "cat < testfile\n", "description": "Input redirection from file"},
        {"command": "echo Hello | grep H\n", "description": "Pipe echo to grep"},
        {"command": "pwd\n", "description": "Print working directory"},
        {"command": "export TEST_VAR=42\n", "description": "Set an environment variable"},
        {"command": "echo $TEST_VAR\n", "description": "Echo an environment variable"},
        {"command": "unset TEST_VAR\n", "description": "Unset a variable"},
        # Add more complex commands without &&, ||, or wildcards
        {"command": "echo \"This is a test\" | tr ' ' '_'\n", "description": "Pipe with tr"},
        {"command": "echo \"Testing\" | rev\n", "description": "Pipe with rev"},
        {"command": "echo \"Line1\nLine2\nLine3\" | grep Line2\n", "description": "Pipe with grep"},
        {"command": "cd ..; pwd\n", "description": "Change directory and print working directory"},
        {"command": "echo \"export TEST_VAR=100\" > script.sh; chmod +x script.sh; ./script.sh; echo $TEST_VAR\n", "description": "Script execution and environment variable"},
        {"command": "echo \"Backtick test: `echo nested`\"\n", "description": "Backtick command substitution"},
        {"command": "echo \"$(echo Subshell test)\"\n", "description": "Subshell command substitution"},
        {"command": "for i in 1 2 3; do echo \"Loop $i\"; done\n", "description": "For loop"},
        {"command": "echo \"Arithmetic: $((2 + 2))\"\n", "description": "Arithmetic expansion"},
        {"command": "echo \"Brace expansion: {A,B,C}{1,2,3}\"\n", "description": "Brace expansion"},
        {"command": "echo \"Command grouping\"; (echo \"Inside grouping\"; echo \"Still inside\")\n", "description": "Command grouping"},
        {"command": "echo \"Process substitution: <(echo substituted)\"\n", "description": "Process substitution"},
        {"command": "echo \"Here document\"; cat <<EOF\nLine1\nLine2\nEOF\n", "description": "Here document"},
        {"command": "echo \"Command chaining: echo First; echo Second\"\n", "description": "Command chaining"},
        {"command": "echo \"Testing nested subshells: $(echo $(echo Nested))\"\n", "description": "Nested subshells"},
        {"command": "echo \"Redirect stderr to stdout\"; ls non_existent_file 2>&1\n", "description": "Redirect stderr to stdout"},
        {"command": "echo \"Advanced test: (cd /tmp; pwd); echo Done\"\n", "description": "Advanced test with subshell"},
        {"command": "echo \"Advanced redirection: echo Test > testfile; cat < testfile\"\n", "description": "Advanced redirection"},
        {"command": "date\n", "description": "Print current date"},
        {"command": "cal\n", "description": "Print calendar"},
        {"command": "uname -a\n", "description": "Print system information"},
        {"command": "whoami\n", "description": "Print current user"},
        {"command": "uptime\n", "description": "Print system uptime"},
        {"command": "id\n", "description": "Print user ID"},
        {"command": "echo 'Hello\nWorld'\n", "description": "Echo multi-line string"},
        {"command": "wc -w < infile\n", "description": "Word count from infile"},
        {"command": "sort infile > sortedfile\n", "description": "Sort infile and output to sortedfile"},
        {"command": "head -n 1 infile\n", "description": "Print first line of infile"},
        {"command": "tail -n 1 infile\n", "description": "Print last line of infile"},
        {"command": "cut -d' ' -f1 infile\n", "description": "Cut first word of each line in infile"},
        {"command": "grep 'content' infile\n", "description": "Grep 'content' in infile"},
        {"command": "tr 'a-z' 'A-Z' < infile\n", "description": "Translate lowercase to uppercase in infile"},
        {"command": "awk '{print $1}' infile\n", "description": "Print first column of infile"},
        {"command": "sed 's/content/CONTENTS/' infile\n", "description": "Replace 'content' with 'CONTENTS' in infile"},
        {"command": "cat infile | tee tee_output\n", "description": "Tee command example"},
        {"command": "cat infile | xargs echo\n", "description": "Xargs command example"},
        {"command": "echo 'ls\npwd' > commands.sh; sh commands.sh\n", "description": "Execute script with multiple commands"},
    ]

    for test in tests:
        run_test(test["command"], test["description"])

    # Run file redirection tests
    file_tests = [
        {"command": "cat infile > outfile\n", "input_file": "infile", "expected_output_file": "outfile", "description": "File redirection test"},
        {"command": "sort infile > sortedfile\n", "input_file": "infile", "expected_output_file": "sortedfile", "description": "Sort infile and output to sortedfile"}
    ]

    for test in file_tests:
        run_file_redirection_test(test["command"], test["input_file"], test["expected_output_file"], test["description"])

    # Print summary
    print_summary()

    # Cleanup
    cleanup_files()

if __name__ == "__main__":
    main()
