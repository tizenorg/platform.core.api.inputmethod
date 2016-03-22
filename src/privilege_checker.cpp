#include "privilege_checker_private.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlog.h>
#include <unistd.h>

#include <cynara-client.h>
#include <cynara-error.h>
#include <cynara-session.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "INPUTMETHOD"

namespace
{

cynara *p_cynara = NULL;

}

bool
inputmethod_cynara_initialize()
{
    int ret = cynara_initialize(&p_cynara, NULL);
    if (ret != CYNARA_API_SUCCESS)
        LOGW("Failed to call cynara_initialize (). ret : %d.", ret);

    return ret == CYNARA_API_SUCCESS;
}

void
inputmethod_cynara_finish()
{
    if (p_cynara)
        cynara_finish(p_cynara);

    p_cynara = NULL;
}

bool
check_privilege(const char *uid, const char *privilege)
{
    FILE *fp = NULL;
    char smack_label[1024] = "/proc/self/attr/current";

    if (!p_cynara) {
        return false;
    }

    fp = fopen(smack_label, "r");
    if (fp != NULL) {
        if (fread(smack_label, 1, sizeof(smack_label), fp) <= 0)
            LOGW("Error : fread");

        fclose(fp);
    }

    pid_t pid = getpid();
    char *session = cynara_session_from_pid(pid);
    int ret = cynara_check(p_cynara, smack_label, session, uid, privilege);
    if (session)
        free(session);

    if (ret != CYNARA_API_ACCESS_ALLOWED) {
        LOGW("Access denied. The result of cynara_check() : %d.", ret);
        return false;
    }
    return true;
}

