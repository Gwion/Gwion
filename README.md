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

there are a few *new* features:  	
    * variadic funcs
    * templates (func only for now)
    * function pointers
    * enums

####requires####
* soundpipe (compiled to use double, you can use [my fork](https://github.com/fennecdjay/Soundpipe))
* libsoundio

####configure####
simply *config.mk* to suit your needs, 

####build####
```
make
```


let me know...
