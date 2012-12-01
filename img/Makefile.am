keyrunnerdir = $(prefix)/share/keyrunner
imgdir=$(keyrunnerdir)/img

install-data-local:
	@$(NORMAL_INSTALL)
	$(MKDIR_P) $(DESTDIR)$(imgdir)
	$(INSTALL_DATA) `ls -1 *png` $(DESTDIR)$(imgdir)
