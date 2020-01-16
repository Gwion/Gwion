sedi () {
  sed --version >/dev/null 2>&1 && sed -i -- "$@" || sed -i "" "$@"
}

run() {
	mdbook serve&
  sedi 's/^md/#md/' .gitignore
	while true
  do bash scripts/watch.sh
  done
}

clean() {
  trap - SIGINT SIGTERM ERR
  killall mdbook
  sedi 's/#md/md/' .gitignore
  exit 1
}

trap clean SIGINT SIGTERM ERR; run
