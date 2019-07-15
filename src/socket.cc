#include <nan.h>
#include <sys/socket.h>
#include <errno.h>

using namespace v8;

static inline const char*
ErrorMessage() {
  return strerror(errno);
}

static inline Local<Value>
ExceptionFromError() {
  return Nan::Error(ErrorMessage());
}

void createSocket(const Nan::FunctionCallbackInfo<Value> &info)
{
  Isolate* isolate = info.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  Local<Object> params = info[0].As<Object>();
  Local<Value> ifname = params->Get(
    context,
    String::NewFromUtf8(isolate, "ifname", NewStringType::kNormal).ToLocalChecked()).ToLocalChecked();

  if (!ifname.IsEmpty()) {
    String::Utf8Value ifNameUtf8(isolate, ifname);
    std::string ifnameStr(*ifNameUtf8);

    if (setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, ifnameStr.c_str(), ifnameStr.length()+1) < 0) {
      Nan::ThrowError(ExceptionFromError());
    }
  }

  Local<Value> localAddress = params->Get(
    context,
    String::NewFromUtf8(isolate, "localAddress", NewStringType::kNormal).ToLocalChecked()).ToLocalChecked();
  if (!localAddress.IsEmpty()) {
    String::Utf8Value localAddressUtf8(isolate, localAddress);
    std::string localAddressStr(*localAddressUtf8);

    struct sockaddr_in addr;
    uv_ip4_addr(localAddressStr.c_str(), 0, &addr);

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
      Nan::ThrowError(ExceptionFromError());
    }
  }

  info.GetReturnValue().Set(Nan::New(sockfd));
}

NAN_MODULE_INIT(Init) {
  Nan::Set(target, Nan::New<String>("socket").ToLocalChecked(),
      Nan::GetFunction(Nan::New<FunctionTemplate>(createSocket)).ToLocalChecked());
}

NODE_MODULE(socket, Init)
