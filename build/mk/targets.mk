all: opt-debug

opt-debug:
	@for s in $(SUBDIRS); do \
		$(MAKE) -C $$s opt-debug; \
	done

debug:
	@for s in $(SUBDIRS); do \
		$(MAKE) -C $$s debug; \
	done

opt:
	@for s in $(SUBDIRS); do \
		$(MAKE) -C $$s opt; \
	done

clean: clean-opt-debug

clean-opt-debug:
	@for s in $(SUBDIRS); do \
		echo Cleaning in $$s; \
		$(MAKE) -C $$s clean-opt-debug; \
	done

clean-debug:
	@for s in $(SUBDIRS); do \
		echo Cleaning in $$s; \
		$(MAKE) -C $$s clean-debug; \
	done

clean-opt:
	@for s in $(SUBDIRS); do \
		echo Cleaning in $$s; \
		$(MAKE) -C $$s clean-opt; \
	done
