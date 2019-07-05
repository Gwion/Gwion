# For Loops
**For** loops in Gwion is pretty similar to classic **C** syntax

## basic loops
``` gw
for(int i; i < 3; ++i)
   <<<i>>>;
```
<p style="background-color:#e3e3e3; border: 5px solid #343131; padding: 10px; margin-right: 20%; margin-left: 20%; -moz-border-radius: 15px; -webkit-border-radius: 15px;">
(int) 0<br/>
(int) 1<br/>
(int) 2<br/>
&#10004;
</p>

Of course, it also works with a block of code.

``` gw
for(int i; i < 3; ++i) {
   i/2 => float f1;
   i/2. => float f2;
   <<<i, " " , f1, " ", f2>>>;
}
```
<p style="background-color:#e3e3e3; border: 5px solid #343131; padding: 10px; margin-right: 20%; margin-left: 20%; -moz-border-radius: 15px; -webkit-border-radius: 15px;">
0 0.0000 0.0000<br/>
1 0.0000 0.5000<br/>
2 1.0000 1.0000<br/>
&#10004;
</p>

## Nested Loops
``` gw
int array[3][4];

for(int i; i < 3; ++i) {
  for(int j; j < 4; ++j) {
    <<<array[i][j]>>>;
  }
}
```
<p style="background-color:#e3e3e3; border: 5px solid #343131; padding: 10px; margin-right: 20%; margin-left: 20%; -moz-border-radius: 15px; -webkit-border-radius: 15px;">
(int) 0<br/>
(int) 0<br/>
(int) 0<br/>
(int) 0<br/>
(int) 0<br/>
(int) 0<br/>
(int) 0<br/>
(int) 0<br/>
(int) 0<br/>
(int) 0<br/>
(int) 0<br/>
(int) 0<br/>
&#10004;
</p>

### Auto Loops

#### Simple auto loop
``` gw
int array[2][3];
for(auto a: array) {
  <<<a>>>;
  for(auto b: a)
    <<<b>>>;
}
```
<p style="background-color:#e3e3e3; border: 5px solid #343131; padding: 10px; margin-right: 20%; margin-left: 20%; -moz-border-radius: 15px; -webkit-border-radius: 15px;">
(int[]) 0x4e0ff00<br/>
(int) 0<br/>
(int) 0<br/>
(int) 0<br/>
(int[]) 0x4e0ff20<br/>
(int) 0<br/>
(int) 0<br/>
(int) 0<br/>
&#10004;
</p>

### Auto Pointer loop
With the simple auto loop, you only get the value in the array.  
If you want to change it, you need a pointer 

``` gw
int array[2][3];
int i;
for(auto a: array) {
  for(auto @b: a)
    <<<++i => *b>>>;
}
for(auto a: array) {
  for(auto @b: a)
    <<<*b>>>;
}
```
<p style="background-color:#e3e3e3; border: 5px solid #343131; padding: 10px; margin-right: 20%; margin-left: 20%; -moz-border-radius: 15px; -webkit-border-radius: 15px;">
(int) 1<br/>
(int) 2<br/>
(int) 3<br/>
(int) 4<br/>
(int) 5<br/>
(int) 6<br/>
(int) 1<br/>
(int) 2<br/>
(int) 3<br/>
(int) 4<br/>
(int) 5<br/>
(int) 6<br/>
&#10004;
</p>
