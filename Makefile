LIBNAME=macros.h

.PHONY: concat
concat:
	cat defer.h todo.h ancipe.h > $(LIBNAME)