#ifndef H_MYECHO_SERVER_H
#define H_MYECHO_SERVER_H

#include <msgpack/rpc/server.h>
#include <tuple>

class myecho : public msgpack::rpc::dispatcher {
public:
    typedef msgpack::rpc::request request;

	void dispatch(request req)
    {
        try {
            std::string method;
            req.method().convert(&method);

            if (method == "add") {
                std::tuple<int, int> params;
                req.params().convert(&params);
                add(req, std::get<0>(params), std::get<1>(params));

            } else if (method == "echo") {
                std::tuple<std::string> params;
                req.params().convert(&params);
                echo(req, std::get<0>(params));

            } else if (method == "echo_huge") {
                std::tuple<msgpack::type::raw_ref> params;
                req.params().convert(&params);
                echo_huge(req, std::get<0>(params));

            } else if (method == "err") {
                std::tuple<> params;
                req.params().convert(&params);
                err(req);

            } else if (method == "timeout") {
                ;
            } else {
                req.error(msgpack::rpc::NO_METHOD_ERROR);
            }

        } catch (msgpack::type_error&) {
            req.error(msgpack::rpc::ARGUMENT_ERROR);
            return;

        } catch (std::exception& e) {
            req.error(std::string(e.what()));
            return;
        }
    }


	void add(request req, int a1, int a2)
	{
		req.result(a1 + a2);
	}

	void echo(request req, const std::string& msg)
	{
		req.result(msg);
	}

	void echo_huge(request req, const msgpack::type::raw_ref& msg)
	{
		req.result(msg);
	}

	void err(request req)
	{
		req.error(std::string("always fail"));
	}
};

#endif //  H_MYECHO_SERVER_H
