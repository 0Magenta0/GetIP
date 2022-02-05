# GetIP 1.1.1

Simple utility to get information about IP written in C

# Manual Compilation

- For compilation you need [json-c](https://github.com/json-c/json-c) library
```
gcc *.c -ljson-c -o getip
```

# Usage

GetIP help message
```
Usage: getip <args>

  -h            Print this message
  -e <str>      Use another's IP
  -4            Print IP parameter
  -o            Print ORG parameter
  -n            Print Hostname parameter
  -a            Print AS parameter
  -A            Print AS Name parameter
  -i            Print ISP parameter
  -C            Print Continent parameter
  -c            Print Country parameter
  -r            Print Region parameter
  -t            Print City parameter
  -z            Print TimeZone parameter
  -H            Print Hosting parameter
  -p            Print Proxy parameter
  -m            Print Mobile parameter

Version: 1.1.1
Author: _Magenta_
```
