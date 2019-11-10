serve: ensure
	@sh scripts/run.sh

build: ensure
	@mdbook build

book: build

deploy: book
	@git subtree push --prefix book origin gh-pages

ensure:
	@sh scripts/ensure.sh

clean:
	@[ -d src ] && mdbook clean

clean-tests:
	@rm -rf *.gw log vlog

clean-all: clean clean-tests
	@rm -rf src

.SUFFIXES: .gw .test
.gw.test:
	@sh scripts/test.sh $< "${CONTAINS}"
