ex11: advice_client.c advice_server.c ikkp_server_thread.c ikkp_server_fork.c
	gcc advice_client.c -o advice_client
	gcc advice_server.c -o advice_server
	gcc ikkp_server_thread.c -lpthread -o ikkp_server_thread
	gcc ikkp_server_fork.c -o ikkp_server_fork
clean: advice_client advice_server ikkp_server_thread ikkp_server_fork
	rm -f advice_client advice_server ikkp_server_thread ikkp_server_fork
