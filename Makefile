deps = src/skip.o
callgrind_opts = --read-inline-info=yes --read-var-info=yes --dump-line=yes \
								 --dump-instr=yes --collect-jumps=yes --collect-systime=yes \
								 --collect-bus=yes --cache-sim=yes --branch-sim=yes

callgrinds_targets = callgrind/skip.out callgrind/skip-n.out
callgrinds_logs    = callgrind/skip.log callgrind/skip-n.log
callgrinds_all 		 = callgrind.out callgrind.log

all: skip

define do_callgrind
	@mkdir -p $(@D)
	valgrind --tool=callgrind -v --log-file=callgrind/$(1).log \
					 --callgrind-out-file=callgrind/$(1).out \
					 $(callgrind_opts) $(2)
endef

.INTERMEDIATE: src/skip.o $(callgrinds_targets) \
	$(callgrinds_logs)

skip: $(deps)
	cc -o skip $(deps)

### Callgrind

callgrind/skip.%:	skip
	$(call do_callgrind,skip,./skip)

callgrind/skip-n.%: skip
	$(call do_callgrind,skip-n,./skip -n1)

callgrind/callgrind.out: $(callgrinds_targets)
	cat $(callgrinds_targets) > $@
	$(RM) $(callgrinds_targets)

callgrind/callgrind.log: $(callgrinds_logs)
	cat $(callgrinds_logs) > $@
	$(RM) $(callgrinds_logs)

callgrind: callgrind/callgrind.out callgrind/callgrind.log

.PHONY: clean test-clean

test-clean:
		$(RM) -r callgrind/

clean: test-clean
		$(RM) skip $(deps)
