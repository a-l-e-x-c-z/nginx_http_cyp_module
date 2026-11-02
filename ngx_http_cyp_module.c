#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include "html_head.h"
#include "html_foot.h"
#include "html_gpg.h"

// location lat, lon
static double my_lat = 49.7775922;
static double my_lon = 18.4483944;

static ngx_int_t cyp_handler(ngx_http_request_t *r);
static char *cyp_enable(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static ngx_command_t cyp_commands[] = {
    { ngx_string("cyp"), NGX_HTTP_SRV_CONF | NGX_CONF_NOARGS,
      cyp_enable, 0, 0, NULL },
    ngx_null_command
};

static ngx_http_module_t cyp_context = {
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

ngx_module_t ngx_http_cyp_module = {
    NGX_MODULE_V1,
    &cyp_context,
    cyp_commands,
    NGX_HTTP_MODULE,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL,
    NGX_MODULE_V1_PADDING
};

static ngx_int_t cyp_send(ngx_http_request_t *r, const char *html)
{
    size_t len = ngx_strlen(html);
    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_length_n = (off_t)len;
    ngx_str_set(&r->headers_out.content_type, "text/html");

    ngx_int_t rc = ngx_http_send_header(r);
    if (rc == NGX_ERROR || rc > NGX_OK || r->header_only)
        return rc;

    ngx_buf_t *b = ngx_calloc_buf(r->pool);
    if (b == NULL)
        return NGX_HTTP_INTERNAL_SERVER_ERROR;

    b->pos = (u_char *)html;
    b->last = (u_char *)html + len;
    b->memory = 1;
    b->last_buf = (r == r->main);

    ngx_chain_t out = { .buf = b, .next = NULL };
    return ngx_http_output_filter(r, &out);
}

// parse args to location lat, lon
static double parse_coordinate(ngx_str_t *arg)
{
    ngx_int_t whole = ngx_atoi(arg->data, 2);
    ngx_int_t frac  = ngx_atoi(arg->data + 3, 6);

    return (double)whole + (double)frac / 1000000.0;
}


static int parse_lat_lon(ngx_http_request_t* r, double* lat, double *lon)
{	
  ngx_str_t arg;

  if (ngx_http_arg(r, (u_char *)"lat", 3, &arg) != NGX_OK)
    return NGX_HTTP_BAD_REQUEST;

  *lat = parse_coordinate(&arg);

  if (ngx_http_arg(r, (u_char *)"lon", 3, &arg) != NGX_OK)
    return NGX_HTTP_BAD_REQUEST;

  *lon = parse_coordinate(&arg);

  ngx_log_error(NGX_LOG_INFO, r->connection->log, 0, "CYP parse lat=%.6f lon=%.6f", *lat, *lon);

  return NGX_HTTP_OK;
}

// the locator page
static ngx_int_t page_locator(ngx_http_request_t *r)
{
    if (r->args.data)
    {
       parse_lat_lon(r, &my_lat, &my_lon);

       ngx_log_error(NGX_LOG_INFO, r->connection->log, 0, "CYP locator changed lat/lon 1: %2.6f  %2.6f", my_lat, my_lon);
    } 

    ngx_log_error(NGX_LOG_INFO, r->connection->log, 0, "CYP locator no parameter changed");
    return cyp_send(r, "OK");
}

// the index page
static ngx_int_t page_index(ngx_http_request_t *r)
{
    //ngx_log_error(NGX_LOG_INFO, r->connection->log, 0, "CYP index: URI=%V, args=%V", &r->uri, &r->args);


    char buffer[4000] = {'\0'};

    //ngx_log_error(NGX_LOG_INFO, r->connection->log, 0, "CYP strcmp: %V %s %d", &r->args, "tech", ngx_strncmp(r->args.data, "tech", 4));
    
    // tech
    if (r->args.data && ngx_strncmp(r->args.data, "tech", 4) == 0)
    {
        ngx_log_error(NGX_LOG_INFO, r->connection->log, 0, "CYP page_index = tech");

        char html_body_tech[] = "<section id='alex'><h2>+-- TECH / DEV --+</h2>\n"
	          "C, C++, Golang, Python<br>\n"
                  "Linux, Plan9<br>\n"
                  "GCC, GDB, Valgrind, CMake<br>\n"
                  "Nginx, Postgres+PostGIS<br>\n"
                  "K3S, AWS, Azure<br>\n"
                  "OpenAI, Claude, Github Copilot, Llama.cpp, OpenClaw<br>\n"
                  "</section>\n </main>\n <aside>\n"
      		  "<img class='portrait' src='img/a-l-e-x-brutus.png'>\n"
                  "</aside>\n </div>\n";
    
        ngx_sprintf((u_char*)buffer, "%s%s%s\n", html_head, html_body_tech, html_foot);
        return cyp_send(r, buffer);
    }
    
    // gpg
    if (r->args.data && ngx_strncmp(r->args.data, "gpg", 3) == 0) 
    {
      
	ngx_log_error(NGX_LOG_INFO, r->connection->log, 0, "CYP page_index = gpg");
    
        char html_body_gpg[] = "<section id='alex'><h2>+-- GPG public key --+</h2>\n";

	char html_body_gpg2[] = "</section>\n </main>\n <aside>\n"
      		  "<img class='portrait' src='img/a-l-e-x-radio.png'>\n"
                  "</aside>\n </div>\n";

        ngx_sprintf((u_char*)buffer, "%s%s%s%s%s\n", html_head, html_body_gpg, html_gpg, html_body_gpg2, html_foot);
        return cyp_send(r, buffer);
    }

    // links
    if (r->args.data && ngx_strncmp(r->args.data, "links", 5) == 0) 
    {
      
	ngx_log_error(NGX_LOG_INFO, r->connection->log, 0, "CYP page_index = links");
	ngx_log_error(NGX_LOG_INFO, r->connection->log, 0, "CYP page_index = links, lat=%2.6f lon=%2.6f", my_lat, my_lon);
    
        char html_body_links[] = "<section id='alex'><h2>+-- LINKS --+</h2>\n"
		  "<a href='https://www.root.cz'>root.cz</a><br>\n"
	          "<a href='https://phrack.org'>phrack</a><br>\n"
	          "<a href='https://9front.org'>9front (plan9)</a><br>\n"
	          "<a href='https://www.chinamieville.net'>china mieville</a><br>\n"
	          "<a href='https://www.hplovecraft.com'>H.P.L.</a><br>\n"
                  "</section>\n </main>\n <aside>\n"
      		  "<img class='portrait' src='img/a-l-e-x-beer.png'>\n"
                  "</aside>\n </div>\n";

        ngx_sprintf((u_char*)buffer, "%s%s%s\n", html_head, html_body_links, html_foot);
        return cyp_send(r, buffer);
    }
    
    // location
    if (r->args.data && ngx_strncmp(r->args.data, "location", 8) == 0) 
    {
      
	ngx_log_error(NGX_LOG_INFO, r->connection->log, 0, "CYP page_index = location, %2.6f %2.6f", my_lat, my_lon);
    
        char html_loc1[] = "<section id='alex'><h2>+-- MY LOCATION --+</h2>\n<img src='https://api.mapy.com/v1/static/map?lon=";
        char html_loc2[] = "&lat=";
	char html_loc3[] = "&zoom=16&width=600&height=400&mapset=outdoor&markers=color:red;size:normal;";
	char html_loc4[] = "&apikey=QDwAJBncxaRx36m6IRC_I36ywiyNSTHsCr6TFr6Tqrw'>\n</section>\n </main>\n <aside>\n<img class='portrait' src='img/a-l-e-x-map.png'>\n</aside>\n </div>\n";

        ngx_sprintf((u_char*)buffer, "%s%s%2.6f%s%2.6f%s%2.6f,%2.6f%s%s\n", html_head, html_loc1, my_lon, html_loc2, my_lat, html_loc3, my_lon, my_lat, html_loc4, html_foot);
        return cyp_send(r, buffer);
    }

   
    // index, alex  
    //ngx_log_error(NGX_LOG_INFO, r->connection->log, 0, "CYP page_index = alex, argslen=%d", ngx_strlen(r->args.data));

    char html_body_alex[] = "<section id='alex'><h2>+-- A-L-E-X --+</h2>\n"
	          "<p>Hello, I am Alex. <br>\n"
	          "Running Nginx with my ngx_http_cyp_module generating the whole web.\n"
		  "No HTML, CSS, Javascript files, no PHP, no NodeJS, no backend, only C module inside Nginx :-)</p>\n"
                  "</section>\n </main>\n <aside>\n"
      		  "<img class='portrait' src='img/a-l-e-x.png'>\n"
                  "</aside>\n </div>\n";
    
    ngx_sprintf((u_char*)buffer, "%s%s%s\n", html_head, html_body_alex, html_foot);

    return cyp_send(r, buffer);
}

typedef ngx_int_t (*cyp_handler_t)(ngx_http_request_t *);
typedef struct {
    ngx_uint_t method;
    ngx_str_t path;
    cyp_handler_t handler;
} cyp_route_t;

// router to index page, handler function
static cyp_route_t routes[] = {
    { NGX_HTTP_GET | NGX_HTTP_HEAD, ngx_string("/"), page_index },
    { NGX_HTTP_GET | NGX_HTTP_HEAD, ngx_string("/locator"), page_locator }
};

static ngx_int_t cyp_handler(ngx_http_request_t *r)
{
    ngx_uint_t allowed = 0;
    for (size_t i = 0; i < sizeof(routes) / sizeof(routes[0]); ++i) {
        if (r->uri.len == routes[i].path.len &&
            ngx_memcmp(r->uri.data, routes[i].path.data, r->uri.len) == 0) {
            allowed |= routes[i].method;
            if (r->method & routes[i].method)
                return routes[i].handler(r);
        }
    }
    return allowed ? NGX_HTTP_NOT_ALLOWED : NGX_HTTP_NOT_FOUND;
}

static char *cyp_enable(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t *clcf =
        ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    if (clcf->handler != NULL)
        return "is duplicate or conflicts with another content handler";
    clcf->handler = cyp_handler;
    return NGX_CONF_OK;
}
