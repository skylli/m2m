/*
 * protocol_m2m.h
 * description: m2m protocol declaration file.
 *	Created on: 2018-1-13
 *  	Author: skylli
*/
#ifndef _M2M_PROTOCOL_H_
#define _M2M_PROTOCOL_H_
#include "../../../include/m2m.h"
#include "../../../include/util.h"

typedef struct NET_ENC_T{
    Encrypt_type_T type;
    u8 keylen;
    u16 unused;
    u8 *p_enckey;  //秘钥
}Net_enc_T;


typedef struct M2M_PROTO_CMD_ARG_T{
    u8 messageid;
    u32 ctoken;
    u32 stoken;
    M2M_id_T src_id;
    M2M_id_T dst_id;
    Net_enc_T *p_enc;
    
    int socket_fd;
    M2M_Address_T address;
    u16 payloadlen;
    u8 *p_payload;

}M2M_Proto_Cmd_Arg_T;


typedef struct M2M_PROTO_RECV_RAWPKT_T{
    M2M_id_T dst_id;
    M2M_id_T src_id;

    u8 msgid;
    int socket_fd;
    M2M_Address_T remote;
    Encrypt_type_T enc_type;

    u32 stoken;
    M2M_packet_T payload;
}M2M_proto_recv_rawpkt_T;
typedef struct M2M_PROTO_DEC_RECV_PKT_T{

    Net_enc_T *p_enc;

    u8 cmd;
    u8 msgid;
    u16 code;
    u32 ctoken;
    M2M_packet_T payload;
}M2M_proto_dec_recv_pkt_T;
typedef struct M2M_DEC_ARGS_T{
    M2M_proto_dec_recv_pkt_T *p_dec;
    M2M_proto_recv_rawpkt_T *p_rawpkt;

}M2M_dec_args_T;

typedef struct M2M_PROTO_ACK_T{
    
    int socket_fd;
    u8 msgid;
    u16 code; // ack code.

    u32 ctoken;
    u32 stoken;
    M2M_id_T src_id;
    M2M_id_T dst_id;
    Net_enc_T *p_enc;
    M2M_Address_T remote_addr;
    M2M_packet_T payload;
}M2M_proto_ack_T;
// 定义协议相关，协议类型(todo)，协议入口函数
typedef struct M2M_PROTOCOL_T{

    u8 ver;
    u16 local_port;
    int socket_fd;
    m2m_func func_proto_ioctl;
    M2M_Address_T remote_addr;
}M2M_Protocol_T;
typedef struct M2M_PROTOCOL_RELAY_T{

    int socket_fd;
    M2M_Address_T *p_remote_addr;
    M2M_packet_T *p_payload;
}M2M_protocol_relay_T;



// 远端主动发送过来的命令，注意这时候 本地没有该包 session的任何信息.
typedef enum M2M_PROTO_IOCTL_CMD_T{

    M2M_PROTO_IOC_CMD_NONE  = 0,
    M2M_PROTO_IOC_CMD_SESSION_CREAT_RQ = 1,
    M2M_PROTO_IOC_CMD_SESSION_CREAT_ACK,
    M2M_PROTO_IOC_CMD_TOKEN_RQ,
    M2M_PROTO_IOC_CMD_TOKEN_ACK,
    M2M_PROTO_IOC_CMD_SETKEY_RQ, // 5
    M2M_PROTO_IOC_CMD_SETKEY_ACK,
    M2M_PROTO_IOC_CMD_PING_RQ,
    M2M_PROTO_IOC_CMD_PING_ACK,
    M2M_PROTO_IOC_CMD_DATA_RQ,
    M2M_PROTO_IOC_CMD_DATA_ACK, // 10
    M2M_PROTO_IOC_CMD_SESSION_DESTORY_RQ, 
    M2M_PROTO_IOC_CMD_SESSION_DESTORY_ACK,
    M2M_PROTO_IOC_CMD_RECVPKT_RQ,
    M2M_PROTO_IOC_CMD_DECODE_PKT_RQ,   // 对接收包进行分拆。
    M2M_PROTO_IOC_CMD_ERR_PKT_RQ,      //15 包解析层面出错，秘钥、crc、protocol 错误
    M2M_PROTO_IOC_CMD_ERR_PKT_ACK,     // 回应包解析出错。

#ifdef CONF_BROADCAST_ENABLE
    M2M_PROTO_IOC_CMD_BROADCAST_SEND, // 17
    M2M_PROTO_IOC_CMD_BROADCAST_ACK,
#endif // CONF_BROADCAST_ENABLE

    M2M_PROTO_IOC_CMD_RELAY,
    
    M2M_PROTO_IOC_CMD_ONLINK_CHECK,
    M2M_PROTO_IOC_CMD_ONLINK_CHECK_ACK,
    
    M2M_PROTO_IOC_CMD_MAX
}M2M_Proto_Ioctl_Cmd_T;

typedef enum M2M_PROTO_CMD_T{

    M2M_PROTO_CMD_NONE  = 0,
    M2M_PROTO_CMD_SESSION_CREAT_RQ = 1,
    M2M_PROTO_CMD_SESSION_CREAT_ACK,// 2
    M2M_PROTO_CMD_TOKEN_RQ,// 3
    M2M_PROTO_CMD_TOKEN_ACK,//4
    M2M_PROTO_CMD_SETKEY_RQ,// 5
    M2M_PROTO_CMD_SETKEY_ACK,
    M2M_PROTO_CMD_PING_RQ,
    M2M_PROTO_CMD_PING_ACK,
    M2M_PROTO_CMD_DATA_RQ,
    M2M_PROTO_CMD_DATA_ACK,//10
    M2M_PROTO_CMD_SESSION_DESTORY_RQ,
    M2M_PROTO_CMD_SESSION_DESTORY_ACK,

    M2M_PROTO_CMD_RECVPKT,
    M2M_PROTO_CMD_DECODE_PKT_RQ,   //对接收包进行分拆。
    M2M_PROTO_CMD_ERR_PKT_RQ,
    M2M_PROTO_CMD_ERR_PKT_ACK,

#ifdef CONF_BROADCAST_ENABLE
    M2M_PROTO_CMD_BROADCAST_RQ,
    M2M_PROTO_CMD_BROADCAST_ACK,
#endif // CONF_BROADCAST_ENABLE

    M2M_PROTO_CMD_RELAY,

    M2M_PROTO_CMD_ONLINK_CHECK_RQ,
    M2M_PROTO_CMD_ONLINK_CHECK_ACK,
    
    M2M_PROTO_CMD_ACK,
    
    M2M_PROTO_CMD_MAX
}M2M_Proto_Cmd_T;
/*********************** TOOLS *********************/
#define _DEV_ID_EQUAL(a,b,n)  ( memcmp(&a, &b,n) == 0 )
#define DEV_ID_EQUAL(a,b)     ( _DEV_ID_EQUAL(a,b,sizeof(M2M_id_T)) )
#define CPY_DEV_ID(d,s)         ( mcpy((u8*) &d, (u8*)&s,sizeof(M2M_id_T)) )

#define DEV_ID_PRINT(level,devid, head)  m2m_debug_level_noend(level,\
                                                        "%s: %x%x%x%x:%x%x%x%x",head,\
                                                        devid.id[0],devid.id[1],devid.id[2],devid.id[3],\
                                                        devid.id[4],devid.id[5],devid.id[6],devid.id[7])
#define DEV_ADDR_PRINT(addr_in)  m2m_printf("Address:%u.%u.%u.%u",addr_in.ip[0], \
                                                addr_in.ip[1], addr_in.ip[2],addr_in.ip[3])
                                                
#define DEV_INFO_PRINT(level,devid,devaddr,headlog) do{ \
                                                    m2m_debug_level_noend(level,devid,headlog);\
                                                    DEV_ADDR_PRINT(level,devaddr);}while(0)
                                                    
#define DEV_ID_LOG_PRINT(level,devid, head,format,...)   do{    DEV_ID_PRINT(level,devid,head);     \
                                                                m2m_printf(format,##__VA_ARGS__);}while(0)
                                                                
#define PKT_INFO_PRINT(level,msgid,cmd,ctoken,stoken,addr,hdlog)    do{ m2m_debug_level_noend(level,"%s :: ",hdlog);\
                                                                        m2m_printf("message  id= %x \t cmd = %x \t ctoken = %x \t stoken = %x \t",\
                                                                            msgid,cmd,ctoken,stoken); \
                                                                        DEV_ADDR_PRINT(addr); m2m_printf("\r\n");}while(0)

/*
* Description:
*   1.建立 socket,绑定端口.
*   2.注册 协议处理函数.
**/
M2M_Return_T m2m_protocol_init(M2M_Protocol_T *p_proto);
M2M_Return_T m2m_protocol_deInit(M2M_Protocol_T *p_proto);

#endif /* _M2M_PROTOCOL_H_ */
