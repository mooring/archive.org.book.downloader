// ******************************************************************
// Copyright (c) mooring@live.com All rights reserved.
// This code is licensed under the GNU GENERAL PUBLIC LICENSE Version 3.
// THE CODE IS PROVIDED ?AS IS?, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH
// THE CODE OR THE USE OR OTHER DEALINGS IN THE CODE.
// ******************************************************************

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <unistd.h>
#endif

#define VER            "0.22"
#define HCAT           "\" -H \""
#define USER_AGENT     "Mozilla/5.0 (Windows NT 10.0; Win64; x64) "\
                       "AppleWebKit/537.36 (KHTML, like Gecko) "\
                       "Chrome/110.0.0.0 Safari/537.36 Edg/110.0.1587.50"
#define HEADER_1       "accept: image/png,image/svg+xml,image/*,*/*;q=0.8"
#define HEADER_2       "authority: %s"
#define HEADER_3       "referer: %s"
#define COOKIE_AUTH    "ol-auth-url=%%2F%%2Farchive.org%%2Fservices%%2Fborrow"\
                       "%%2FXXX%%3Fmode%%3Dauth"

#define DEBUG          " -A \"" USER_AGENT "\""
#define CURL_MAIN      "curl \"%s%04d%s\" -o leaf%03d.jpg"
#define HEADERS        " -H \"" HEADER_1 HCAT HEADER_2 HCAT HEADER_3 "\" "

#define COOKIES        COOKIE_AUTH
#define URL            CURL_MAIN DEBUG HEADERS

/**
  * @description setup curl params: 
  *     url's param:  prefix, suffix
  *     header param: referer
  * @param/host  : also the image domain
  * @param/bookId: read from the config.conf file bookid
  * @param/zipnum: the image page folder for zip param
  * @param/page  : the sequence's page number to be download
  */
static void setupUrlInfo(
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
    /*
     * if you want to download smaller image, change the scale 1 to 2 or 4
     */
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

/** @description: 
  *   prepare the curl cookie to download a the book page
  *
  * @param/bookId    : read from the config.conf file bookid
  * @param/donationId: read from the config.conf file donation
  * @param/sessionId : read from the config.conf file PHPSESSID
  * @param/loginSign : read from the config.conf file sig
  * @param/loanSign  : read from the config.conf file loan
  * @param/user      : read from the config.conf file user
  */
static void setupCookie(
    char (*cookie)[600],
    char *bookId,
    char *donationId,
    char *sessionId, 
    char *loginSign,
    char *loanSign,
    char *user
)
{
    sprintf(*cookie, 
        "PHPSESSID=%s; donation-identifier=%s; "
        "logged-in-sig=%s; logged-in-user=%s; "
        "br-loan-%s=1; loan-%s=%s;",
        sessionId, donationId,
        loginSign, user,
        bookId, bookId, loanSign
    );
}

/** @description: 
  *   download the page to local
  *
  * @param/page : page to download
  * @param/host : image host
  * @param/auth : configuated information for curl
  * @param/msg  : message to dispaly before download the page
  */
static void downloadImage(int page, char *host, char *auth[], char *msg)
{
    char str[3000] = {0};
    printf(
        "getting \"%s\" pages from leaf%03d %s"
        "\n-------------------------------------------"
        "--------------------------------------------\n",
        auth[5], page, msg
    );
    sprintf(
        // prefix page sufix page authority ref cookie proxy
        str, URL "-H \"cookie: " COOKIES "; %s\" %s", 
        auth[0], page, auth[1], page, host, auth[2], auth[3], auth[4]
    );
    // printf("%s\n", str);
    system(str);
    putchar('\n');
#if defined(_WIN32) || defined(_WIN64)
    Sleep(10);
#else
    sleep(100);
#endif
}

/** @description: 
  *   get config item from config.conf
  *
  * @param/key : key to get
  * @param/val : value to write back
  * 
  * @return    : 0 success, 1 fail
  */
static int getBookItem(FILE *fp, char *key, char (*val)[400])
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

/** @description: 
  *   get all config items from config.conf
  *
  * @param/config : all configurations from config.conf file
  * 
  */
static void getBookAuthConf(char *config[10])
{
    char buff[400]        = {0};
    char authority[100]   = {0};
    char proxy[100]       = {0};
    char bookId[50]       = {0};
    char user[50]         = {0};
    char zipnum[50]       = {0};
    char sessionId[50]    = {0};
    char donationId[50]   = {0};
    char loanSign[50]     = {0};
    char loginSign[300]   = {0};
    char bookTitle[300]   = {0};
    FILE *fp              = fopen("config.conf", "r");
    if(0 == getBookItem(fp, "authority", &buff)){
        sprintf(authority, "%s", buff);
    }
    if(0 == getBookItem(fp, "proxy", &buff)){
        if(strlen(buff) > 0){
            sprintf(proxy, "--proxy \"%s\"", buff);
        }
    }
    if(0 == getBookItem(fp, "zipnum", &buff)){
        sprintf(zipnum, "%s", buff);
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
    if(0 == getBookItem(fp, "title", &buff)){
        sprintf(bookTitle, "%s", buff);
    }
    
    strcpy(config[0], proxy);
    strcpy(config[1], zipnum);
    strcpy(config[2], authority);
    strcpy(config[3], bookId);
    strcpy(config[4], user);
    strcpy(config[5], sessionId);
    strcpy(config[6], donationId);
    strcpy(config[7], loanSign);
    strcpy(config[8], loginSign);
    strcpy(config[9], bookTitle);
    fclose(fp);
}

/** @description: 
  *   configuration and curl prepare entry
  *
  *   @param/conf : curl configurations for downloadImage
  *   @param/host : image host to be download
  */
static void setupAuth(char *conf[6], char (*host)[100])
{
    char prefix[2400]     = {0};
    char suffix[200]      = {0};
    char referer[200]     = {0};
    char cookie[600]      = {0};
   
    char authority[100]   = {0};
    char proxy[100]       = {0};
    char bookId[50]       = {0};
    char user[50]         = {0};
    char zipnum[10]       = {0};
    char sessionId[50]    = {0};
    char donationId[50]   = {0};
    char loanSign[50]     = {0};
    char loginSign[300]   = {0};
    char bookTitle[300]   = {0};
    char *bookConf[10] = {
        proxy, zipnum, authority, bookId, 
        user, sessionId, donationId,
        loanSign, loginSign, bookTitle
    };
    getBookAuthConf(bookConf);
    sprintf(*host, authority);
    setupCookie(
        &cookie, 
        bookId, donationId, sessionId, 
        loginSign, loanSign, user
    );
    setupUrlInfo(
        &prefix, &suffix, &referer, 
        bookId, *host, zipnum, 400
    );
    strcpy(conf[0], prefix);
    strcpy(conf[1], suffix);
    strcpy(conf[2], referer);
    strcpy(conf[3], cookie);
    strcpy(conf[4], proxy);
    strcpy(conf[5], bookTitle);
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
    char title[300]      = {0};
    char msg[100]        = {0};
    char *conf[6]        = {prefix, suffix, referer, cookie, proxy, title};
    FILE *fp             = fopen("config.conf", "r");
    puts("\narchive.org book downloader v" VER " by mooring[at]live.com\n" );
    if(fp == NULL){
        printf(
            "Error: config.conf file not found at current folder\n"
            "please use archiveOrgAssistant script of Tempermonkey extension\n"
            "put the config.conf file along with %s\n",
            argv[0]
        );
        exit(404);
    }
    fclose(fp);
    setupAuth(conf, &host);
    if(argc == 1)
    {
        printf("%s start/start end\n", argv[0]);
    }
    else if(argc == 2)
    {
        start = atoi(argv[1]);
        sprintf(
            msg, 
            "to leaf%03d [%d/%d]",
            start, 1, 1
        );
        downloadImage(start, host, conf, msg);
    }
    else
    {
        if(argc == 3){
            start = atoi(argv[1]);
            end   = atoi(argv[2]);
            for(i=start; i<end+1; i++){
                sprintf(
                    msg, 
                    "to leaf%03d [%d/%d]", 
                    end, ++j, end-start+1
                );
                downloadImage(i, host, conf, msg);
            }
        }
    }
    return 0;
}

