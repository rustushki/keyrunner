keyrunnerdir = $(prefix)/share/keyrunner
leveldir=$(keyrunnerdir)/level

install-data-local:
	@$(NORMAL_INSTALL)
	$(MKDIR_P) $(DESTDIR)$(leveldir)
	$(INSTALL_DATA) `ls -1 | grep '[0-9]' -` $(DESTDIR)$(leveldir)
