# Gwion

[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/fennecdjay/Gwion.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/fennecdjay/Gwion/context:cpp)
[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/2417/badge)](https://bestpractices.coreinfrastructure.org/projects/2417)
[![Coverage](https://raw.githubusercontent.com/fennecdjay/gwion-coverage-report/master/badge.svg?sanitize=true)](https://fennecdjay.github.io/gwion-coverage-report/master)
[![All Contributors](https://img.shields.io/badge/all_contributors-3-orange.svg)](#contributors)

![Linux](https://github.com/fennecdjay/Gwion/workflows/Linux/badge.svg)
![MacOs](https://github.com/fennecdjay/Gwion/workflows/MacOs/badge.svg)
![Windows](https://github.com/fennecdjay/Gwion/workflows/Windows/badge.svg)

Gwion is a programming language, aimed at making music

**strongly** inspired by [chuck](http://chuck.stanford.edu/), but adding a bunch *high-level* features:  
	  templating, first-class functions and more.  
<!-- TODO: add benchmarks in doc and link to it -->
It aims to be simple, small,
 [fast](https://fennecdjay.github.io/Gwion/#Benchmarks/),
 [extendable](https://github.com/fennecdjay/Gwion-plug) and [embeddable](https://github.com/fennecdjay/Gwion/blob/master/src/main.c#L18-L31).

### simple example code (hello_world.gw):

```cpp
// print hello world
<<< "Hello World" >>>;
```
to run this, do

```sh
./gwion hello_world.gw
```
You want to know more? :smile: Look [here](https://fennecdjay.github.io/Gwion/)
Both outdated and WIP :construction_worker: but a nice place to learn and [contribute](https://github.com/fennecdjay/gwion/issues)

## Build
### Download the source
You might just want the minimum to start with, try
``` sh
git clone https://github.com/fennecdjay/Gwion
cd Gwion
git submodule update --init util ast
make
```

You can get a list of config files to tweak with
``` sh
find . -name "config.mk"
```
> Besides develloper options, you migth want to check *USE_DOUBLE*, in util/config.mk, which set the floating point size (float or double).

## Reporting bugs / Contributing

:+1: Every helping hand is welcome!!! :+1:  

If there's anything you see that can make Gwion better, please let us know!

:book: Please see the [contributing](.github/CONTRIBUTING.md) page for more information.

## Acknowledgements.
The whole [Chuck](http://chuck.cs.princeton.edu/) team, for inspiration.  
[Paul Batchelor](https://github.com/PaulBatchelor) and the awesome [soundpipe](https://github.com/PaulBatchelor/Soundpipe) library, that got me started.

## Contributors

Thanks goes to these wonderful people ([emoji key](https://github.com/kentcdodds/all-contributors#emoji-key)):
<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore -->
<table><tr><td align="center"><a href="http://paulbatchelor.github.io"><img src="https://avatars3.githubusercontent.com/u/8139389?v=4" width="100px;" alt="Paul Batchelor"/><br /><sub><b>Paul Batchelor</b></sub></a><br /><a href="#question-PaulBatchelor" title="Answering Questions">💬</a> <a href="https://github.com/fennecdjay/Gwion/issues?q=author%3APaulBatchelor" title="Bug reports">🐛</a> <a href="https://github.com/fennecdjay/Gwion/commits?author=PaulBatchelor" title="Code">💻</a> <a href="#ideas-PaulBatchelor" title="Ideas, Planning, & Feedback">🤔</a></td><td align="center"><a href="https://github.com/originalsouth"><img src="https://avatars1.githubusercontent.com/u/5300799?v=4" width="100px;" alt="Benny"/><br /><sub><b>Benny</b></sub></a><br /><a href="#question-originalsouth" title="Answering Questions">💬</a> <a href="https://github.com/fennecdjay/Gwion/issues?q=author%3Aoriginalsouth" title="Bug reports">🐛</a> <a href="https://github.com/fennecdjay/Gwion/commits?author=originalsouth" title="Code">💻</a></td><td align="center"><a href="https://github.com/scalarwaves"><img src="https://avatars1.githubusercontent.com/u/4212896?v=4" width="100px;" alt="Andrew Prentice"/><br /><sub><b>Andrew Prentice</b></sub></a><br /><a href="#question-scalarwaves" title="Answering Questions">💬</a> <a href="https://github.com/fennecdjay/Gwion/issues?q=author%3Ascalarwaves" title="Bug reports">🐛</a> <a href="https://github.com/fennecdjay/Gwion/commits?author=scalarwaves" title="Code">💻</a> <a href="#ideas-scalarwaves" title="Ideas, Planning, & Feedback">🤔</a></td></tr></table>

<!-- ALL-CONTRIBUTORS-LIST:END -->
This project follows the [all-contributors](https://github.com/kentcdodds/all-contributors) specification. Contributions of any kind welcome!

>    NOTE: if you should be on the list of contributors but we forgot you, don't be shy and let us know!
