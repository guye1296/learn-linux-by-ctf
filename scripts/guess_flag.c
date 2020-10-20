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
#include <limits.h>

#define TERMINAL_BOLD_GREEN  "\033[1;32m"
#define TERMINAL_RED "\033[0;31m"
#define TERMINAL_BOLD_CYAN "\033[1;36m"
#define TERMINAL_COLOR_RESET "\033[0m"

// for some reason limits.h does not contain MAX_PATH
// on some platforms
#ifndef MAX_PATH
#define MAX_PATH (4094)
#endif 

/* Ask the user for a password.
   Return true if the user gives the correct password for entry PW,
   false if not.  Return true without asking for a password if run by UID 0
   or if PW has an empty password.  */
static bool guess_flag(const struct passwd *pw)
{
    char *encrypted, *correct;
    char guess[128];
    char *index = NULL;
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
    printf("Enter Flag for " TERMINAL_BOLD_CYAN "%s" TERMINAL_COLOR_RESET ": ", pw->pw_name);
    fgets(guess, 128, stdin);

    // remove newline from string
    index = strchr(guess, '\n');
    if (NULL != index) {
        *index = '\0';
    }

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

static void run_shell (char const *shell, bool prompt)
{
    int exit_status;

    // TODO: change to actuall shell
    char *args[] = {"/bin/bash", NULL};

    // set prompt environ
    if(prompt) {
        setenv("PROMPT_CHALLENGE", "1", true);
    } else {
        setenv("PROMPT_CHALLENGE", "0", true);
    }

    execv(shell, args);
    exit_status = (errno == ENOENT ? ENOENT : 3);
    exit(exit_status);
}



static char* get_current_dir() 
{
    // all calls will be null if failed
    char* curdir = malloc(MAX_PATH);
    curdir = getcwd(curdir, MAX_PATH);

    cleanup:
        return curdir;
}



static int change_context(char *user)
{
    unsigned int status = 0;
    struct passwd *pw = NULL;
    struct passwd pw_copy;
    char *dir = NULL;
    bool same_user = false;

    pw = getpwnam(user);
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

    // if same user, its proably a failure. skip flag check
    // in addition, do not change its directory
    same_user = (pw->pw_uid ==getuid());
    if (!same_user) {
        if (!guess_flag(pw)) {
            fprintf(stderr, TERMINAL_RED "incorrect flag :(" TERMINAL_COLOR_RESET "\n");
            status = 2;
            goto cleanup;
        } 
        printf(TERMINAL_BOLD_GREEN "Good job!" TERMINAL_COLOR_RESET " Moving to the next challenge\n");
    } 
    else {
        if (pw->pw_dir) {
            (void)free(pw->pw_dir);
        }
        pw->pw_dir = get_current_dir();
    }


    // flag is correct! yay!!
    modify_environment(pw, pw->pw_shell);
    change_identity(pw);

    if(chdir(pw->pw_dir)) {
        fprintf(stderr, "warning: can not change to dir %s\n", pw->pw_dir);
    }

    // verbose code for readability
    if (same_user) {
        run_shell(pw->pw_shell, false);
    } else {
        run_shell(pw->pw_shell, true);
    }

    cleanup:
    if(dir) {
        (void)free(dir);
    }
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


char *get_current_user()
{
    struct passwd *pw = NULL;
    char *user = NULL;

    pw = getpwuid(getuid());
    endpwent();

    if(pw && pw->pw_name) {
        user = strdup(pw->pw_name);
    }

    return user;
}


// user probably guessed flag wrong
int fallback() {
    // get user name
    int status = 0;
    char* current_user = get_current_user();

    if (NULL == current_user) {
        status = 1;
        goto cleanup;
    }

    status = change_context(current_user);

cleanup:
    if(NULL != current_user) {
        free(current_user);
    }

    return status;
}


int main(int argc, char *argv[]) {
    int status = 0;
    char *next_challenge;

    if (2 != argc) {
        fprintf(stderr, "USAGE: %s [CHALLENGE]\n", argv[0]);
        exit(1);	
    }

    next_challenge = argv[1];
    status = change_context(next_challenge);

    if (0 != status) {
        status = fallback();
    }

    return status;
}
