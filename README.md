# Gwion

[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/Gwion/Gwion.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/fennecdjay/Gwion/context:cpp)
[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/2417/badge)](https://bestpractices.coreinfrastructure.org/projects/2417)
[![Coverage](https://raw.githubusercontent.com/Gwion/gwion-coverage-report/master/badge.svg?sanitize=true)](https://fennecdjay.github.io/gwion-coverage-report/master)
[![Line Count](https://tokei.rs/b1/github/Gwion/Gwion)](https://github.com/fennecdjay/Gwion)
<!-- ALL-CONTRIBUTORS-BADGE:START - Do not remove or modify this section -->
[![All Contributors](https://img.shields.io/badge/all_contributors-16-orange.svg)](#contributors)
<!-- ALL-CONTRIBUTORS-BADGE:END --> 
![Linux](https://github.com/Gwion/Gwion/workflows/Linux/badge.svg)
![MacOs](https://github.com/Gwion/Gwion/workflows/MacOs/badge.svg)
![Windows](https://github.com/Gwion/Gwion/workflows/Windows/badge.svg)

Gwion is a programming language, aimed at making music



<img src=".img/gwion_bear-transparent-up.webp" 
     height="500" 
     alt = "Bran the musical wizard grizzly bear casting spell with a wand" /> < Bran, our mascot, kindly provided by [neverRare](https://github.com/neverRare)  


**strongly** inspired by [ChucK](http://chuck.stanford.edu/), but adding a bunch *high-level* features;
	  templating, first-class functions and more.  

It aims to be simple, small,
 [fast](https://Gwion.github.io/Gwion/#Benchmarks/),
 [extendable](https://github.com/Gwion/Gwion-plug) and [embeddable](https://github.com/fennecdjay/Gwion/blob/master/src/main.c#L18-L31).

Checkout the community, and join us on discord: https://discord.gg/KmHqbTKSmS.
You can also check this very nice server about programming language devlopment: https://discord.gg/4Kjt3ZE,
since a lot of improvements to gwion are discussed there.

Please also consider taking the (WIP) [*tour*](https://github.com/Gwion/GwionTour) 

## Build
### Download the source

``` sh
git clone --recursive https://github.com/Gwion/Gwion
cd Gwion
make
```

### Configuring (optional)
You can get a list of config files to tweak with
``` sh
find . -name "config.mk"
```

Please rebuild to take your change into account.
```
make -C util clean
make -C ast  clean
make clean
make
```

> Besides developer options, you migth want to check *USE_DOUBLE*, in util/config.mk, which set the floating point size (float or double).
> Note that the option you choose must match how you built your soundpipe library (more on soundpipe later). 

## Executing your first code (hello_world.gw):

This assumes you have successfully compiled gwion. To build follow [these steps](#build) . To check, if the build was successfully run
```
./gwion
```
if this gives out some error, there were problems with your compilation.
Try building again, and open a [issue](https://github.com/Gwion/Gwion/issues)
if the problem persists. We would love to help you out.  
If you see no errors, Congratulations!! You have successfully compiled gwion, and can move ahead.

Create a new file "hello_world.gw" in the same directory.(You are free to use any command)
```
touch hello_world.gw
```

Open this file using your favourite text editor
```
vim hello_world.gw
```

Add the following lines to print "Hello World" (Note the semicolon at the end)
```
<<< "Hello World" >>>;
```

Save and exit the file(:wq in vim). Use the following command to run your first gwion program
```sh
./gwion hello_world.gw
```
Congratulations!! You ran your first gwion program.

## Making Sound

_This section is currently very Linux-centric. We are working to improve that. Pull requests welcome!_

Gwion relies on plugins for most of its language features, including all those that make sound. Plugins are located in the subdirectories of
`plug`. To get some sounds going under linux using jack sound server, you can build the plugins `Jack`, `Soundpipe`, and `Modules`. Make sure you have Jack installed.

Start by downloading the sources
```
git submodule update plug
cd plug
```

Starting from the Gwion base directory, to build `Jack`:
```
cd plug/Jack
make
```
This will give you a shared object file, `Jack.so`. The default place Gwion will look for plugins is in a subdir of your home directory 
named `.gwplug`. So create that directory and move `Jack.so` there:
```
mkdir ~/.gwplug
mv Jack.so ~/.gwplug
```
Repeat for the other plugins mentioned. The `Soundpipe` plugin requires the [gwion-soundpipe](https://github.com/Gwion/gwion-soundpipe) library, which we hope to have build seamlessly for you when you build the `Soundpipe` module, but we're not quite there yet. For now, please clone gwion-soundpipe in the Soundpipe plugin directory `plug/Soundpipe`. Please ask for help if this isn't working.

When all those plugin `.so` files are in your `~/.gwplug` directory, you should be able to run a Gwion program that makes sound! In `plug/Modules` there's a `test.gw` program which plays a sine wave for 5 seconds. If the `gwion` you built is still in the base dir of your cloned repo, from the
`plug/Modules` subdirectory you should be able to run `../../gwion -d Jack test.gw` and hear some sound!

## Installation

It's a good idea to install Gwion now that you have tried it out. Use the following command to install it
```
sudo make install
```

To confirm Gwion can make sound, update `hello_world.gw` to:

```
#import Modules

<<<"Sound from Gwion!">>>;

new SinOsc ~> dac;

5::second => now;
```

To run it with Jack as the driver: `gwion hello_world.gw -dJack`. Hopefully you will hear a smooth sine wave. If not, please reach out on Discord and we'll help you out.

## Documentation

Do you want to know more? :smile: Look [here](https://Gwion.github.io/Gwion/)
Both outdated and WIP :construction_worker: but a nice place to learn and [contribute](https://github.com/Gwion/gwion/issues)


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
<!-- prettier-ignore-start -->
<!-- markdownlint-disable -->
<table>
  <tr>
    <td align="center"><a href="http://paulbatchelor.github.io"><img src="https://avatars3.githubusercontent.com/u/8139389?v=4" width="100px;" alt=""/><br /><sub><b>Paul Batchelor</b></sub></a><br /><a href="#question-PaulBatchelor" title="Answering Questions">💬</a> <a href="https://github.com/Gwion/Gwion/issues?q=author%3APaulBatchelor" title="Bug reports">🐛</a> <a href="https://github.com/fennecdjay/Gwion/commits?author=PaulBatchelor" title="Code">💻</a> <a href="#ideas-PaulBatchelor" title="Ideas, Planning, & Feedback">🤔</a></td>
    <td align="center"><a href="https://github.com/originalsouth"><img src="https://avatars1.githubusercontent.com/u/5300799?v=4" width="100px;" alt=""/><br /><sub><b>Benny</b></sub></a><br /><a href="#question-originalsouth" title="Answering Questions">💬</a> <a href="https://github.com/Gwion/Gwion/issues?q=author%3Aoriginalsouth" title="Bug reports">🐛</a> <a href="https://github.com/fennecdjay/Gwion/commits?author=originalsouth" title="Code">💻</a></td>
    <td align="center"><a href="https://github.com/scalarwaves"><img src="https://avatars1.githubusercontent.com/u/4212896?v=4" width="100px;" alt=""/><br /><sub><b>Andrew Prentice</b></sub></a><br /><a href="#question-scalarwaves" title="Answering Questions">💬</a> <a href="https://github.com/Gwion/Gwion/issues?q=author%3Ascalarwaves" title="Bug reports">🐛</a> <a href="https://github.com/fennecdjay/Gwion/commits?author=scalarwaves" title="Code">💻</a> <a href="#ideas-scalarwaves" title="Ideas, Planning, & Feedback">🤔</a></td>
    <td align="center"><a href="https://github.com/Aleserche"><img src="https://avatars3.githubusercontent.com/u/2920837?v=4" width="100px;" alt=""/><br /><sub><b>aleserche</b></sub></a><br /><a href="https://github.com/Gwion/Gwion/commits?author=Aleserche" title="Code">💻</a></td>
    <td align="center"><a href="https://github.com/Pranav2612000"><img src="https://avatars3.githubusercontent.com/u/20909078?v=4" width="100px;" alt=""/><br /><sub><b>Pranav Joglekar</b></sub></a><br /><a href="#userTesting-Pranav2612000" title="User Testing">📓</a> <a href="https://github.com/Gwion/Gwion/commits?author=Pranav2612000" title="Documentation">📖</a></td>
    <td align="center"><a href="http://dev.to/amberisvibin"><img src="https://avatars3.githubusercontent.com/u/63863236?v=4" width="100px;" alt=""/><br /><sub><b>Amber</b></sub></a><br /><a href="#question-amberisvibin" title="Answering Questions">💬</a> <a href="https://github.com/Gwion/Gwion/commits?author=amberisvibin" title="Documentation">📖</a> <a href="#ideas-amberisvibin" title="Ideas, Planning, & Feedback">🤔</a></td>
    <td align="center"><a href="https://github.com/TotallyNotChase"><img src="https://avatars0.githubusercontent.com/u/44284917?v=4" width="100px;" alt=""/><br /><sub><b>Chase</b></sub></a><br /><a href="https://github.com/Gwion/Gwion/commits?author=TotallyNotChase" title="Code">💻</a></td>
  </tr>
  <tr>
    <td align="center"><a href="https://github.com/nithin-pankaj"><img src="https://avatars2.githubusercontent.com/u/15152472?v=4" width="100px;" alt=""/><br /><sub><b>Nithin Pankaj</b></sub></a><br /><a href="https://github.com/fennecdjay/Gwion/commits?author=nithin-pankaj" title="Code">💻</a></td>
    <td align="center"><a href="https://github.com/evayde"><img src="https://avatars1.githubusercontent.com/u/25255815?v=4" width="100px;" alt=""/><br /><sub><b>Enrico Gruner</b></sub></a><br /><a href="https://github.com/fennecdjay/Gwion/commits?author=evayde" title="Code">💻</a></td>
    <td align="center"><a href="https://github.com/umer2001"><img src="https://avatars2.githubusercontent.com/u/35496058?v=4" width="100px;" alt=""/><br /><sub><b>Muhammad Umer Farooq</b></sub></a><br /><a href="https://github.com/fennecdjay/Gwion/commits?author=umer2001" title="Code">💻</a></td>
    <td align="center"><a href="https://github.com/a-mere-peasant"><img src="https://avatars3.githubusercontent.com/u/50142453?s=400&u=4844f21d4f48431d854307ecdf4d1f4d80977ab9&v=4" width="100px;" alt=""/><br /><sub><b>Harsh Jain</b></sub></a><br /><a href="https://github.com/Gwion/Gwion/commits?author=a-mere-peasant" title="Documentation">📖</a></td>
    <td align="center"><a href="https://github.com/ry-v1"><img src="https://avatars1.githubusercontent.com/u/72290009?v=4" width="100px;" alt=""/><br /><sub><b>ry-v1</b></sub></a><br /><a href="https://github.com/fennecdjay/Gwion/commits?author=ry-v1" title="Documentation">📖</a></td>
    <td align="center"><a href="https://github.com/deekts"><img src="https://avatars0.githubusercontent.com/u/51462833?v=4" width="100px;" alt=""/><br /><sub><b>deekts</b></sub></a><br /><a href="https://github.com/fennecdjay/Gwion/commits?author=deekts" title="Code">💻</a></td>
    <td align="center"><a href="https://github.com/forrcaho"><img src="https://avatars3.githubusercontent.com/u/492742?v=4" width="100px;" alt=""/><br /><sub><b>Forrest Cahoon</b></sub></a><br /><a href="#ideas-forrcaho" title="Ideas, Planning, & Feedback">🤔</a></td>
  </tr>
  <tr>
    <td align="center"><a href="https://github.com/SenorGrande"><img src="https://avatars2.githubusercontent.com/u/22025776?v=4" width="100px;" alt=""/><br /><sub><b>Connor Hewett</b></sub></a><br /><a href="https://github.com/fennecdjay/Gwion/commits?author=SenorGrande" title="Code">💻</a></td>
    <td align="center"><a href="https://github.com/euppal"><img src="https://avatars2.githubusercontent.com/u/67026187?v=4" width="100px;" alt=""/><br /><sub><b>Ethan Uppal</b></sub></a><br /><a href="#ideas-PaulBatchelor" title="Ideas, Planning, & Feedback">🤔</a></td>
  </tr>
</table>

<!-- markdownlint-enable -->
<!-- prettier-ignore-end -->
<!-- ALL-CONTRIBUTORS-LIST:END -->
This project follows the [all-contributors](https://github.com/kentcdodds/all-contributors) specification. Contributions of any kind welcome!

>    NOTE: if you should be on the list of contributors but we forgot you, don't be shy and let us know!
