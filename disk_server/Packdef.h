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
#define _uploadfile_continue 1
#define _uploadfile_flash 2
#define _uploadfile_isuploaded 3

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
    //删除文件
    //搜索文件

    //分享
    //提取链接
    //添加好友


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
// struct STRU_UPLOADFILEBLOCK_RS :public STRU_BASE{
//     STRU_UPLOADFILEBLOCK_RS(){
//         m_nType = _default_upload_fileinfo_rs;
//     }
//     char m_szFileName[MAXSIZE];
//     char m_szResult;
//     long m_pos;         //偏移量
// };

#endif // PACKDEF_H























