ifeq (,$(wildcard docs/config.mk))
$(shell cp docs/config.mk.orig docs/config.mk)
endif
include docs/config.mk

mdr_list = (filter-out docs/09_Benchmarks.mdr, $(shell find docs -type f -name "*.mdr"))
md_list  = $(mdr_list:.mdr=.md)

_docserver_config       = bash help/doc-config.sh > mkdocs.yml
_docserver_serve = $(call _docserver_config); $(DOCTOOL) -q serve
_docserver_launch = $(call _docserver_serve)& echo $$! > .server_pid
_docserver_kill   = [ -f .server_pid ] && (kill $$(cat .server_pid); rm .server_pid) || true
_mdr_wait=$$(inotifywait -q -r docs --format "%w%f" | tail -n1)

doc-run:
	@bash -c "trap 'trap - SIGINT SIGTERM ERR; $(MAKE) -s doc-clean; exit 1' SIGINT SIGTERM ERR; $(MAKE) -s doc-watch"

doc-watch: ${md_list}
	@$(call _docserver_launch)
	@while true; do file=$(call _mdr_wait); echo $$file | grep '\.mdr$$' && mdr $$file; done

doc-serve: $(md_list)
	@$(call _docserver_serve)

doc-clean:
	-@$(call _docserver_kill)
	@rm -rf *.gw vlog log $(md_list) .tmp_list .mdr_list site

doc-build: $(md_list)
	$(DOCTOOL) build

doc-deploy: $(md_list)
	-@$(call _docserver_config)
	@$(DOCTOOL) gh-deploy
	@$(MAKE) -s doc-clean

.PHONY: mdr_loop watch serve clean run

.SUFFIXES: .mdr .md
.mdr.md:
	$(info compiling $<)
	@mdr $< ||:

.SUFFIXES: .gw .test
.gw.test:
	@${VALGRIND} ${VALGRIND_OPT} gwion $< 2>&1 > log
	@[ -t 1 ] && $(call _interm) || $(call _noterm)

define _test_check
	for a in $(CONTAINS); do grep "$$a" log >/dev/null; done && valgrind_parse vlog
endef

_interm_status=printf "$(INTERM_OK)\n" || printf "$(INTERM_NOT_OK)\n"
_interm=(cat log; $(call _test_check) && $(call _interm_status))

_noterm_log=sed 's/$$/\<br\/>/' log
_noterm_status=printf "$(NOTERM_OK)\n" || printf "$(NOTERM_NOT_OK)\n"
_noterm_test=$(call _test_check) && $(call _noterm_status)
_noterm_header=echo '<p style=$(CSS)>'
_noterm_footer=echo '</p>'
_noterm=($(call _noterm_header); $(call _noterm_log); $(call _noterm_test); $(call _noterm_footer))
