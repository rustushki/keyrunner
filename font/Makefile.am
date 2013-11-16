keyrunnerdir = $(prefix)/share/keyrunner
fontdir=$(keyrunnerdir)/font

install-data-local:
	@$(NORMAL_INSTALL)
	$(MKDIR_P) $(DESTDIR)$(fontdir)
	$(INSTALL_DATA) `ls -1 *ttf` $(DESTDIR)$(fontdir)

