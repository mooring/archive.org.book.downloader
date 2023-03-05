# archive.org book downloader
download books from archive.org 

![image](https://user-images.githubusercontent.com/1917297/220877106-3b5f3d12-7fc4-456e-8a93-856ff578c196.png)

#### Full version "Windows NT Shell Scripting" download by this tool can be **download** bellow:

- book title: **Windows NT Shell Scripting**
- book autho: Tim Hill
- book format: pdf
- book size : 192MB
- book pages: 404
- book url on baidu cloud: https://pan.baidu.com/s/1rmLvsn6hD57L3sb0NmSzYA?pwd=8fle
- baidu cloud password：8fle
- book url on aliyun cloud: https://www.aliyundrive.com/s/oFwUJpTQxV2
- aliyun cloud password: a6w5


### 1. first download Tampermonkey 
- https://www.tampermonkey.net/

### 2. install archiveOrgAssistant script
- https://greasyfork.org/en/scripts/460562-archiveorgassistant

### 3. pick a book from archive.org and borrow it for next step
- https://archive.org/details/inlibrary


### 4. compile the program from source code
-  `cl get.c`

![image](https://user-images.githubusercontent.com/1917297/220957059-90399112-ac0c-4758-a857-501b1f6b2f5d.png)


### 5. download the configuration
- click archiveOrgAssistant
- in the popup menu, select "get Configuration" menu item
- input the http proxy string if has any
- save the config.conf to downloader(ie. get.exe) folder

![image](https://user-images.githubusercontent.com/1917297/220851488-26c212a3-b869-41f8-8135-dc455618d78c.png)

### 6. run the program to get pages
- `get 1` get first page
- `get 103 110` get pages from 103 to 110

![image](https://user-images.githubusercontent.com/1917297/220956965-afd84808-e59a-4859-8be7-8e20838eb973.png)

compiled binary file **[get.exe](https://github.com/mooring/archive.org.book.downloader/blob/main/demo/get.exe)** can be **[download](https://github.com/mooring/archive.org.book.downloader/blob/main/demo/get.exe) here** 
just click this link: https://github.com/mooring/archive.org.book.downloader/raw/main/demo/get.exe to **download**

### 7. use jpeg2pdf to merge pages into a pdf

```bash
jpeg2pdf.exe *.jpg -o book_name.pdf
```

jpeg2pdf can be download https://sourceforge.net/projects/jpeg2pdf/


### enjoy

author: mooring

source: https://github.com/mooring/archive.org.book.downloader

website: https://codernote.club/


