SUBDIRS = Client Server

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: $(SUBDIRS)

clean:
	cd Client && $(MAKE) clean && cd .. && cd Server && $(MAKE) clean
