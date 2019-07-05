# Lambda

## Overview

Simply put, *lambda*s are anonymous functions.  

The syntax to create them is simple:
```
\ variable0 variable1 ... {  your code here }
```
You can even use it to
### Call a function just once
``` gw
\ i { <<<"passed '", i, "'">>>; }(3);
```
<p style="background-color:#e3e3e3; border: 5px solid #343131; padding: 10px; margin-right: 20%; margin-left: 20%; -moz-border-radius: 15px; -webkit-border-radius: 15px;">
passed '3'<br/>
&#10004;
</p>


## Use case

### Passing to a function pointer
``` gw
typedef void fptr_t(int);
\ i { <<<"passed '", i, "'">>>; } @=> fptr_t fptr;
fptr(4);
```
<p style="background-color:#e3e3e3; border: 5px solid #343131; padding: 10px; margin-right: 20%; margin-left: 20%; -moz-border-radius: 15px; -webkit-border-radius: 15px;">
passed '4'<br/>
&#10004;
</p>

### As Argument to Functions
``` gw
typedef void fptr_t(int);
fun void test(fptr_t fptr) {
   fptr(5);
}
test(\ i { <<<"passed '", i, "'">>>; });
```
<p style="background-color:#e3e3e3; border: 5px solid #343131; padding: 10px; margin-right: 20%; margin-left: 20%; -moz-border-radius: 15px; -webkit-border-radius: 15px;">
passed '5'<br/>
&#10004;
</p>
