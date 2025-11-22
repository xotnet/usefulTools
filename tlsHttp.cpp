#include <openssl/ssl.h>
#include <stdio.h>
#include "net.c"
#include "smartPtr.h"

void tlsHttp(const char* domain, const char* port, char* readTo, int readToLen) {
    char localDomain[512] = "";
    char path[512] = "";
    for (int i = 0; domain[i] != 0; ++i) {
        if (domain[i] == '/') {
            strcpy(path, domain+i+1);
            break;
        }
        localDomain[i] = domain[i];
    }

    char ip[256];
    if (resolve_net(localDomain, ip, dnsA) != 0) {
        printf("Resolve host error!\n");
        return;
    }
    for (int i = 0; i<256; ++i) {if (ip[i] == ';') {ip[i] = 0; break;}}
    printf("Connecting to %s...\n", ip);
    int socket = connect_net(ip, port, setTCP | setIPv4);
    if (socket <= 0) {
        printf("TCP connect error!\n");
        return;
    }
    
    SSL_CTX* ctx = SSL_CTX_new(TLS_client_method());
    if (ctx == nullptr) {
        printf("Openssl CTX error!\n");
        return;
    }

    SSL *ssl = SSL_new(ctx);
    if (ssl == nullptr) {
        printf("SSL_new() failed\n");
        return;
    }

    SSL_set_fd(ssl, socket);
    SSL_set_tlsext_host_name(ssl, localDomain);
    int e = SSL_connect(ssl);
    if (e <= 0) {
        printf("Handshake error! %d\n", e);
    }
    printf("Connected %s\n", SSL_get_cipher(ssl));

    int len = sprintf(readTo, "GET /%s HTTP/1.1\r\nHost: %s\r\nUser-agent: chrome\r\nConnection: Close\r\n\r\n", path, localDomain);
    SSL_write(ssl, readTo, len);

    SSL_read(ssl, readTo, readToLen);

    close_net(socket);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
}

int main() {
    smartPtr<char> buf(4096);
    buf.erase();
    tlsHttp("browserleaks.com/ip", "443", buf.get(), 4096);
    printf("%s\n", buf.get());
}
