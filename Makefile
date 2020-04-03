serve: ensure
	@sh scripts/run.sh

build: ensure
	@mdbook build

deploy: build
	@sh scripts/deploy.sh

ensure: style.css
	@sh scripts/ensure.sh

lint:
	vale docs

clean:
	@[ -d src ] && mdbook clean || true

clean-tests:
	@rm -rf *.gw log vlog

clean-all: clean clean-tests
	@rm -rf src

style.css: ansi2html.sh
	@bash ansi2html.sh --css-only > ansi2html.css

ansi2html.sh:
	@curl https://raw.githubusercontent.com/pixelb/scripts/master/scripts/ansi2html.sh -o $@

.SUFFIXES: .gw .test
.gw.test:
	@bash scripts/test.sh $< "${CONTAINS}"
