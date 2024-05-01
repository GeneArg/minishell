#include "../include/minishell.h"

int	main(void)
{
	char		*input;
	t_token		*head_token;
	t_token		*current;
	t_command	*head_command;
	t_command	*current_command;

	input = "grep 'pattern' file1.txt | sort | uniq > results.txt";
	printf("INPUT: %s\n", input);
	head_token = lex(input);
	current = head_token;
	printf("\nLEXER OUTPUT:\n");
	while (current)
	{
		printf("Type: %d, Value: %s\n", current->type, current->value);
		current = current->next;
	}
	printf("\nPARSER OUTPUT:\n");
	head_command = parse(head_token);
	current_command = head_command;
	while (current_command)
	{
		printf("Command: %s\n", *(current_command->argv));
		current_command->argv++;
		while (*(current_command->argv))
		{
			printf("Arg: %s\n", *(current_command->argv));
			current_command->argv++;
		}
		if (current_command->redirect_in)
			printf("Redirect in: %s\n", current_command->redirect_in);
		if (current_command->redirect_out)
			printf("Redirect out: %s\n", current_command->redirect_out);
		if (current_command->heredoc_content)
			printf("Heredoc content: %s\n", current_command->heredoc_content);
		if (current_command->append)
			printf("Append: %d\n", current_command->append);
		current_command = current_command->next;
		printf("\n");
	}
}
