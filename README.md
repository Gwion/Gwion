#Gwion#
![](http://b.repl.ca/v1/version-pre--alpha-red.png)
![](http://b.repl.ca/v1/language-C-green.png)  
gwion is a object-oriented programming language, aimed at making music

**strongly** inspired by [chuck](http://chuck.stanford.edu/)  
synthesis is based on [soundpipe](http://paulbatchelor.github.io/proj/soundpipe.html)  
audio output is done using [libsoundio](http://libsound.io/)  

simple example code:
> // print hello world
>	<<<"Hello World">>>;
> // create a sinusoidal generator and connect it to dac  
> SinOsc s => dac;  
> // let one minute pass  
> 5::second => now;  
> // you're done!

to build, simply edit config.mk to suit your needs, and type make

####requires####
* soundpipe (compiled to use double, you can use [my fork](https://github.com/fennecdjay/Soundpipe))
* libsoundio

let me know...
