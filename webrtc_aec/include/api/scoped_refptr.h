// Simple implementation of scoped_refptr for compilation purposes
#ifndef API_SCOPED_REFPTR_H_
#define API_SCOPED_REFPTR_H_

namespace webrtc {

// Forward declaration
class RefCountInterface;

// Simple scoped_refptr implementation
template <typename T>
class scoped_refptr {
public:
  scoped_refptr() : ptr_(nullptr) {}
  scoped_refptr(T* p) : ptr_(p) {
    if (ptr_) ptr_->AddRef();
  }
  scoped_refptr(const scoped_refptr<T>& r) : ptr_(r.ptr_) {
    if (ptr_) ptr_->AddRef();
  }
  ~scoped_refptr() {
    if (ptr_) ptr_->Release();
  }

  scoped_refptr<T>& operator=(const scoped_refptr<T>& r) {
    if (ptr_ != r.ptr_) {
      if (ptr_) ptr_->Release();
      ptr_ = r.ptr_;
      if (ptr_) ptr_->AddRef();
    }
    return *this;
  }

  T* get() const { return ptr_; }
  T& operator*() const { return *ptr_; }
  T* operator->() const { return ptr_; }
  bool operator==(const scoped_refptr<T>& other) const { return ptr_ == other.ptr_; }
  bool operator!=(const scoped_refptr<T>& other) const { return ptr_ != other.ptr_; }

private:
  T* ptr_;
};

}  // namespace webrtc

#endif  // API_SCOPED_REFPTR_H_
