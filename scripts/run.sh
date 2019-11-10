run() {
	mdbook serve&
	while true
  do sh scripts/watch.sh
  done
}

clean() {
  trap - SIGINT SIGTERM ERR; killall mdbook
  exit 1
}

trap clean SIGINT SIGTERM ERR; run
