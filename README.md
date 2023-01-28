# GetIP 2.2.2

Simple utility to get information about IP written in C

### GetIP supports this APIs
 - [ip-api.com](https://ip-api.com) (Default)
 - [ipapi.co](https://ipapi.co)

### Dependencies
 - [libcurl](https://curl.se/libcurl)
 - [json-c](https://github.com/json-c/json-c)

### Manual build

```
$ git clone https://github.com/0Magenta0/GetIP
$ cd GetIP/build
$ ./builder.sh R
```

### Usage

GetIP help message
```
GetIP (version 2.2.2)
Utility to request info about IP from APIs
https://github.com/0Magenta0/GetIP

Usage: getip [target] [options]
 or:   getip [options]

  -help            Print this message and exit
  -verbose         Debug output to stderr
  -api <api-id>    Select API
  -api-list        List of supported apis
  -fields-list     List of supported fields by selected API
  -agent <ua>      Custom User-Agent header
```

GetIP api-list message
```
GetIP (version 2.2.2)
Utility to request info about IP from APIs
https://github.com/0Magenta0/GetIP

List of supported apis:
  IP_API_COM (Default)
  IPAPI_CO
```

GetIP fields-list message (IP\_API\_COM)
```
GetIP (version 2.2.2)
Utility to request info about IP from APIs
https://github.com/0Magenta0/GetIP

Usage: getip [target] [options]
 or:   getip [options]

List of supported field by current API:
  -ip                 Print IP field
  -org                Print ORG field
  -host               Print Hostname field
  -as                 Print AS field
  -asname             Print AS Name field
  -isp                Print ISP field
  -continent          Print Continent field
  -country            Print Country field
  -region             Print Region field
  -city               Print City field
  -zone               Print Time Zone field
  -is-host            Print Hosting field
  -is-proxy           Print Time Proxy field
  -is-mobile          Print Time Mobile field
```
