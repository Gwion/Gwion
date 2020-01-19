NOTERM_OK="&#10004;"
NOTERM_NOT_OK="&#10008;"

# terminal status
INTERM_OK="\033[32mOK\033[0m"
INTERM_NOT_OK="\033[31mNOT OK\033[0m"

check() {
  for a in $@;
  do grep "$a" log >/dev/null || return 1
  done #&& [ -z "${VALGRIND}" ] || return valgrind_parse vlog
}

interm() {
  cat log; check $@ && printf "${INTERM_OK}\n" || printf "${INTERM_NOT_OK}\n"
}

noterm() {
  echo '<p class="mdr">'
  sed 's/$/<\/br>/' log
  echo "</br>"
  check $@ && printf "${NOTERM_OK}\n" || printf "${NOTERM_NOT_OK}\n"
  echo '</p>'

}

${VALGRIND} ${VALGRIND_OPT} gwion "$1" 2>&1 > log
shift
[ -t 1 ] && interm $@ || noterm $@
