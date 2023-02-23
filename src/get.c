#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

#define HEADER_CAT     "\" -H \""
#define USER_AGENT     "Mozilla/5.0 (Windows NT 10.0; Win64; x64) "\
                       "AppleWebKit/537.36 (KHTML, like Gecko) "\
                       "Chrome/110.0.0.0 Safari/537.36 Edg/110.0.1587.50"
#define HEADER_1       "accept: image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8"
#define HEADER_2       "authority: %s"
#define HEADER_3       "referer: %s"
#define COOKIE_AUTH    "ol-auth-url=%%2F%%2Farchive.org%%2Fservices%%2Fborrow%%2FXXX%%3Fmode%%3Dauth"

#define DEBUG          " -A \"" USER_AGENT "\""
#define CURL_MAIN      "curl \"%s%04d%s\" -o leaf%03d.jpg"
#define HEADERS        " -H \"" HEADER_1 HEADER_CAT HEADER_2 HEADER_CAT HEADER_3 "\" "

#define COOKIES        COOKIE_AUTH
#define URL            CURL_MAIN DEBUG HEADERS

void setupUrlInfo(
    char (*prefix)[2400], 
    char (*suffix)[200], 
    char (*referer)[200],
    char *bookId,
    char *host,
    char *zipnum,
    int  page
)
{
    const char *path = "/BookReader/BookReaderImages.php?zip=";
    sprintf(
        *prefix,
        "https://%s%s/%s/items/%s/%s_jp2.zip&file=%s_jp2/%s_",
        host, path, zipnum, bookId, bookId, bookId, bookId
    );
    sprintf(
        *suffix,
        ".jp2&id=%s&scale=1&rotate=0",
        bookId
    );
    sprintf(
        *referer,
        "https://archive.org/details/%s/page/%d/mode/1up?view=theater",
        bookId, page
    );
}
void setupCookie(
    char (*cookie)[600],
    char *bookId,
    char *donationId,
    char *phpSessionId, 
    char *loginSign,
    char *loanSign,
    char *email
)
{
    sprintf(*cookie, 
        "PHPSESSID=%s; donation-identifier=%s; "
        "logged-in-sig=%s; logged-in-user=%s; "
        "br-loan-%s=1; loan-%s=%s;",
        phpSessionId, donationId,
        loginSign, email, 
        bookId, bookId, loanSign
    );
}

void getImage(int page, char *host, char *auth[], char *msg)
{
    char str[3000] = {0};
    printf(
        "getting image from leaf%03d.jpg %s" 
        "\n----------------------------------------------------------\n",
        page, msg
    );
    sprintf(
        // prefix page sufix page authority ref cookie proxy
        str, URL "-H \"cookie: " COOKIES "; %s\" %s", 
        auth[0], page, auth[1], page, host, auth[2], auth[3], auth[4]
    );
    //printf("%s\n", str);
    system(str);
    putchar('\n');
    Sleep(10);
}

int getBookItem(FILE *fp, char *key, char (*val)[400])
{
    int  ret = 1, max_read = 20, index = 0;
    char buff[400]   = {0};
    char format[100] = {0};
    fseek(fp, 0, SEEK_SET);
    sprintf(format, "%s=%%s\\n", key);
    while(fgets(buff, 400, fp) != NULL){
        char *find = strstr(buff, key);
        if( find ){
            find += strlen(key)+1;
            *(find + strlen(find) - 1) = 0;
            sprintf(*val, "%s", find);
            ret = strlen(*val) == 0;
            break;
        }
        if(++index > max_read){
            break;
        }
    }
    return ret;
}

void getBookAuthConf(char *config[8])
{
    char buff[400]        = {0};
    char authority[100]   = {0};
    char proxy[100]       = {0};
    char bookId[50]       = {0};
    char user[50]         = {0};
    char sessionId[50]    = {0};
    char donationId[50]   = {0};
    char loanSign[50]     = {0};
    char loginSign[300]   = {0};
    FILE *fp              = fopen("config.conf", "r");

    if(0 == getBookItem(fp, "authority", &buff)){
        sprintf(authority, "%s", buff);
    }
    if(0 == getBookItem(fp, "proxy", &buff)){
        if(strlen(buff) > 0){
            sprintf(proxy, "--proxy \"%s\"", buff);
        }
    }
    if(0 == getBookItem(fp, "bookid", &buff)){
        sprintf(bookId, "%s", buff);
    }
    if(0 == getBookItem(fp, "user", &buff)){
        sprintf(user, "%s", buff);
    }
    if(0 == getBookItem(fp, "PHPSESSID", &buff)){
        sprintf(sessionId, "%s", buff);
    }
    if(0 == getBookItem(fp, "donation", &buff)){
        sprintf(donationId, "%s", buff);
    }
    if(0 == getBookItem(fp, "loan", &buff)){
        sprintf(loanSign, "%s", buff);
    }
    if(0 == getBookItem(fp, "sig", &buff)){
        sprintf(loginSign, "%s", buff);
    }
    strcpy(config[0], proxy);
    strcpy(config[1], authority);
    strcpy(config[2], bookId);
    strcpy(config[3], user);
    strcpy(config[4], sessionId);
    strcpy(config[5], donationId);
    strcpy(config[6], loanSign);
    strcpy(config[7], loginSign);
    fclose(fp);
}

void setupAuth(char *conf[5], char (*host)[100])
{
    char prefix[2400]    = {0};
    char suffix[200]     = {0};
    char referer[200]    = {0};
    char cookie[600]     = {0};
    
    char authority[100]   = {0};
    char proxy[100]       = {0};
    char bookId[50]       = {0};
    char user[50]         = {0};
    char sessionId[50]    = {0};
    char donationId[50]   = {0};
    char loanSign[50]     = {0};
    char loginSign[300]   = {0};
    char *bookConf[8] = {
        proxy, authority, bookId, 
        user, sessionId, donationId,
        loanSign, loginSign
    };
    getBookAuthConf(bookConf);
    sprintf(*host, authority);
    setupCookie(
        &cookie, 
        bookConf[2], bookConf[5], bookConf[4], 
        bookConf[7], bookConf[6], bookConf[3]
    );
    setupUrlInfo(
        &prefix, &suffix, &referer, 
        bookConf[2], *host, "32", 400
    );
    strcpy(conf[0], prefix);
    strcpy(conf[1], suffix);
    strcpy(conf[2], referer);
    strcpy(conf[3], cookie);
    strcpy(conf[4], proxy);
}

int main(int argc, char *argv[])
{
    int i = 1, j = 0, start = 1, end = 1;
    char prefix[2400]    = {0};
    char suffix[200]     = {0};
    char referer[200]    = {0};
    char cookie[600]     = {0};
    char proxy[100]      = {0};
    char host[100]       = {0};
    char msg[100]        = {0};
    char *conf[5]        = {prefix, suffix, referer, cookie, proxy};
    
    setupAuth(conf, &host);
    
    if(argc == 1)
    {
        printf("%s start/start end", argv[0]);
    }
    else if(argc == 2)
    {
        start = atoi(argv[1]);
        sprintf(
            msg, 
            "to leaf%03d of %d/%d",
            start, 1, 1
        );
        getImage(start, host, conf, msg);
    }
    else
    {
        if(argc == 3){
            start = atoi(argv[1]);
            end   = atoi(argv[2]);
            printf("getting page %d - %d\n", start, end);
            for(i=start; i<end+1; i++){
                sprintf(
                    msg, 
                    "to leaf%03d of %d/%d", 
                    end, ++j, end-start+1
                );
                getImage(i, host, conf, msg);
            }
        }
    }
    return 0;
}

