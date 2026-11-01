# CYP (C-YOUR-PAGE), C-generated pages inside Nginx

A small nginx module to generate personal web page. No `location /` directive, no static HTML files,
no FastCGI, nor template engine is needed. No PHP, no NodeJS, no backend, only Nginx. The `cyp;` http/server directive installs
a content handler for the server's implicit location.

## Files
- `ngx_http_cyp_module.c`: router, pages, response helper, nginx module
- `config`: nginx module build integration
- `nginx.conf.example`: minimal configuration

## Build

From the nginx source directory:

```sh
./auto/configure --with-http_ssl_module --with-http_v2_module --with-http_realip_module --with-threads --with-compat --add-dynamic-module=/home/alex/develop/nginx_http_cyp_module/

make
```
