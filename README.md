# minishell
A simple shell

* pipe `|`
* redirections `<`, `>`, `<<`, `>>`
* expand variables `$`

----------------------------------------------------------------------------

Finished with tester

- minishell_tester all (https://github.com/LucasKuhn/minishell_tester)
- 'mstest m' at 1594 (https://github.com/zstenger93/42_minishell_tester)

## Usage

Compile Minishell `make`

Run `./minishell`

## For test memory leak

`valgrind --suppressions=readline.supp --leak-check=full --show-leak-kinds=all ./minishell`
