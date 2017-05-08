# Configuring Gwion


## float precision

Gwion can be built using either `float`or `double` as floating point type size.  
By default, precision is set to `float` type.  
You can change this behavior by passing `--double` to `configure`.  

(**Note**: `--no-double` also sets precision to `float` type.)


## Drivers

> for examples, we use `xxx` as the driver name.

drivers can be enabled using `--xxx` as argument.  
Similarly, they can be disabled with `--no-xxx`  

You can also set library path: `--xxx_lib=/path/to/driver/library`  
Set driver include path using `--xxx_inc=/path/to/driver/include/dir`

### Driver list


| driver name | default |
|-------------|---------|
| spa         | on      |
| sndfile     | on      |
| alsa        | on      |
| jack        | off     |
| portaudio   | off     |
| soundio     | off     |

there are also two mandatory drivers, which output no sound :astonished:
  * `dummy`  : computes as fast as possible.
  * `silent` : tries to run for as long as it would if run in realtime.

#### IMPORTANT
You should set default driver, otherwise Gwion will use `dummy`.
To do so, use the `--d_func` flag, e.g.: `--d_func=alsa` to use alsa driver by default.

## Common options

  * program name: `--prog=xx`    (default *Gwion*)
  * compiler    : `--cc=xxx`     (default *gcc*, but *clang* is also known to work)
  * parser      : `--yacc=xxx`   (default *yacc*)
  * lexer       : `--lex=xxx`    (default *lex*)
  * prefix      : `--prefix=xxx` (default */usr/local*)


## Instrumentation options
You can compile with debug flags (`-g`) using: `--memcheck`  
Also you can add coverage instrumentation to the code with `--coverage`


## Directories

Gwion uses a few directories at runtime
These options **need** argument, e.g.: `--xxx=dir_name`

  *  the place of plugins: `add`

All directories will be prefixed with `$PREFIX/lib/Gwion/`

## Debug flags

This might concerns only people interrested in develloping Gwion.  
You can get Gwion to output useful (in debugging proccess) messages passing it `--debug-xxx`  
All debug flags are off by default.

### Debug list

  * compile
  * operator
  * scan0
  * scan1
  * scan2
  * type (type checking phase)
  * emit
  * vm
  * instr (vm instruction) 
  * shreduler (shred ordering)
  * stack

-----------------

# NOTE

Except `--(no-)double`, everything can be set when running make, using environment variables.  

Drivers can be set on using, e.g. for *alsa* : `ALSA_D=1` or `ALSA_D=on`.  
In the same way, then can be disabled with     `ALSA_D=0` or `ALSA_D=off`

> Just capitalize the options name

**NOTE**: to set instrumentation flags at make time, capitalize those and prefix them with "USE_", e.g.: `USE_COVERAGE=1` or `USE_COVERAGE=on`
