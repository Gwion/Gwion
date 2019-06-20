![](https://img.shields.io/badge/language-C-green.svg)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](http://makeapullrequest.com)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/fennecdjay/Gwion.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/fennecdjay/Gwion/context:cpp)
[![Build Status](https://travis-ci.org/fennecdjay/Gwion.svg?branch=master)](https://travis-ci.org/fennecdjay/Gwion)
<!--[![Coveralls branch](https://img.shields.io/coveralls/fennecdjay/Gwion/master.svg)](https://coveralls.io/github/fennecdjay/Gwion?branch=master)-->
# Gwion
gwion is a object-oriented programming language, aimed at making music

**strongly** inspired by [chuck](http://chuck.stanford.edu/), but adding a bunch *high-level* features:  
	  templates, first-class functions and more.  

:gift: a [plugin system](https://github.com/fennecdjay/Gwion-plug) makes it easily extendable

### simple example code (hello_world.gw):

```cpp
// print hello world
<<<"Hello World">>>;
```
to run this, do

```sh
./gwion hello_world.gw
```
You want to know more? :smile: Look [here](https://fennecdjay.github.io/gwion/)
Still WIP :construction_worker: but a nice place to learn and [contribute](https://github.com/fennecdjay/gwion/issues)

## Build
### Download the source
This project makes use of submodules, so you can clone it with
``` sh
git clone --recursive https://github.com/fennecdjay/Gwion
```
or if you just want the minimum to start with, try
``` sh
git clone https://github.com/fennecdjay/Gwion
git submodule update --init util ast
```

You can now move to the source directory
``` sh
cd Gwion
```

### Configure (optionnal)
You might want to make sure every compile-time option is set to your liking.
First list the files that set those.
``` sh
find . -name "config.mk"
```
You can now edit then to your preference.

> Besides develloper options, the most important one is *USE_DOUBLE*, in util/config.mk, which set the floating point size (float or double).

### Compiling

```
make
```

### Other make targets
Other basic targets include: clean install uninstall test


## Reporting bugs / Contributing

:+1: Every helping hand is welcome!!! :+1:  
:book: Please see the [contributing](.github/CONTRIBUTING.md) page for more information.

## Acknowledgements.
The whole [Chuck](http://chuck.cs.princeton.edu/) team, for inspiration.  
[Paul Batchelor](https://github.com/PaulBatchelor) and the great [soundpipe](https://github.com/PaulBatchelor/Soundpipe) library, that got me started.  
And of course all the [contributors](https://github.com/fennecdjay/Gwion/graphs/contributors), not to mention the OSS community.
