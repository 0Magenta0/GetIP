# GetIP 1.2.0

Simple utility to get information about IP written in C

[![Language](https://img.shields.io/badge/Language-C-blue)](https://en.wikipedia.org/wiki/C%20%28programming%20language%29)
[![Dependency](https://img.shields.io/badge/Dependency-json--c-blue)](https://github.com/json-c/json-c)
[![Release](https://img.shields.io/badge/Latest%20Release%20Version-1.1.5-green)](https://github.com/0Magenta0/GetIP/releases/tag/1.2.0)
[![Gnu89Support](https://img.shields.io/badge/Gnu89%20Standart%20Support-Yes-green)](https://gcc.gnu.org/onlinedocs/gcc-3.3.6/gcc/Standards.html)

GetIP uses this [API](https://ip-api.com)

# Automatic Installation

- To install GetIP just execute it!
```
# ./install.sh install
```

- If you want to remove GetIP you can use this command!
```
# ./install.sh uninstall
```

# Manual Compilation

- For compilation you need [json-c](https://github.com/json-c/json-c) library
```
$ gcc *.c -ljson-c -o getip
```

# Usage

GetIP help message
```
Usage: getip <args>

  -h --help                     Print this message
  -e <str> --addr=<str>         Use another's IP
  -i --print-ip                 Print IP parameter
  -o --print-org                Print ORG parameter
  -n --print-hostname           Print Hostname parameter
  -a --print-as                 Print AS parameter
  -A --print-asname             Print AS Name parameter
  -I --print-isp                Print ISP parameter
  -C --print-continent          Print Continent parameter
  -c --print-country            Print Country parameter
  -r --print-region             Print Region parameter
  -t --print-city               Print City parameter
  -z --print-timezone           Print TimeZone parameter
  -H --print-hosting            Print Hosting parameter
  -p --print-proxy              Print Proxy parameter
  -m --print-mobile             Print Mobile parameter

Version: 1.2.0
Author: _Magenta_
```
