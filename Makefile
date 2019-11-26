serve: ensure
	@sh scripts/run.sh

build: ensure
	@mdbook build

deploy: build
	@sh scripts/deploy.sh

ensure:
	@sh scripts/ensure.sh

lint:
	vale docs

clean:
	@[ -d src ] && mdbook clean || true

clean-tests:
	@rm -rf *.gw log vlog

clean-all: clean clean-tests
	@rm -rf src book.toml custom.css

.SUFFIXES: .gw .test
.gw.test:
	@sh scripts/test.sh $< "${CONTAINS}"
