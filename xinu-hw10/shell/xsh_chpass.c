/**
 * @file     xsh_chpass.c
 * @provides xsh_chpass
 *
 */
/* Embedded XINU, Copyright (C) 2024.  All rights reserved. */

#include <xinu.h>

/**
 * Shell command (chpass) changes an existing user password.
 * @param args array of arguments
 * @return OK for success, SYSERR for errors.
 */
command xsh_chpass(int nargs, char *args[])
{
/**
 * TODO:
 * This function creates a new password for an existing user.
 * You may break this task down into any number of new helper
 * functions within this file, and also may rely on helper functions
 * that already exist, such as getusername(), hasspassword(), and
 * passwdFileWrite().
 *
 * Steps:
 * 1) If no user name was provided to the shell command, superusr should
 *    be prompted for one.  Normal users are not prompted, because we
 *    default to changing their own password.
 * 2) Search for the user name in the usertab.
 * 3) If the current user is not superusr, prompt for the previous password.
 *    Prompt text = "Enter previous password for user %s: ".
 * 4) If the hash of the previous password matched what is on record,
 *    prompt for new password.
 *    Prompt test = "Enter new password for user %s: ".
 * 5) Place the new password hash into the user entry, and commit to disk.
 * 6) Printf "Successfully changed password for user ID %d\n" with user ID.
 *
 * Errors to watch for:
 * 1) There is not already a user logged in.
 *    Error text = "Must login first\n".
 * 2) The logged in userid is not SUPERUID, but is trying to change someone
 *    else's password.
 *    Error text = "ERROR: Only superusr can change other passwords!\n".
 * 3) The given user name cannot be found in the existing user table.
 *    Error text = "User name %s not found.\n".   
 * 4) The password change failed.  (i.e., passwords didn't match.)
 *    Error text = "Password for user %s does not match!\n".
 */
    return OK;
}
