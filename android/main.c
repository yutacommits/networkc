#include <jni.h>
#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <android/log.h>

#define LOG_TAG "NativeC"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__);

JNIEXPORT jstring JNICALL
Java_com_example_capp_MainActivity_lookupHost(JNIEnv *env, jobject obj, jstring hostname) {
    const char *ptr = (*env)->GetStringUTFChars(env, hostname, NULL);
    struct addrinfo hints, *res;
    char outputstr[1024] = "";

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    LOGD("Calling getaddrinfo() for: %s", ptr);

    if (getaddrinfo(ptr, NULL, &hints, &res) != 0) {
        LOGD("Error: getaddrinfo failed");
        return (*env)->NewStringUTF(env, "Error: Cannot resolve hostname\n");
    }

    struct addrinfo *p;
    for (p = res; p != NULL; p = p->ai_next) {
        void *addr;
        char ipstr[INET6_ADDRSTRLEN];

        if (p->ai_family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
        } else {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
        }

        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        strcat(outputstr, ipstr);
        strcat(outputstr, "\n");
    }

    freeaddrinfo(res);
    LOGD("Resolved IPs:\n%s", outputstr);

    return (*env)->NewStringUTF(env, outputstr);
}

