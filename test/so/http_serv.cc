// http server support both ipv4 and ipv6
//
// build:
//   xmake -b http_serv
//
// start http server:
//   xmake r http_serv                          # 0.0.0.0:80
//   xmake r http_serv ip=127.0.0.1 port=7777   # 127.0.0.1:7777
//   xmake r http_serv ip=::                    # :::80  (ipv6)
//   
// special notes:
//   For ipv6 link-local address, we have to specify the network interface:
//   xmake r http_serv ip=fe80::a00:27ff:fea7:a888%eth0

#include "co/so.h"
#include "co/flag.h"
#include "co/log.h"
#include "co/time.h"

DEF_string(ip, "0.0.0.0", "http server ip");
DEF_int32(port, 80, "http server port");

int main(int argc, char** argv) {
    flag::init(argc, argv);
    log::init();

    http::Server serv(FLG_ip.c_str(), FLG_port);

    serv.on_req(
        [](const http::Req& req, http::Res& res) {
            if (req.is_method_get()) {
                if (req.url() == "/hello") {
                    res.set_status(200);
                    res.set_body("hello world");
                } else {
                    res.set_status(404);
                }
            } else {
                res.set_status(501);
            }
        }
    );

    serv.start();

    while (true) sleep::sec(1024);
    return 0;
}
