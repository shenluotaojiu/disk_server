#ifndef PACKDEF_H
#define PACKDEF_H

#define _BEGIN_PROTOCOL_MAP static struct MyProtocolMap g_PMEntries[]={
#define _END_PROTOCOL_MAP     {0,0}, \
};
#define PM(x,y) {x,y},


#define _default_base   1
#define MAXSIZE  45
#define MAXFILENUM  15
#define SQLLEN 256
#define ONEPAGE 4096


// 注册成功失败
#define _register_err   0
#define _register_success  1

// 登录成功失败
#define _login_password_err   0
#define _login_usernoexists   2
#define _login_success        1

// 上传文件方式
#define _uploadfile_normal 0
#define _uploadfile_continue 1      //断点续传
#define _uploadfile_flash 2         //闪传
#define _uploadfile_isuploaded 3

//文件下载请求返回
#define _dowloadfilerq_succeed 0
#define _dowloadfilerq_fail 1

//文件下载方式
#define _downloadfile_normal 0
#define _downloadfile_continue 1    //断点续下

// 分享文件返回类型
#define _sharelink_succeed 0
#define _sharelink_fail 1

//申请账号
#define _default_register_rq      _default_base +1
#define _default_register_rs      _default_base +2
//登录
#define _default_login_rq      _default_base +3
#define _default_login_rs      _default_base +4
//获取文件列表
#define _default_getfilelist_rq      _default_base +5
#define _default_getfilelist_rs      _default_base +6
//上传文件
#define _default_upload_fileinfo_rq _default_base+7
#define _default_upload_fileinfo_rs _default_base+8
//上传文件块
#define _default_upload_fileblock_rq _default_base+9
#define _default_upload_fileblock_rs _default_base+10

//下载文件
#define _default_download_fileinfo_rq _default_base+11
#define _default_download_fileinfo_rs _default_base+12

//下载文件块
#define _default_download_fileblock_rq _default_base+13
#define _default_download_fileblock_rs _default_base+14

//删除文件
//搜索文件

//分享
#define _default_sharelink_rq       _default_base + 15
#define _default_sharelink_rs       _default_base + 16
//提取链接
#define _default_getlink_rq       _default_base + 17
#define _default_getlink_rs       _default_base + 18


    //协议包
    //申请账号
    struct STRU_BASE {
protected:
    char m_nType;
};
struct STRU_REGISTER_RQ :public STRU_BASE{
    STRU_REGISTER_RQ(){
        m_nType = _default_register_rq;
    }
    char m_szName[MAXSIZE];
    char m_szPassword[MAXSIZE];
    long long m_tel;
};
struct STRU_REGISTER_RS :public STRU_BASE{
    STRU_REGISTER_RS(){
        m_nType = _default_register_rs;
    }
    char m_szResult;

};
//登录
struct STRU_LOGIN_RQ :public STRU_BASE{
    STRU_LOGIN_RQ(){
        m_nType = _default_login_rq;
    }
    char m_szName[MAXSIZE];
    char m_szPassword[MAXSIZE];

};
struct STRU_LOGIN_RS :public STRU_BASE{
    STRU_LOGIN_RS(){
        m_nType = _default_login_rs;
    }
    long long m_userID;
    char m_szResult;

};
//获取文件列表
struct STRU_GETFILELIST_RQ :public STRU_BASE{
    STRU_GETFILELIST_RQ(){
        m_nType = _default_getfilelist_rq;
    }
    long long m_userID;

};
struct FILEINFO{
    char m_szFileName[MAXSIZE];
    long long m_llFileSize;
    char m_szDateTime[MAXSIZE];
};
struct STRU_GETFILELIST_RS :public STRU_BASE{
    STRU_GETFILELIST_RS(){
        m_nType = _default_getfilelist_rs;
    }
    FILEINFO m_aryFile[MAXFILENUM];
    int m_nFileNum;
};
//上传文件
struct STRU_UPLOADFILEINFO_RQ :public STRU_BASE{
    STRU_UPLOADFILEINFO_RQ(){
        m_nType = _default_upload_fileinfo_rq;
    }
    long long m_userID;
    char m_szFileName[MAXSIZE];
    long long m_fileSize;
    char m_szMD5[MAXSIZE];
};
struct STRU_UPLOADFILEINFO_RS :public STRU_BASE{
    STRU_UPLOADFILEINFO_RS(){
        m_nType = _default_upload_fileinfo_rs;
    }
    char m_szFileName[MAXSIZE];
    char m_szResult;
    long m_pos;         //偏移量
    long long m_fileID;
};
struct STRU_UPLOADFILEBLOCK_RQ :public STRU_BASE{
    STRU_UPLOADFILEBLOCK_RQ(){
        m_nType = _default_upload_fileblock_rq;
    }
    long long m_userID;
    char m_szFileContent[ONEPAGE];
    long long m_fileID;
    long m_fileblocksize;
};
//分享
struct STRU_SHARELINK_RQ:public STRU_BASE{
    STRU_SHARELINK_RQ(){
        m_nType = _default_sharelink_rq;
    }
    long long m_userID;
    char m_szFileName[MAXSIZE];
    char m_szCode[MAXSIZE];
};
struct STRU_SHARELINK_RS:public STRU_BASE{
    STRU_SHARELINK_RS(){
        m_nType = _default_sharelink_rs;
    }
    char m_szFileName[MAXSIZE];
    char m_szResult;
};
struct STRU_GETLINK_RQ:public STRU_BASE{
    STRU_GETLINK_RQ(){
        m_nType = _default_getlink_rq;
    }
    long long m_userID;
    char m_szCode[MAXSIZE];
};
struct STRU_GETLINK_RS:public STRU_BASE{
    STRU_GETLINK_RS(){
        m_nType = _default_getlink_rs;
    }
    FILEINFO m_fileinfo;
};
// 文件下载
struct STRU_DOWNLOADFILE_RQ:public STRU_BASE
{
    STRU_DOWNLOADFILE_RQ() {
        m_nType = _default_download_fileinfo_rq;
    }
    long long userId;
    char fileName[MAXSIZE];
};

struct STRU_DOWNLOADFILE_RS:public STRU_BASE
{
    STRU_DOWNLOADFILE_RS() {
        m_nType = _default_download_fileinfo_rs;
    }
    long long f_id;
    char result;
    char fileName[MAXSIZE];
};

struct STRU_DOWNLOADBLOCK_RQ:public STRU_BASE
{
    STRU_DOWNLOADBLOCK_RQ() {
        m_nType = _default_download_fileblock_rq;
    }
    char m_szResult;    //如何传
    long m_pos;         //偏移量
    long long fileId;   //用来匹对信息的文件id
    long long fileSize;
};

struct STRU_DOWNLOADBLOCK_RS:public STRU_BASE
{
    STRU_DOWNLOADBLOCK_RS() {
        m_nType = _default_download_fileblock_rs;
    }
    char m_szFileContent[ONEPAGE];
    long long m_fileID;
    long long m_fileSize;
    long m_fileblocksize;
};


#endif // PACKDEF_H
