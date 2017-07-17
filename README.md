# Gwion
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat-square)](http://makeapullrequest.com) 
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/7669efe4c1c84b888cea5885201c79ae)](https://www.codacy.com/app/fennecdjay/Gwion?utm_source=github.com&utm_medium=referral&utm_content=fennecdjay/Gwion&utm_campaign=badger)
[![Build Status](https://travis-ci.org/fennecdjay/Gwion.svg?branch=dev)](https://travis-ci.org/fennecdjay/Gwion)
[![Coveralls branch](https://img.shields.io/coveralls/fennecdjay/Gwion/dev.svg)](https://coveralls.io/github/fennecdjay/Gwion?branch=dev)
![](http://b.repl.ca/v1/version-pre--pre--alpha-red.svg)
[![release](http://github-release-version.herokuapp.com/github/fennecdjay/Gwion/release.svg)](https://github.com/fennecdjay/Gwion/releases/latest)
![](https://img.shields.io/badge/language-C-yellowgreen.svg)  

gwion is a object-oriented programming language, aimed at making music

**strongly** inspired by [chuck](http://chuck.stanford.edu/)  
synthesis is based on [soundpipe](http://paulbatchelor.github.io/proj/soundpipe.html)  
*high-level* features: variadic functions, templates, func pointers.
a plugin systems makes it easily extendable

#### simple example code (hello_world.gw):

```cpp
// print hello world
<<<"Hello World">>>;
// create a sinusoidal generator and connect it to dac  
SinOsc s => dac;  
// let 5 seconds pass  
5::second => now;  
// you're done!
```
to run this, do

```sh
./gwion hello_world.gw
```

learn more about options:

```
./gwion --help
```

#### requires

* [soundpipe](https://github.com/PaulBatchelor/Soundpipe)

#### build

first you need to configure the build system., look [here](https://github.com/fennecdjay/Gwion/blob/dev/Configure.md).
also, 

```
./configure --help
```
should show a list of available options.

then, just
```
make
```

#### issues

if anyting goes wrong or seems unclear, please report an [issue](https://github.com/fennecdjay/Gwion/issues/new).  

#### contribute 

Every helping hand is welcome!!!
if you wish to do something on [Gwion](https://github.com/fennecdjay/Gwion), please use the [issue tracker](https://github.com/fennecdjay/Gwion/issues),
either to find a issue you can fix, or add a new one.  
Pull Requests also highly appreciated.
