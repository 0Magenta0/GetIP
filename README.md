# GetIP 3.0.0

Simple utility to request information about IP written in C

### GetIP supports this APIs
 - [ip-api.com](https://ip-api.com) (Default)
 - [ipapi.co](https://ipapi.co)
 - [extreme-ip-lookup.com](https://extreme-ip-lookup.com)
 - [db-ip.com](https://db-ip.com)
 - [ipgeolocation.io](https://ipgeolocation.io)
 - [ipwhois.io](https://ipwhois.io/)

### Dependencies
 - [libcurl](https://curl.se/libcurl)
 - [json-c](https://github.com/json-c/json-c)
 - [libmaxminddb](https://github.com/maxmind/libmaxminddb)

### Manual build
```
$ git clone https://github.com/0Magenta0/GetIP
$ cd GetIP/build
$ ./builder.sh R
```

### Usage
GetIP help message
```
GetIP (version 3.0.0)
Utility to request info about IP from APIs or MMDB
https://github.com/0Magenta0/GetIP

Usage: getip [[targets][-]] [options]
 or:   getip <targets> -mmdb <path> [options]

  -help            Print this message and exit
  -verbose         Debug output to stderr
  -api <api-id>    Select API
  -mmdb <path>     Use MMDB file instead of API
  -api-list        List of supported apis
  -fields-list     List of supported fields by selected API or MMDB
  -agent <ua>      Custom User-Agent header
  -api-key <key>   Set API key
  -raw             Print response in raw format
```

GetIP api-list message
```
GetIP (version 3.0.0)
Utility to request info about IP from APIs or MMDB
https://github.com/0Magenta0/GetIP

Legend:
+--- (-) Can't TARGET (/) Can TARGET (+) Req TARGET
|+-- (-) Can't APIKEY (/) Can APIKEY (+) Req APKEY
||+- (-/+) Some fields require the APIKEY
***

List of supported apis:
  IP_API_COM (/--)(Default)
  IPAPI_CO (//+)
  EXTREME_IP (//-)
  DB_IP (//+)
  IPGEO_IO (/++)
  WHOIS (//+)
```

GetIP fields-list message (IP\_API\_COM)
```
GetIP (version 3.0.0)
Utility to request info about IP from APIs or MMDB
https://github.com/0Magenta0/GetIP

Usage: getip [[targets][-]] [options]
 or:   getip <targets> -mmdb <path> [options]

List of supported fields by current API:
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
  -is-proxy           Print Proxy field
  -is-mobile          Print Mobile field
```
