# Declarations

## Basics

Declaring a primitive or an object is quite straight forward:
``` gw
int i;
Object o;
<<<i, " ", o>>>;
```
0 0x56418af88660

## Declaring a reference
However ...
``` gw
Object @ref;
<<<"Reference points to no object yet: ", ref>>>;
//Object o @=> ref;
new Object @=> ref;
<<<"But now it does: ", ref>>>;
```
make[1] : on entre dans le répertoire « /home/djay/src/git/gwion/gwion-github »
<p style="background-color:#e3e3e3; border: 5px solid #343131; padding: 10px; margin-right: 20%; margin-left: 20%; -moz-border-radius: 15px; -webkit-border-radius: 15px;">
(int[]) (nil)<br/>
(int[]) 0x4dedc10<br/>
t: (nil)<br/>
But now it does: 0x4dedc30<br/>
&#10004;
</p>
make[1] : on quitte le répertoire « /home/djay/src/git/gwion/gwion-github »

## Arrays

### array as refs

``` gw
int ref[];
<<<ref>>>;
new int[2] @=> ref;
<<<ref>>>;
```
make[1] : on entre dans le répertoire « /home/djay/src/git/gwion/gwion-github »
<p style="background-color:#e3e3e3; border: 5px solid #343131; padding: 10px; margin-right: 20%; margin-left: 20%; -moz-border-radius: 15px; -webkit-border-radius: 15px;">
(int[]) (nil)<br/>
(int[]) 0x4dedc10<br/>
t: (nil)<br/>
But now it does: 0x4dedc30<br/>
&#10004;
</p>
make[1] : on quitte le répertoire « /home/djay/src/git/gwion/gwion-github »
