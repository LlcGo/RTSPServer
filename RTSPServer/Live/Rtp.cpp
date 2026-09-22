#include "Rtp.h"
#include <string>

RtpPacket::RtpPacket() :
    //mBuf(new uint8_t[4 + RTP_HEADER_SIZE + RTP_MAX_PKT_SIZE + 100]),
    mBuf((uint8_t*)malloc(4 + RTP_HEADER_SIZE + RTP_MAX_PKT_SIZE + 100)),
    mBuf4(mBuf + 4),
    mRtpHeader((RtpHeader*)mBuf4),
    mSize(0) {
}