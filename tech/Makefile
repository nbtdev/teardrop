SUBDIRS = \
	Asset \
	Core \
	Gfx \
	Math \
	Memory \
	Package \
	Stream \
	Reflection \
	Util \
	VFX

all:
	@echo $(ROOT)
	@for s in $(SUBDIRS); do \
		$(MAKE) -C $$s; \
	done

clean:
	@for s in $(SUBDIRS); do \
		echo Cleaning in $$s; \
		$(MAKE) -C $$s clean; \
	done
