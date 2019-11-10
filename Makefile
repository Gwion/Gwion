serve: ensure
	@sh scripts/run.sh

build: ensure
	@mdbook build

book: build

deploy: book
	@$(shell git push origin :gh-pages || true)
	@git add -f book
	@git commit -m "add book"
	@git subtree push --prefix book origin gh-pages
	eho hehe
	@git rm -r book
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
