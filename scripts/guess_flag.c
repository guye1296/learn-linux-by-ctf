#include <stdio.h>
#include <pwd.h>
#include <grp.h>
#include <stdlib.h>
#include <string.h>
#include <crypt.h> 
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <errno.h>
#include <shadow.h>


/* Ask the user for a password.
   Return true if the user gives the correct password for entry PW,
   false if not.  Return true without asking for a password if run by UID 0
   or if PW has an empty password.  */
static bool guess_flag(const struct passwd *pw)
{
    char *guess, *encrypted, *correct;
    struct spwd *spwd = getspnam(pw->pw_name);

    if (NULL == spwd) {
        fprintf(stderr, "not root\n");
        return false;
    }

    correct = spwd->sp_pwdp;
    
    // if is root or empty flag, return true
    if (getuid () == 0 || !correct || correct[0] == '\0')
        return true;

    // ask user to enter flag
    printf("Enter Flag: ");
    scanf("%s", guess);

    if (!guess)
    {
        fprintf(stderr, "cannot open /dev/tty\n");
        return false;
    }

    encrypted = crypt(guess, correct);

    // zero out the guess, leave unencrypted for little as possible
    memset(guess, 0, strlen(guess));

    return 0 == strcmp(encrypted, correct);
}


/* Update `environ' for the new shell based on PW, with SHELL being
   the value for the SHELL environment variable.  */
static void modify_environment (const struct passwd *pw, const char *shell)
{
    setenv("HOME", pw->pw_dir, true);
    setenv("SHELL", shell, true);
    setenv("USER", pw->pw_name, true);
    setenv("LOGNAME", pw->pw_name, true);
}


/* Become the user and group(s) specified by PW.  */
static void change_identity (const struct passwd *pw)
{
    if (setgid (pw->pw_gid)) {
        fprintf(stderr, "cannot set group id\n");
        exit(1);
    }
    if (setuid (pw->pw_uid)) {
        fprintf(stderr, "cannot set user id\n");
        exit(1);
    }
}


/* Run SHELL, or DEFAULT_SHELL if SHELL is empty.
   If COMMAND is nonzero, pass it to the shell with the -c option.
   Pass ADDITIONAL_ARGS to the shell as more arguments; there
   are N_ADDITIONAL_ARGS extra arguments.  */

static void run_shell (char const *shell)
{
    int exit_status;
    // TODO: change to actuall shell
    char *args[] = {"/bin/bash", NULL};

    execv(shell, args);
    exit_status = (errno == ENOENT ? ENOENT : 3);
    exit(exit_status);
}


// TODO: actually implement
static char* get_next_challenge() {
	char* next = "challenge99";
	return strdup(next);
}



int main(int argc, char *argv[]) {
    int status = 0;
    char *next_challenge;
    struct passwd *pw = NULL;
    struct passwd pw_copy;

    next_challenge = get_next_challenge();

    pw = getpwnam(get_next_challenge());
    (void)free(next_challenge);
    if (! (pw && pw->pw_name && pw->pw_name[0] && pw->pw_dir && pw->pw_dir[0] && pw->pw_passwd)) {
        fprintf(stderr, "user does not exist\n");
        status = 1;
        goto cleanup;
    }

    /* Make a copy of the password information and point pw at the local
        copy instead.  Otherwise, some systems (e.g. GNU/Linux) would clobber
        the static data through the getlogin call from log_su.
        Also, make sure pw->pw_shell is a nonempty string.
        It may be NULL when NEW_USER is a username that is retrieved via NIS (YP),
        but that doesn't have a default shell listed.  */
    pw_copy = *pw;
    pw = &pw_copy;
    pw->pw_name = strdup(pw->pw_name);
    pw->pw_passwd = strdup(pw->pw_passwd);
    pw->pw_dir = strdup(pw->pw_dir);
    pw->pw_shell = strdup(pw->pw_shell && pw->pw_shell[0] ? pw->pw_shell : "/bin/bash");
    endpwent();

    if (!guess_flag(pw)) {
        fprintf(stderr, "incorrect flag :(\n");
        status = 2;
        goto cleanup;
    }

    printf("Good job! Moving to the next challenge\n");

    // flag is correct! yay!!
    modify_environment(pw, pw->pw_shell);
    change_identity(pw);
    if(chdir(pw->pw_dir)) {
        fprintf(stderr, "warning: can not change to dir %s\n", pw->pw_dir);
    }
    run_shell(pw->pw_shell);

cleanup:
    if(pw->pw_name) {
        (void)free(pw->pw_name);
    }
    if(pw->pw_passwd) {
        (void)free(pw->pw_passwd);
    }
    if(pw->pw_dir) {
        (void)free(pw->pw_dir);
    }
    if(pw->pw_shell) {
        (void)free(pw->pw_shell);
    }
    return status;
}
