#ifndef _MISC_H_
#define _MISC_H_

#ifndef LIST_FOREACH_SAFE
#define LIST_FOREACH_SAFE(var, head, field, tvar)	\
for ((var) = LIST_FIRST((head)); \
    (var) && ((tvar) = LIST_NEXT((var), field), 1);\
    (var) = (tvar))
#endif

#endif
