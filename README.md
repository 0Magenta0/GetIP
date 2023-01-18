# GetIP 2.0.0

Simple utility to get information about IP written in C

[![Language](https://img.shields.io/badge/Language-C-blue)](https://en.wikipedia.org/wiki/C%20%28programming%20language%29)
[![Release](https://img.shields.io/badge/Latest%20Release%20Version-1.2.1-green)](https://github.com/0Magenta0/GetIP/releases/tag/1.2.1)

GetIP uses this [API](https://ip-api.com)

# Dependencies
 - [libcurl](https://curl.se/libcurl)
 - [json-c](https://github.com/json-c/json-c)

# Project build

```
$ git clone https://github.com/0Magenta0/GetIP
$ cd GetIP/build
$ ./builder.sh R
```

# Usage

GetIP help message
```
Usage: getip [args]

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

Version: 1.2.1
Author: _Magenta_
```
