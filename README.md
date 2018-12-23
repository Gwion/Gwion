![](https://img.shields.io/badge/language-C-yellowgreen.svg)
[![release](http://github-release-version.herokuapp.com/github/fennecdjay/Gwion/release.svg)](https://github.com/fennecdjay/Gwion/releases/latest)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](http://makeapullrequest.com)


[![Codacy Badge](https://api.codacy.com/project/badge/Grade/7669efe4c1c84b888cea5885201c79ae)](https://www.codacy.com/app/fennecdjay/Gwion?utm_source=github.com&utm_medium=referral&utm_content=fennecdjay/Gwion&utm_campaign=badger)
[![Build Status](https://travis-ci.org/fennecdjay/Gwion.svg?branch=master)](https://travis-ci.org/fennecdjay/Gwion)
[![Coveralls branch](https://img.shields.io/coveralls/fennecdjay/Gwion/master.svg)](https://coveralls.io/github/fennecdjay/Gwion?branch=master)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/fennecdjay/Gwion.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/fennecdjay/Gwion/context:cpp)
[![FOSSA Status](https://app.fossa.io/api/projects/git%2Bgithub.com%2Ffennecdjay%2FGwion.svg?type=shield)](https://app.fossa.io/projects/git%2Bgithub.com%2Ffennecdjay%2FGwion?ref=badge_shield)
[![Supported achievements](http://achievibit.kibibit.io/achievementsShield)](https://achievibit.kibibit.io)
<!--[![Income](https://img.shields.io/liberapay/receives/Liberapay.svg)](https://liberapay.com/Liberapay)-->
[![Donate](https://liberapay.com/assets/widgets/donate.svg)](https://liberapay.com/fennecdjay/donate)
[![Coverity Scan Build Status](https://img.shields.io/coverity/scan/15155.svg?style=flat-square)](https://scan.coverity.com/projects/fennecdjay-gwion)
# Gwion
gwion is a object-oriented programming language, aimed at making music

**strongly** inspired by [chuck](http://chuck.stanford.edu/)  
synthesis is based on [soundpipe](http://paulbatchelor.github.io/proj/soundpipe.html)  
*high-level* features: variadic functions, templates, func pointers.  
:gift: a [plugin system](https://github.com/fennecdjay/Gwion-plug) makes it easily extendable
<!--TODO: link to plugin DOC rather than repo -->

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

:mag: learn more about options: :mag_right:

```
./gwion --help
```

#### requires

* [soundpipe](https://github.com/PaulBatchelor/Soundpipe)

#### build

first you need to configure the build system

```
./configure --help
```
should show a list of available options.

:computer: now run `./configure` with the options you need

then, just
```
make
```

> if you run into troubles, please look [here](https://github.com/fennecdjay/Gwion/blob/master/docs/Building.md) and read on.

#### Reporting bugs / Contributing

:+1: Every helping hand is welcome!!! :+1:  
:book: Please see the [contributing](.github/CONTRIBUTING.md) page for more information.
