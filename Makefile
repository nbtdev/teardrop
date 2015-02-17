SUBDIRS = tech

DIR = `pwd`

ROOT := $(CURDIR)
export ROOT

all:
	@for s in $(SUBDIRS); do \
		$(MAKE) -C $$s; \
	done

clean:
	@for s in $(SUBDIRS); do \
		echo Cleaning in $$s; \
		$(MAKE) -C $$s clean; \
	done
