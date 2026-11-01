#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include "html_head.h"
#include "html_foot.h"
#include "html_gpg.h"

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

        char html_body_tech[] = "<nav class='box' aria-label='Main navigation'>\n"
	          "<a href='?alex'>[ a-l-e-x ]</a><a href='?tech'>[ tech stack ]</a>\n"
                  "<a href='?gpg'>[ GPG key ]</a><a href='?links'>[ Links ]</a><a href='html/rebtris.html'>[ Rebtris ]</a>\n"
                  "</nav>\n"
                  "<div class='grid'>\n"
                  "<main class='box'>\n"
                  "<section id='alex'><h2>+-- TECH / DEV --+</h2>\n"
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
    
        char html_body_gpg[] = "<nav class='box' aria-label='Main navigation'>\n"
	          "<a href='?alex'>[ a-l-e-x ]</a><a href='?tech'>[ tech stack ]</a>\n"
                  "<a href='?gpg'>[ GPG key ]</a><a href='?links'>[ Links ]</a><a href='html/rebtris.html'>[ Rebtris ]</a>\n"
                  "</nav>\n"
                  "<div class='grid'>\n"
                  "<main class='box'>\n"
                  "<section id='alex'><h2>+-- GPG public key --+</h2>\n";

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
    
        char html_body_links[] = "<nav class='box' aria-label='Main navigation'>\n"
	          "<a href='?alex'>[ a-l-e-x ]</a><a href='?tech'>[ tech stack ]</a>\n"
                  "<a href='?gpg'>[ GPG key ]</a><a href='?links'>[ Links ]</a><a href='html/rebtris.html'>[ Rebtris ]</a>\n"
                  "</nav>\n"
                  "<div class='grid'>\n"
                  "<main class='box'>\n"
                  "<section id='alex'><h2>+-- LINKS --+</h2>\n"
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

   
    // index, alex  
    //ngx_log_error(NGX_LOG_INFO, r->connection->log, 0, "CYP page_index = alex, argslen=%d", ngx_strlen(r->args.data));

    char html_body_alex[] = "<nav class='box' aria-label='Main navigation'>\n"
	          "<a href='?alex'>[ a-l-e-x ]</a><a href='?tech'>[ tech stack ]</a>\n"
                  "<a href='?gpg'>[ GPG key ]</a><a href='?links'>[ Links ]</a><a href='html/rebtris.html'>[ Rebtris ]</a>\n"
                  "</nav>\n"
                  "<div class='grid'>\n"
                  "<main class='box'>\n"
                  "<section id='alex'><h2>+-- A-L-E-X --+</h2>\n"
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
    { NGX_HTTP_GET | NGX_HTTP_HEAD, ngx_string("/"), page_index }
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
