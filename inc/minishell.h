/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jolecomt <jolecomt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 17:58:29 by jolecomt          #+#    #+#             */
/*   Updated: 2024/02/29 01:59:24 by jolecomt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

/*------------*/
/*--INCLUDES--*/
/*------------*/
# include "../src/gc/gc.h"
# include "../src/libft/libft.h"
# include "./get_next_line.h"
# include <signal.h>
# include <sys/wait.h>
# include <sys/ioctl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include <fcntl.h>
# include <stdio.h>
# include <unistd.h>

# define READ_END 0
# define WRITE_END 1
# define TOKEN_R "minishell:  error near unexpected token test '>' or '<'"
# define TOKEN_DR "minishell: error near unexpected token '>>' or '<<'"
# define TOKEN_EP_L "minishell: error near unexpected token '<' or '<<'"
# define TOKEN_EP_R "minishell: error near unexpected token '>' or '>>'"
# define TOKEN_ERR_NL "minishell: error near unexpected token 'newline"

/*--------------*/
/*--STRUCTURES--*/
/*--------------*/
typedef struct s_glob
{
	t_gcan	gc;
	int		g_state;
	int		g_state_old;
}				t_glob;

typedef struct s_prompt
{
	t_list		*cmds;
	char		**envp;
	pid_t		pid;
}				t_prompt;

typedef struct s_input
{
	char	**full_cmd;
	char	*full_path;
	int		pipein;
	int		pipeout;
}				t_input;

typedef struct s_double_str
{
	char	*s;
	char	*set;
}				t_double_str;

typedef struct s_double_sg
{
	t_glob	*g_global;
	char	*set;
}				t_double_sg;

typedef struct s_pt
{
	t_glob		*g_global;
	t_prompt	*prompt;
}				t_pt;
/*---------*/
/*--TYPES--*/
/*---------*/

typedef enum e_rl_error
{
	QUOTE = 1,
	NO_DIR,
	NO_PERM,
	NOT_CMD,
	DUP_ERR,
	FORK_ERR,
	PIPE_ERR,
	PIPES_ERR,
	ARG_ERROR,
	MEM,
	IS_DIR,
	NOT_DIR,
	SYNTAXE_DR,
	SYNTAXE_R,
	SYNTAXE_NL,
	SYNTAXE_PIP,
	PARS_ERROR	
}				t_rl_error;
/*---------*/
/*FUNCTIONS*/
/*---------*/
/*--[exec.c]--*/
void	*check_to_fork(t_prompt *prompt, t_list *cmd, int fd[2], t_glob	*g_glo);
/*--[signal.c]--*/
void	handle_sigint_cmd(int sig);
void	handle_sigint(int sig);
/*--[error.c]--*/
void	*ft_perror(int err_type, char *param, int err, t_glob *g_global);
void	syntax_error(int err_type, char **param, t_glob *g_global);
int		ft_exit(t_list *cmd, int *is_exit);
void	free_content(void *content);
void	cd_error(char **s[2], t_glob *g_global);
/*--[builtins_1.c]--*/
int		builtins(t_prompt *prompt, t_list *cmd, int *is_exit, t_glob *g_global);
int		is_builtins(t_input *node);
int		ft_pwd(void);
int		ft_echo(t_list *cmd);
int		ft_cd(t_prompt *p, t_glob *g_global);
int		ft_env(t_pt *pt, t_list *cmd);
int		var_in_envp(char *argv, char **envp, int ij[2]);

/*--[utils_1.c]--*/
int		ft_countchar(char *s, char c);
int		ft_strchr_i(const char *s, int c);
int		ft_strchars_i(const char *s, char *set);
void	*my_ptr(void *init_ptr);
void	print_error_heredoc(char c, int *i, t_glob *g_global);
/*--[utils_2.c]--*/
int		whath_is(char ***a, int *i);
void	ft_close(int fd);
int		check_arg(char *str);
void	ft_strnull(char **tab, size_t n);
/*--[ft_matrix.c]--*/
void	ft_free_matrix(char ***m);
char	**ft_extend_matrix(char **in, char *newstr, t_glob *g_global);
int		ft_matrixlen(char **m);
char	**ft_matrix_replace_in(char ***big, char **small, int n, t_glob *g_glo);
int		ft_putmatrix_fd(char **m, int nl, int fd);
/*--[ft_matrix_2.c]--*/
char	**ft_dup_matrix(char **m, t_glob *g_global);
/*--[ft_strtrim_all.c ]--*/
char	*ft_strtrim_all(char const *s1, int squote, int dquote, t_glob *g_glob);
/*--[heredoc.c]*/
int		get_here_doc(char *s[2], char *aux[2], t_glob *g_global);
/*--[env.c]--*/
char	*ft_getenv(char *var, char **envp, int n, t_glob *g_global);
char	**ft_setenv(char *var, char *value, char **envp, t_glob *g_global);
int		ft_export(t_prompt *prompt, t_glob *g_global);
int		ft_unset(t_prompt *prompt, t_glob *g_global);
/*--[get_params.c ]--*/
int		get_fd(int oldfd, char *path, int flags[2], t_glob *g_global);
t_input	*get_pipeout1(t_input *node, char **args, int *i, t_glob *g_global);
t_input	*get_pipeout2(t_input *node, char **args, int *i, t_glob *g_global);
t_input	*get_pipein1(t_input *node, char **args, int *i, t_glob *g_global);
t_input	*get_pipein2(t_input *node, char **args, int *i, t_glob *g_global);
/*--[get_cmd.c]--*/
void	*exec_cmd(t_prompt *prompt, t_list *cmd, t_glob *g_global);
/*--[expend.c]--*/
char	*expand_path(char *s, int i, int quotes[2], t_double_sg *dsg);
char	*expand_vars(char *s, int i, int quotes[2], t_pt *pt);
/*--[ft_cmdsubsplit.c ]--*/
char	**ft_cmdsubsplit(char const *s, char *set, t_glob *g_global);
/*--[ft_cmdsubsplit.c ]--*/
char	**ft_cmdtrim(char const *s, char *set, t_glob *g_global);

/*--[fill_nodes.c]--*/
t_list	*fill_nodes(char **args, int i, t_glob *g_global);
/*--[parse_args.c]--*/
void	*check_args(char *out, t_prompt *p, t_glob *g_global);
/*--[parse_args_utils.c]--*/
// int		check_string_redir_tab(char **s, t_glob *g_global);
int		check_token_redir(char **s, t_glob *g_global);
int		check_string_redir(char **s, t_glob *g_global);

#endif
