SUBDIRS = file_log \
		monitering_log \
		P_E_log \
		proc_log \
		remote_logon

target :
	for DIRS in $(SUBDIRS); do \
		$(MAKE) -C $$DIRS; \
	done

clean :
	for DIRS in $(SUBDIRS); do \
		$(MAKE) -C $$DIRS clean; \
	done