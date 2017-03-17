# Gwion
[![Build Status](https://travis-ci.org/fennecdjay/Gwion.svg?branch=dev)](https://travis-ci.org/fennecdjay/Gwion)
[![Coveralls branch](https://img.shields.io/coveralls/fennecdjay/Gwion/dev.svg)](https://coveralls.io/github/fennecdjay/Gwion?branch=dev)
![](http://b.repl.ca/v1/version-pre--pre--alpha-red.svg)
![](https://img.shields.io/badge/version-pre--pre--alpha-red.svg)
![](https://img.shields.io/badge/language-C-yellowgreen.svg)  

gwion is a object-oriented programming language, aimed at making music

**strongly** inspired by [chuck](http://chuck.stanford.edu/)  
synthesis is based on [soundpipe](http://paulbatchelor.github.io/proj/soundpipe.html)  

realtime audio input/output can be done using  
*  [libsoundio](http://libsound.io/)  
*  [portaudio](http://portaudio.com/)  
*  [jack](http://jackaudio.org/)  
*  [alsa](http://alsa-project.org)

there is also:
*  *silent* driver, which makes no sound
*  *dummy* driver, runs as fast as possible, with no sound
*  *file* driver, as fast as possible, writes to a file, either wav or spa, depending on *-r* switch
try
 
```sh
./gwion --help
```

to learn more

simple example code:

```cpp
// print hello world
<<<"Hello World">>>;
// create a sinusoidal generator and connect it to dac  
SinOsc s => dac;  
// let one minute pass  
5::second => now;  
// you're done!
```

there are a few *new* features:  	
	
*  variadic funcs
*  templates (func only for now)
*  function pointers
*  enums

#### requires

* [soundpipe](https://github.com/PaulBatchelor/Soundpipe)
	(the [dev](https://github.com/PaulBatchelor/Soundpipe/tree/dev) branch

#### configure

simply edit *config.mk* to suit your needs, 

#### build

first you need to generate Makefile:

```
./configure
```
try

```
./configure --help
```
to learn more about options.

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
You can also use the [Wiki](https://github.com/fennecdjay/Gwion/wiki).

---------

let me know...
