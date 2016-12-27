#include <qdebug.h>
#include "uartodom.h"

namespace zw {

int32_t UartOdom::StartScan(void)
{
  static uint8_t scanflags = 0;

  createPthread = 1;
  if (scanflags == 0)
  {
    scanflags = 1;
    pthread_create(&id, NULL, UartCreatePthread, (void *)this);
  }
}

int32_t UartOdom::StopScan(void)
{

}

void *UartOdom::DoPthread(void)
{
  while(createPthread)
  {
    int32_t nRet=0;
    uint8_t data_buf[UARTBUFMAX];
    nRet= read(fd, data_buf, UARTBUFMAX);
    if( nRet >0 )
    {
      Analysis(data_buf, nRet);
    }
    usleep(30000);
  }
}

void UartOdom::Analysis(uint8_t *arry, int nRet)
{
    static QByteArray buf;
    buf.append((char *)arry,nRet);
    int32_t startIndex=0;
    int32_t endIndex = buf.count();
    ParaGetSet packInfo={0,0,0,nullptr};
    while(m_paraModbus.UnPackparas((const byte*)buf.data(),startIndex ,endIndex, packInfo))
    {
        if(packInfo.fuc == R_HOLDING_REGISTER){
            packInfo.data=new int32_t[packInfo.len];
            modbus.GetAddressValue(packInfo);
            packInfo.fuc=W_MULTI_REGISTER;
            int32_t size=FIXEDLENGTH +packInfo.len*4;
            byte* msg =new byte[size];
            if(size!=m_paraModbus.PackParas(packInfo,msg))
                qDebug () <<"Error in pack size!";
            else
                write(fd,(char*)msg ,size);
            delete msg;
        }
        else if(packInfo.fuc == W_MULTI_REGISTER){
            modbus.SetAddressValue(packInfo);

//            int32_t dat[2];
//            zw::ParaGetSet  packInfo = {zw::R_HOLDING_REGISTER,2,zw::CONTROL,dat};
//            modbus.GetAddressValue(packInfo);
//            zw::Float2Int32 ff1,ff2;
//            ff1.i=dat[0];
//            ff2.i=dat[1];
//            qDebug () <<ff1.f<<ff2.f;
        }
        delete packInfo.data;
        packInfo={0,0,0,nullptr};
    }
    if(startIndex!=0){
        buf.remove(0,startIndex);
        startIndex=0;
    }
}

}