#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 
#define IPSTR "114.212.10.206"
#define PORT 8088
#define BUFSIZE 1024
extern char stu_id[20];
 
int pa_audio_ele(char* opera)
{
        int sockfd, ret;
        struct sockaddr_in servaddr;
        char str1[4096];
 
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
                exit(0);
        };
 
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(PORT);
        if (inet_pton(AF_INET, IPSTR, &servaddr.sin_addr) <= 0 ){
                exit(0);
        };
 
        if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
                exit(0);
        }
 
 
        memset(str1, 0, 4096);
        strcat(str1, "GET /");
        strcat(str1, opera);
        strcat(str1, " HTTP/1.1\n");
	strcat(str1, "Host: ");
	strcat(str1, IPSTR);
	strcat(str1, "\n");
	strcat(str1, "STU_ID: ");
	strcat(str1, stu_id);
	strcat(str1, "\n");
        strcat(str1, "\n\n");
        strcat(str1, "\r\n\r\n");
        printf("%s\n",str1);
 
        ret = write(sockfd,str1,strlen(str1));
        if (ret < 0) {
                return 0;
        }else{
        	
        }
        close(sockfd);
        return 1;
}

void web_audio_play()
{
	if (pa_audio_ele("paPlay")==0) return;
	printf("web audio play\n");
}

void web_audio_pause()
{
	if(pa_audio_ele("paPause")==0) return;
	printf("web audio pause\n");
}

void web_audio_close()
{
	if(pa_audio_ele("paStop")==0) return;
	printf("web audio close\n");
}





