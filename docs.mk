mdr_list = $(shell find -regextype posix-egrep -regex '.*.mdr$$')
md_list  = $(mdr_list:.mdr=.md)

_docserver_launch = $(DOCTOOL) -q serve & echo $$! > .server_pid
_docserver_kill   = [ -f .server_pid ] && (kill $$(cat .server_pid); rm .server_pid) || true

_mdr_wait=$$(inotifywait -q -r docs --format "%w%f" | tail -n1)

doc-run:
	@bash -c "trap 'trap - SIGINT SIGTERM ERR; $(MAKE) -s doc-clean; exit 1' SIGINT SIGTERM ERR; $(MAKE) -s doc-watch"

doc-watch: $(md_list)
	@$(call _docserver_launch)
	@while true; do file=$(call _mdr_wait); echo $$file | grep '\.mdr$$' && mdr $$file; done

doc-serve:
	@(call _docserver_launch)

doc-clean:
	-@$(call _docserver_kill)
	@rm -rf *.gw vlog log $(md_list) .tmp_list .mdr_list site

doc-build: $(md_list)
	$(DOCTOOL) build

doc-deploy: $(md_list)
	@$(DOCTOOL) gh-deploy
	@$(MAKE) -s doc-clean

.PHONY: mdr_loop watch serve clean run

.SUFFIXES: .mdr .md
.mdr.md:
	$(info compiling $<)
	@mdr $< ||:

ifeq ($(VALGRIND), NO_VALGRIND)
VALGRIND     =
VALGRIND_OPT =
endif

include config.mk

.SUFFIXES: .gw .test
.gw.test:
	@${VALGRIND} ${VALGRIND_OPT} gwion $< &> log
	@[ -t 1 ] && $(call _interm) || $(call _noterm)

define _test_check
	for a in $(CONTAINS); do grep "$$a" log >/dev/null; done && valgrind_parse vlog
endef

_interm_status=echo -e "$(INTERM_OK)" || echo -e "$(INTERM_NOT_OK)"
_interm=(cat log; $(call _test_check) && $(call _interm_status))

_noterm_log=sed 's/$$/\<br\/>/' log
_noterm_status=echo -e "$(NOTERM_OK)" || echo -e "$(NOTERM_NOT_OK)"
_noterm_header=echo '<blockquote><p style=$(CSS)>'
_noterm_test=$(call _test_check) && $(call _noterm_status)
_noterm_footer=echo '</p></blockquote>'
_noterm=($(call _noterm_header); $(call _noterm_log); $(call _noterm_test); $(call _noterm_footer))
DOCTOOL ?= mkdocs

# output box css
BACKGROUND    = background-color:\#e3e3e3;
BORDER        = border: 5px solid \#343131;
PADDING       = padding: 10px;
MARGIN        = margin-right: 20%; margin-left: 20%;
BORDER_RADIUS = -moz-border-radius: 15px; -webkit-border-radius: 15px;
CSS           = "$(BACKGROUND) $(BORDER) $(PADDING) $(MARGIN) $(BORDER_RADIUS)"

VALGRIND     ?= valgrind
VALGRIND_LOG ?= vlog
VALGRIND_OPT += --leak-check=yes --log-file=${VALGRIND_LOG}

INTERM_OK       = \033[32mOK\033[0m
INTERM_NOT_OK   = \033[31mNOT OK\033[0m

NOTERM_OK= &\#10004;
NOTERM_NOT_OK= &\#10008;
