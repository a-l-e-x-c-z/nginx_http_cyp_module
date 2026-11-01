# CYP (C-YOUR-PAGE): C-generated HTTP pages inside nginx

A small dynamic nginx HTTP module to generate personal web page. No `location /` directive, no static HTML files,
no FastCGI, nor template engine is needed. No PHP, no NodeJS, only Nginx. The `cyp;` server directive installs
a content handler for the server's implicit location.

## Files
- `ngx_http_cyp_module.c`: router, pages, response helper, nginx module
- `config`: nginx module build integration
- `nginx.conf.example`: minimal configuration

## Build

Obtain the **same nginx source version** as the nginx binary you will load
this module into. Use compatible configure flags (check `nginx -V`).

From the nginx source directory:

```sh
./configure --add-dynamic-module=/absolute/path/to/nginx_http_cyp_module
make modules
```
