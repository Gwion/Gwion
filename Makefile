serve: ensure
	@sh scripts/run.sh

build: ensure
	@mdbook build

book: build

deploy: book
	@git checkout gh-pages && git pull && git checkout mdbook
	@git add -f book
	@git commit -m "add book"
	@echo lol
	@git subtree push --prefix book origin gh-pages
	@git rm -rf book
	@rm -rf book
	@git commit -m "remove book"

ensure:
	@sh scripts/ensure.sh

clean:
	@[ -d src ] && mdbook clean || true

clean-tests:
	@rm -rf *.gw log vlog

clean-all: clean clean-tests
	@rm -rf src book.toml custom.css

.SUFFIXES: .gw .test
.gw.test:
	@sh scripts/test.sh $< "${CONTAINS}"
