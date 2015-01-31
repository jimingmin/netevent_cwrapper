/*
 * net_interface.h
 *
 *  Created on: 2014Äê12ÔÂ31ÈÕ
 *      Author: jimm
 */

#include "net_interface.h"

int32_t main(int argc, char** argv)
{
	struct CAcceptor *pAcceptor = NULL;
	struct CConnector *pConnector = NULL;

	init_context_wrapper();

	//pAcceptor = create_acceptor_wrapper();
	//net_bind_wrapper(pAcceptor, "127.0.0.1", 10000);
	//destory_acceptor_wrapper(pAcceptor);

	pConnector = create_connector_wrapper();
	net_connect_wrapper(pConnector, "42.62.31.11", 8001);

	while(1)
	{
		net_run_wrapper();
	}
	//destory_connector_wrapper(pConnector);

	return 0;
}
