# Do recursive builds
all:
	$(MAKE) -C ubftab
	$(MAKE) -C xmlpars
	$(MAKE) -C ubfpars
	$(MAKE) -C clt
	$(MAKE) -C srv

clean:
	$(MAKE) -C xmlpars clean
	$(MAKE) -C ubfpars clean
	$(MAKE) -C ubftab clean
	$(MAKE) -C clt clean
	$(MAKE) -C srv clean

