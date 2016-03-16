#ifndef __PRIVILEGE_CHECKER_H
#define __PRIVILEGE_CHECKER_H

#define IME_PRIVILEGE "http://tizen.org/privilege/ime"

bool inputmethod_cynara_initialize();
void inputmethod_cynara_finish();
bool check_privilege(const char *uid, const char *privilege);

#endif //__PRIVILEGE_CHECKER_H
