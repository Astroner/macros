LIBNAME=macros.h

.PHONY: concat
concat:
	cat defer.h todo.h > $(LIBNAME)