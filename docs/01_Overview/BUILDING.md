# Build / Configure Gwion

## Configure
### gwion-util

  * `USE_MEMCHECK`: compile with debug flags (`-g`) and enable asserts
  * `USE_COVERAGE`: add coverage instrumentation


## Make 
Basically, all that is left to do is
```sh
make
```
The only environment variable affecting the operation is `PREFIX`

Except `--(no-)double`, everything can be set when running make,
using environment variables.  

Drivers can be set on using, e.g. for *alsa* : `ALSA_D=1` or `ALSA_D=on`.  
In the same way, then can be disabled with     `ALSA_D=0` or `ALSA_D=off`

### Running tests
```bash
make tests
```
to run all tests, or 
```bash
bash util/test.sh my_file_or_directory (.. other files/dirs ...)
```
to run specific ones.
look [here](#testing.md) for more.
## Install
*maybe as root*
```sh
make install
```
The only environment variable affecting the operation is `PREFIX`
